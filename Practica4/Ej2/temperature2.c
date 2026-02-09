#include "contiki.h"
#include "dev/leds.h"
#include "temperature-sensor.h"
#include "dev/button-hal.h"
#include <stdio.h>

/*---------------------------------------------------------------------------*/
PROCESS(timer_process, "Temporizador 2s");
PROCESS(sensor_process, "Lectura temperatura");
PROCESS(button_process, "Lectura pulsador");

static process_event_t event_sample;

AUTOSTART_PROCESSES(&timer_process, &sensor_process, &button_process);
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(timer_process, ev, data)
{
  static struct etimer et;

  PROCESS_BEGIN();

  event_sample = process_alloc_event();
  etimer_set(&et, 2 * CLOCK_SECOND);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    process_post(&sensor_process, event_sample, NULL);
    etimer_reset(&et);
  }

  PROCESS_END();
}

/*---------------------------------------------------------------------------*/

PROCESS_THREAD(sensor_process, ev, data)
{
  int raw;
  int c_ent, c_dec;
  int f_ent, f_dec;
  int f_scaled;

  PROCESS_BEGIN();

  SENSORS_ACTIVATE(temperature_sensor);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == event_sample);

    raw = temperature_sensor.value(0);

    c_ent = raw / 4;
    c_dec = (raw % 4) * 25;

    f_scaled = (raw * 45) + 3200;
    f_ent = f_scaled / 100;
    f_dec = f_scaled % 100;

    printf("temp_c %d.%02d\n", c_ent, c_dec);
    printf("temp_F %d.%02d\n", f_ent, f_dec);

    leds_toggle(LEDS_RED);
  }

  PROCESS_END();
}

/*---------------------------------------------------------------------------*/

PROCESS_THREAD(button_process, ev, data)
{
  static uint8_t state = 0;

  PROCESS_BEGIN();

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == button_hal_press_event);

    state = !state;
    printf("switch %d\n", state);
  }

  PROCESS_END();
}
