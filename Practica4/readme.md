# Practica 4 - Dispositivos IoT

## Estructura de directorios
- Ej1: build  main.py  Makefile  mqtt_temp.log  temperature.c  temp_uart.log
- Ej2: build main.py  Makefile  switch.log  temp_c.log  temperature2.c  temp_F.log




---

## Instrucciones de Compilación, Carga y Visualización
### Ejercicio 1
Se incuyen los logs de la salida de la uart "temp_uart.log" y el de mqtt "mqtt_temp.log"
- Reemplazar main.py en "/contiki-ng/p4/docker/mqtt_exporter"
- - cd Ej1
- make TARGET=nrf52840 BOARD=dongle temperature.dfu-upload
- cd ~/contiki-ng/p4/docker/mqtt_exporter
- docker-compose down
- docker-compose build mqtt-exporter
- docker-compose up -d

### Ejercicio 2
Se incluyen los logs MQTT de los tres topics
- Reemplazar main.py en "/contiki-ng/p4/docker/mqtt_exporter"
- - cd Ej2
- make TARGET=nrf52840 BOARD=dongle temperature2.dfu-upload
- cd ~/contiki-ng/p4/docker/mqtt_exporter
- docker-compose down
- docker-compose build mqtt-exporter
- docker-compose up -d