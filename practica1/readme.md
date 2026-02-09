# Practica 1 - Dispositivos IoT

## Estructura de directorios
- Ej2: ejercicio2.c
- Ej3: blink.c
- Ej4: temperature.c

---

## Instrucciones de Compilación, Carga y Visualización

### Ejercicio 2
cd Ej2
make TARGET=nrf52840 BOARD=dongle ejercicio2.dfu-upload
picocom -b 115200 /dev/ttyACM0

### Ejercicio 3
cd Ej3
make TARGET=nrf52840 BOARD=dongle blink.dfu-upload
picocom -b 115200 /dev/ttyACM0

### Ejercicio 4
cd Ej4
make TARGET=nrf52840 BOARD=dongle temperature.dfu-upload
picocom -b 115200 /dev/ttyACM0


