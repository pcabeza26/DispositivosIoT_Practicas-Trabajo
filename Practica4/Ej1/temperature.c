#include "contiki.h"
#include "dev/leds.h"
#include "temperature-sensor.h"
#include <stdio.h>

/*---------------------------------------------------------------------------*/
PROCESS(timer_process, "Proceso Temporizador (2s)");
PROCESS(sensor_process, "Proceso de Lectura");

static process_event_t event_data_ready;

AUTOSTART_PROCESSES(&timer_process, &sensor_process);
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(timer_process, ev, data)
{
  static struct etimer et;

  PROCESS_BEGIN();

  event_data_ready = process_alloc_event();

  //  2 segundos (como pide el enunciado)
  etimer_set(&et, 2 * CLOCK_SECOND);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    process_post(&sensor_process, event_data_ready, NULL);
    etimer_reset(&et);
  }

  PROCESS_END();
}

/*---------------------------------------------------------------------------*/

PROCESS_THREAD(sensor_process, ev, data)
{
  int temp_raw;

  PROCESS_BEGIN();

  SENSORS_ACTIVATE(temperature_sensor);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == event_data_ready);

    temp_raw = temperature_sensor.value(0);

    int temp_f_int = (temp_raw * 45 + 3200) / 100;
    int temp_f_dec = (temp_raw * 45 + 3200) % 100;
    printf("temp_F %d.%02d\n", temp_f_int, temp_f_dec);


    

   // leds_toggle(LEDS_RED);
  }

  PROCESS_END();
}
