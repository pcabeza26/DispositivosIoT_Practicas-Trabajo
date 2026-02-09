#include "contiki.h"
#include "net/routing/routing.h"
#include "net/ipv6/simple-udp.h"
#include "sys/log.h"

#include "formato_trama.h"

#define LOG_MODULE "UDP-SERVER"
#define LOG_LEVEL LOG_LEVEL_INFO

#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 5678

static struct simple_udp_connection udp_conn;

PROCESS(udp_server_process, "UDP server");
AUTOSTART_PROCESSES(&udp_server_process);

/*---------------------------------------------------------------------------*/
static void
udp_rx_callback(struct simple_udp_connection *c,
                const uip_ipaddr_t *sender_addr,
                uint16_t sender_port,
                const uip_ipaddr_t *receiver_addr,
                uint16_t receiver_port,
                const uint8_t *data,
                uint16_t datalen)
{
  if(datalen != sizeof(trama_temperatura_t)) {
    LOG_WARN("Trama inválida (%u bytes)\r\n", datalen);
    return;
  }

  const trama_temperatura_t *trama =
      (const trama_temperatura_t *)data;

  /* Temperatura en Q12.4 */
  int16_t temp_q12_4 = trama->temperatura;
  int16_t temp_entera = temp_q12_4 / 16;
  uint16_t temp_decimal = (temp_q12_4 % 16) * 100 / 16;

  const char *unidad_str =
      (trama->unidad == UNIDAD_CELSIUS) ? "C" : "F";

  const char *estado_alarma_str =
      (trama->estado_alarma == ALARMA_ACTIVA) ?
        "ACTIVA" : "NO_ACTIVA";

  /* FORMATO EXACTO DEL ENUNCIADO:
   * <ID_nodo>;<Unidad>;<Temperatura>;<ID_alarma>;<Estado_alarma>
   */
  printf("%02X;%s;%d.%02u;%u;%s\r\n",
         trama->id_nodo,
         unidad_str,
         temp_entera,
         temp_decimal,
         trama->id_alarma,
         estado_alarma_str);
}

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_server_process, ev, data)
{
  static struct etimer start_timer;

  PROCESS_BEGIN();

  /* Delay de arranque de 10 segundos */
  etimer_set(&start_timer, 10 * CLOCK_SECOND);
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&start_timer));

  LOG_INFO("Servidor UDP iniciado\r\n");
  LOG_INFO("Escuchando en puerto %u\r\n", UDP_SERVER_PORT);

  NETSTACK_ROUTING.root_start();

  simple_udp_register(&udp_conn,
                      UDP_SERVER_PORT,
                      NULL,
                      UDP_CLIENT_PORT,
                      udp_rx_callback);

  while(1) {
    PROCESS_YIELD();
  }

  PROCESS_END();
}
