/*
 * Copyright (c) 2006, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 * A very simple Contiki application showing how Contiki programs look
 * \author
 * Adam Dunkels <adam@sics.se>
 */

#include "contiki.h"

#include <stdio.h> /* For printf() */

/*---------------------------------------------------------------------------*/
// Declaración de procesos
PROCESS(hello_world_process, "Hello world process");
PROCESS(periodic_process, "Proceso Periódico");

// Variables globales/estáticas
static int counter = 0;
static process_event_t PERIODIC_EVENT;

// Inicio de la aplicación
AUTOSTART_PROCESSES(&hello_world_process, &periodic_process);
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(hello_world_process, ev, data)
{
  

  PROCESS_BEGIN();
  counter = 0;

  while(1) {
    
    PROCESS_WAIT_EVENT();

    if(ev == PERIODIC_EVENT) {
      if (counter > 20) {
        counter = 0; 
      }
      
      printf("Hello World! (number %d)\r\n", counter);
      counter++;

    }

  }

  PROCESS_END();
}

/*---------------------------------------------------------------------------*/
// Nuevo proceso para generar eventos cada 5 segundos
PROCESS_THREAD(periodic_process, ev, data)
{
  static struct etimer et;
  
  PROCESS_BEGIN();

  // Inicializar el TIPO de evento
  PERIODIC_EVENT = process_alloc_event();
  
  // Establecer el temporizador para 5 segundos
  etimer_set(&et, 5 * CLOCK_SECOND);

  while(1) {
    // Esperar hasta que expire el temporizador
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    // Enviar el evento al proceso
    process_post(&hello_world_process, PERIODIC_EVENT, NULL);

    // Reiniciar el temporizador para la próxima ejecución
    etimer_reset(&et);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/