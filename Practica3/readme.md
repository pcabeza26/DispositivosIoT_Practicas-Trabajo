# Practica 3 - Dispositivos IoT

## Estructura de directorios
- Ej1: project-conf.h  udp-client.c  udp-server.c
- Ej2: P3_Ej2.csc  rpl-udp
- Ej3: P3_Ej3.csc  RedA  RedB


---

## Instrucciones de Compilación, Carga y Visualización

### Ejercicio 1
cd Ej1
Reemplazar archivo: udp-client.c en "/home/vmu/contiki-ng/examples/rpl-udp" dentro del PC del compañero
Reemplazar archivo: udp-client.c en "/home/vmu/contiki-ng/examples/rpl-udp" dentro de mi PC
Reemplazar archivo: project-conf.h en "/home/vmu/contiki-ng/examples/rpl-udp" dentro de ambos PCs
Ejecutar en el cliente: 
make TARGET=nrf52840 BOARD=dongle udp-client.dfu-upload
picocom -b 115200 /dev/ttyACM0
Ejecutar en el servidor: 
make TARGET=nrf52840 BOARD=dongle udp-server.dfu-upload
picocom -b 115200 /dev/ttyACM0
### Ejercicio 2
cd Ej2
Reemplazar directorio: rpl-udp en "/home/vmu/contiki-ng/examples/"
Ejecutar Simulación:  P3_Ej2.csc
### Ejercicio 3
cd Ej3
cd RedA
make udp-client TARGET=z1
make udp-server TARGET=sky
cd RedB
make udp-client TARGET=z1
make udp-server TARGET=sky
Ejecutar Simulación: P3_Ej3.csc



