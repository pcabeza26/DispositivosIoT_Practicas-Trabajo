#!/usr/bin/env python3

import os
import time
import serial
import sys
import logging

import paho.mqtt.client as mqtt
from prometheus_client import Gauge, Counter, start_http_server

MQTT_ADDRESS = os.getenv("MQTT_ADDRESS", "localhost")
MQTT_PORT = int(os.getenv("MQTT_PORT", "1883"))

logging.basicConfig(filename='/mqtt_exporter/log/register.log', level=logging.INFO)
LOG = logging.getLogger("mqtt-exporter")

msg_counter = Counter("mqtt_messages_total", "Total MQTT messages received")

metrics = {
    "temp_c": Gauge("temp_c", "Temperature in Celsius"),
    "temp_F": Gauge("temp_F", "Temperature in Fahrenheit"),
    "switch": Gauge("switch", "Switch state")
}

def on_connect(client, userdata, flags, rc, properties=None):
    for topic in metrics.keys():
        client.subscribe(topic)

def main():
    client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
    client.on_connect = on_connect

    while True:
        try:
            client.connect(MQTT_ADDRESS, MQTT_PORT, 60)
            break
        except Exception:
            time.sleep(5)

    client.loop_start()
    start_http_server(9000)

    try:
        ser = serial.Serial(
            port="/dev/ttyACM0",
            baudrate=115200,
            timeout=1
        )
    except Exception:
        sys.exit(1)

    while True:
        try:
            line = ser.readline().decode().strip()
            if not line:
                continue

            parts = line.split()
            if len(parts) != 2:
                continue

            topic, value = parts
            value = float(value)

            if topic in metrics:
                metrics[topic].set(value)
                msg_counter.inc()
                client.publish(topic, value)

        except Exception:
            continue

if __name__ == "__main__":
    main()
