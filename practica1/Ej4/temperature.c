#include "contiki.h"
#include "dev/leds.h"
#include "temperature-sensor.h"
#include <stdio.h>

/*---------------------------------------------------------------------------*/
// Definición de procesos y evento personalizado
PROCESS(timer_process, "Proceso Temporizador (3s)");
PROCESS(sensor_process, "Proceso de Lectura");

// Variable para el evento de lectura
static process_event_t event_data_ready;

// Inicio automático de ambos procesos
AUTOSTART_PROCESSES(&timer_process, &sensor_process);
/*---------------------------------------------------------------------------*/

// Proceso temporizador
PROCESS_THREAD(timer_process, ev, data)
{
  static struct etimer et;

  PROCESS_BEGIN();

 
  event_data_ready = process_alloc_event();
  
  // 3 seg
  etimer_set(&et, 3 * CLOCK_SECOND);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    // Notificar lectura
    process_post(&sensor_process, event_data_ready, NULL);

    etimer_reset(&et);
  }

  PROCESS_END();
}

/*---------------------------------------------------------------------------*/

// Proceso Lectura sensor
PROCESS_THREAD(sensor_process, ev, data)
{
  int temp_raw;

  PROCESS_BEGIN();

  // Inicializar el hardware del sensor
  SENSORS_ACTIVATE(temperature_sensor);

  while(1) {
    // Esperar hasta recibir el evento específico del timer_process
    PROCESS_WAIT_EVENT_UNTIL(ev == event_data_ready);

    // Ejecutar lectura
    temp_raw = temperature_sensor.value(0);
    
    // Cálculo de parte entera y decimal (pasos de 0.25 C)
    int entera = temp_raw / 4;
    int decimal = (temp_raw % 4) * 25;

    printf("Temperatura actual: %d.%02d C\r\n", entera, decimal);

    // Feedback visual con LED (OPcional)
    leds_toggle(LEDS_RED);
  }

  PROCESS_END();
}