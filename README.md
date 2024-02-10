# Plant Communicator: Indoor Plant Care System

## Overview
The Plant Communicator is an innovative indoor plant care system designed to monitor and communicate with your plants through automated actions such as watering and light adjustment. It leverages an ESP32 module for internet connectivity, enabling remote monitoring and control via the Blynk platform. This project aims to ensure the well-being of plants by providing real-time data and notifications about their needs.

## Features
- **Real-time Plant Monitoring:** Utilizes moisture, temperature, and UV sensors to monitor plant conditions.
- **Automated Water Dispensing:** A DC water pump controls watering based on soil moisture levels.
- **Light Adjustment:** An LED strip provides necessary light, with manual override available.
- **Remote Control and Notifications:** Through Blynk, users can monitor plant health, control the system remotely, and receive updates.
- **Energy Efficient:** Potential for solar power integration for sustainability.

## Hardware Requirements
- ESP32 Module
- Moisture Sensor
- Temperature and Phototransistor Sensors
- Adafruit LTR390 UV Sensor
- DC Water Pump
- LED Strip
- Manual Override Switch
- Various cables and connectors

## Software Setup
- Install Arduino IDE and Blynk library.
- Configure Blynk with the provided `BLYNK_TEMPLATE_ID` and `BLYNK_AUTH_TOKEN`.

## Configuration
1. Connect your ESP32 to your computer.
2. Open the provided code in Arduino IDE.
3. Update `ssid` and `pass` with your WiFi credentials.
4. Flash the code to your ESP32.

## Usage
Once set up, the system automatically monitors and takes care of your plants. Use the Blynk app to monitor real-time data and control the system manually if needed.
