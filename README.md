# IoT Temperature Monitoring System With WhatsApp alerts

## Overview

This project implements a **TTGO T-Display ESP32 module** with a **DHT11** sensor that reads room temperature and publishes the data to a **MQTT Broker (Mosquitto)** allocated in a **EC2** instance in **AWS**.

**Home Assistant** subscribes to the **MQTT** topic with the temperature data and displays it in a **Lovelace** dashboard. Furthermore, an automation is configured so that a **WhatsApp** alert is sent via the **CallMeBot API** when temperature is greater than 30°C.

## Hardware

- **TTGO T-Display v2 (ESP32 module)**
- **DHT11** temperature sensor

### Software

- **PlatformIO** (for ESP32 programming)
- **AWS EC2** (to host Mosquitto & Home Assistant)
- **Mosquitto** (MQTT broker)
- **Home Assistant** (in Docker)
- **CallMeBot API** (for WhatsApp messages)
