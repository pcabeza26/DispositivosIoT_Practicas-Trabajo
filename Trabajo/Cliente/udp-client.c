#include "contiki.h"
#include "net/routing/routing.h"
#include "net/ipv6/simple-udp.h"
#include "sys/log.h"

#include "arch/platform/nrf52840/common/temperature-sensor.h"
#include "dev/button-hal.h"
#include "dev/leds.h"

#include "formato_trama.h"

#define LOG_MODULE "UDP-CLIENT"
#define LOG_LEVEL LOG_LEVEL_INFO

#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 5678

#define SEND_INTERVAL   (5 * CLOCK_SECOND)
#define LED_INTERVAL    (CLOCK_SECOND / 2)

static struct simple_udp_connection udp_conn;

PROCESS(udp_client_process, "UDP client");
AUTOSTART_PROCESSES(&udp_client_process);

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_client_process, ev, data)
{
  static struct etimer send_timer;
  static struct etimer led_timer;
  static uip_ipaddr_t dest_ipaddr;
  static trama_temperatura_t trama;

  static uint8_t unidad_actual = UNIDAD_CELSIUS;
  static uint8_t alarma_activa = 0;

  PROCESS_BEGIN();

  LOG_INFO("Proceso UDP client arrancado\r\n");

  SENSORS_ACTIVATE(temperature_sensor);
  button_hal_init();

  simple_udp_register(&udp_conn,
                      UDP_CLIENT_PORT,
                      NULL,
                      UDP_SERVER_PORT,
                      NULL);

  etimer_set(&send_timer, SEND_INTERVAL);
  etimer_set(&led_timer, LED_INTERVAL);

  while(1) {
    PROCESS_WAIT_EVENT();

    /* Botón */
    if(ev == button_hal_press_event) {
      unidad_actual =
        (unidad_actual == UNIDAD_CELSIUS) ?
        UNIDAD_FAHRENHEIT : UNIDAD_CELSIUS;

      LOG_INFO("Unidad cambiada a %s\r\n",
        unidad_actual == UNIDAD_CELSIUS ? "Celsius" : "Fahrenheit");
    }

    /* LED alarma */
    if(ev == PROCESS_EVENT_TIMER && data == &led_timer) {
      if(alarma_activa) leds_toggle(LEDS_RED);
      else leds_off(LEDS_RED);
      etimer_reset(&led_timer);
    }

    /* Envío UDP */
    if(ev == PROCESS_EVENT_TIMER && data == &send_timer) {

      if(NETSTACK_ROUTING.node_is_reachable() &&
         NETSTACK_ROUTING.get_root_ipaddr(&dest_ipaddr)) {

        int32_t raw_temp = temperature_sensor.value(0);
        int32_t temp_c_base100 = raw_temp * 25;
        int32_t temp_envio_base100 = temp_c_base100;

        if(unidad_actual == UNIDAD_FAHRENHEIT) {
          temp_envio_base100 =
            (temp_c_base100 * 9 / 5) + 3200;
        }

        trama.bandera_inicio = TRAMA_BANDERA_INICIO;
        trama.id_trama       = TRAMA_ID_TEMPERATURA;
        trama.id_nodo        = 0xAA;
        trama.unidad         = unidad_actual;
        trama.temperatura    =
          (int16_t)((temp_envio_base100 * 16) / 100);
        trama.id_alarma      = 0x01;

        if(temp_c_base100 > 3500) {
          trama.estado_alarma = ALARMA_ACTIVA;
          alarma_activa = 1;
        } else {
          trama.estado_alarma = ALARMA_NO_ACTIVA;
          alarma_activa = 0;
        }

        simple_udp_sendto(&udp_conn,
                          &trama,
                          sizeof(trama),
                          &dest_ipaddr);

        LOG_INFO("Trama enviada (T=%ld.%02ld %s)\r\n",
          temp_envio_base100 / 100,
          temp_envio_base100 % 100,
          unidad_actual == UNIDAD_CELSIUS ? "C" : "F");
      }

      etimer_reset(&send_timer);
    }
  }

  PROCESS_END();
}


