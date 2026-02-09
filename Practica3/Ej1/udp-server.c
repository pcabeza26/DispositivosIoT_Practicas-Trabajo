#include "contiki.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "sys/log.h"
#include <stdio.h>
#include <stdlib.h>

#define LOG_MODULE "App"
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
  char received_str[32];
  int temp_c = 0;
  int temp_f = 0;
  char response[32];

  snprintf(received_str, sizeof(received_str), "%.*s", datalen, (char *)data);

  LOG_INFO("Info recibida del nodo: '%s' grados\n", received_str);

  LOG_INFO("Direccion = ");
  LOG_INFO_6ADDR(sender_addr);
  LOG_INFO_("\n");

  temp_c = atoi(received_str);

  LOG_INFO("TEMPERATURA RECIBIDA = %d\n", temp_c);

  temp_f = (temp_c * 9 / 5) + 32;

  LOG_INFO("Enviando temperatura en Fahrenheit = %d\n", temp_f);
  LOG_INFO("Destino = ");
  LOG_INFO_6ADDR(sender_addr);
  LOG_INFO_("\n");

  snprintf(response, sizeof(response), "%d", temp_f);


  //LOG_INFO("Temperatura en Fahrenheit calculada por el servidor= '%d' grados.\n", temp_f);

  simple_udp_sendto(&udp_conn, response, strlen(response), sender_addr);
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_server_process, ev, data)
{
  PROCESS_BEGIN();

  LOG_INFO("Servidor UDP iniciado\n");

  NETSTACK_ROUTING.root_start();

  simple_udp_register(&udp_conn, UDP_SERVER_PORT, NULL,
                      UDP_CLIENT_PORT, udp_rx_callback);

  PROCESS_END();
}
