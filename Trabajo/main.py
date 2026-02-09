#!/usr/bin/env python3

import os
import time
import serial
import sys
import logging

import paho.mqtt.client as mqtt
from prometheus_client import Gauge, start_http_server

MQTT_ADDRESS = os.getenv("MQTT_ADDRESS", "localhost")
MQTT_PORT = int(os.getenv("MQTT_PORT", "1883"))

SERIAL_PORT = "/dev/ttyACM0"
SERIAL_BAUDRATE = 115200

logging.basicConfig(level=logging.INFO)
LOG = logging.getLogger("mqtt-exporter")

# ---------- Prometheus metrics ----------
temp_c = Gauge("temperature_celsius", "Temperature in Celsius")
temp_f = Gauge("temperature_fahrenheit", "Temperature in Fahrenheit")
temp_c_avg = Gauge("temperature_celsius_avg", "Average temperature in Celsius")
temp_f_avg = Gauge("temperature_fahrenheit_avg", "Average temperature in Fahrenheit")
alarm_state = Gauge("alarm_state", "Alarm state (0=NO_ACTIVA, 1=ACTIVA)")

# ---------- Acumuladores para la media ----------
sum_c = 0.0
sum_f = 0.0
count = 0

def main():
    global sum_c, sum_f, count

    # MQTT client
    client = mqtt.Client()
    client.connect(MQTT_ADDRESS, MQTT_PORT, 60)
    client.loop_start()

    # Prometheus HTTP endpoint
    start_http_server(9000)
    LOG.info("Prometheus exporter running on port 9000")

    # Serial port
    try:
        ser = serial.Serial(
            port=SERIAL_PORT,
            baudrate=SERIAL_BAUDRATE,
            timeout=1
        )
    except Exception as e:
        LOG.error(f"Serial error: {e}")
        sys.exit(1)

    LOG.info("Reading data from serial port")

    while True:
        try:
            line = ser.readline().decode().strip()
            if not line:
                continue

            # Expected format:
            # ID;UNIT;TEMP;ALARM_ID;ALARM_STATE
            parts = line.split(";")
            if len(parts) != 5:
                continue

            node_id, unit, temp_str, alarm_id, alarm_str = parts
            temp = float(temp_str)

            # Alarm
            alarm = 1 if alarm_str == "ACTIVA" else 0
            alarm_state.set(alarm)
            client.publish(f"iot/{node_id}/alarm", alarm)

            # Temperature handling
            if unit == "C":
                temp_c.set(temp)
                client.publish(f"iot/{node_id}/temperature/celsius", temp)

                temp_f_val = (temp * 9.0 / 5.0) + 32.0
                temp_f.set(temp_f_val)

                sum_c += temp
                sum_f += temp_f_val
            else:
                temp_f.set(temp)
                client.publish(f"iot/{node_id}/temperature/fahrenheit", temp)

                temp_c_val = (temp - 32.0) * 5.0 / 9.0
                temp_c.set(temp_c_val)

                sum_f += temp
                sum_c += temp_c_val

            count += 1

            temp_c_avg.set(sum_c / count)
            temp_f_avg.set(sum_f / count)

        except Exception:
            continue


if __name__ == "__main__":
    main()
