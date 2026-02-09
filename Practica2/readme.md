# Practica 2 - Dispositivos IoT

## Estructura de directorios
- Ej1: ejercicio1.c
- Ej2: border-router.c  Ej2_sim.csc  Makefile  project-conf.h  udp-client.c
- Ej3: P2_Ej3_sim.csc  udp-client.c  udp-server.c


---

## Instrucciones de Compilación, Carga y Visualización

### Ejercicio 1
cd Ej1
make TARGET=z1 ejercicio1.mspsim
### Ejercicio 2
cd Ej2
Reemplazar archivo: border-router.c en "/home/vmu/contiki-ng/examples/rpl-border-router"
Reemplazar archivo: Makefile en en "/home/vmu/contiki-ng/examples/rpl-border-router"
Reemplazar archivo: project-conf.h en  "/home/vmu/contiki-ng/examples/rpl-border-router"
Reemplazar archivo: udp-client.c en "/home/vmu/contiki-ng/examples/rpl-udp"
Ejecutar Simulación: Ej2_sim.csc
### Ejercicio 3
cd Ej3
Reemplazar archivo: udp-client.c en "/home/vmu/contiki-ng/examples/rpl-udp"
Reemplazar archivo: udp-server.c en "/home/vmu/contiki-ng/examples/rpl-udp"
Ejecutar Simulación: P2_Ej3_sim.csc


