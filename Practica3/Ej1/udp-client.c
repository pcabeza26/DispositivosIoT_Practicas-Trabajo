#include "contiki.h"
#include "net/routing/routing.h"
#include "net/ipv6/simple-udp.h"
#include "sys/log.h"
#include <stdio.h>
#include <string.h>

#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 5678

static struct simple_udp_connection udp_conn;

PROCESS(udp_client_process, "UDP client");
AUTOSTART_PROCESSES(&udp_client_process);

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
  char response[32];
  snprintf(response, sizeof(response), "%.*s", datalen, (char *)data);

  LOG_INFO("Temperatura en Fahrenheit calculada por el servidor= '%s' grados\n", response);
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_client_process, ev, data)
{
  static struct etimer periodic_timer;
  static int counter = 0;
  uip_ipaddr_t root_ipaddr;

  PROCESS_BEGIN();

  LOG_INFO("Cliente UDP iniciado\n");

  simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL,
                      UDP_SERVER_PORT, udp_rx_callback);

  etimer_set(&periodic_timer, 5 * CLOCK_SECOND);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
    etimer_reset(&periodic_timer);

    if(NETSTACK_ROUTING.node_is_reachable() &&
       NETSTACK_ROUTING.get_root_ipaddr(&root_ipaddr)) {

      counter++;

      int temp_c = (counter % 2 == 1) ? 42 : 27;

      if(counter % 2 == 1)
        LOG_INFO("Enviando temperatura interna en Celsius = %d\n", temp_c);
      else
        LOG_INFO("Enviando temperatura externa en Celsius = %d\n", temp_c);

      char msg[16];
      snprintf(msg, sizeof(msg), "%d", temp_c);

      simple_udp_sendto(&udp_conn, msg, strlen(msg), &root_ipaddr);
    } else {
      LOG_INFO("No hay alcance aún\n");
    }
  }

  PROCESS_END();
}
