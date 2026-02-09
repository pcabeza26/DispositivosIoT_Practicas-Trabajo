#include "contiki.h"
#include "dev/leds.h"
#include <stdio.h>

/*---------------------------------------------------------------------------*/
PROCESS(tarea_1_process, "Tarea 1");
PROCESS(tarea_2_process, "Tarea 2");
PROCESS(tarea_3_process, "Tarea 3");

AUTOSTART_PROCESSES(&tarea_1_process, &tarea_2_process, &tarea_3_process);
/*---------------------------------------------------------------------------*/

// TAREA 3: Temporizador inicial de 3 segundos
PROCESS_THREAD(tarea_3_process, ev, data)
{
  static struct etimer et;
  PROCESS_BEGIN();

  etimer_set(&et, 3 * CLOCK_SECOND);
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  
  printf("Tarea 3: 3 segundos pasados, activando tareas 1 y 2\n");
  
  process_poll(&tarea_1_process);
  process_poll(&tarea_2_process);
  
  PROCESS_END();
}

/*---------------------------------------------------------------------------*/

// TAREA 1: Parpadeo LED verde cada 2 segundos tras activación
PROCESS_THREAD(tarea_1_process, ev, data)
{
  static struct etimer et;
  PROCESS_BEGIN();

  PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_POLL);
  etimer_set(&et, 2 * CLOCK_SECOND);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    leds_toggle(LEDS_GREEN);
    printf("Tarea 1: Toggle Verde\n");
    etimer_reset(&et);
  }

  PROCESS_END();
}

/*---------------------------------------------------------------------------*/

// TAREA 2: Parpadeo LED azul cada 4 segundos tras activación
PROCESS_THREAD(tarea_2_process, ev, data)
{
  static struct etimer et;
  PROCESS_BEGIN();

  PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_POLL);
  etimer_set(&et, 4 * CLOCK_SECOND);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    leds_toggle(LEDS_YELLOW);
    printf("Tarea 2: Toggle Azul\n");
    etimer_reset(&et);
  }

  PROCESS_END();
}