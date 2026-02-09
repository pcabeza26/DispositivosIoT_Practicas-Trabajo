# Trabajo final - Dispositivos IoT

## Estructura de directorios
- Cliente: formato_trama.h  Makefile  project-conf.h  udp-client.c
- Servidor: formato_trama.h  Makefile  project-conf.h  udp-server.c
- dashboard.json
- main.py


---

## Instrucciones de Compilación, Carga y Visualización

### Archivos
- Reemplazar main.py en "/p4/docker/mqtt_exporter"
### Compilación
- Conectar placa 1
- cd Servidor
- make TARGET=nrf52840 BOARD=dongle udp-server.dfu-upload
- Desconectar placa
- Conectar placa 2
- cd Cliente
- make TARGET=nrf52840 BOARD=dongle udp-client.dfu-upload
- Desconectar placa
### Visualización Ejecución UART
- Conectar placa 1
- picocom -b 115200 /dev/ttyACM0
- Conectar placa 2
- picocom -b 115200 /dev/ttyACM1
### Visualización Ejecución Grafana
- docker-compose down
- docker-compose build mqtt-exporter
- docker-compose up -d

Acceso a Grafana (http://localhost:3000) e importar dashboard.json





