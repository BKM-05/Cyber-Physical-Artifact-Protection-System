# Cyber-Physical Artifact Protection System

An IoT-based security system for protecting valuable artifacts using multiple sensors, RFID authentication, ESP32, ESP32-CAM, and Blynk IoT monitoring.

## Overview

The system is designed to monitor an artifact and detect unauthorized access or abnormal physical activity.

An ESP32 DevKit V1 acts as the main controller and monitors a PIR motion sensor, vibration sensor, and ultrasonic sensor. An MFRC522 RFID reader is used for access authentication. When unauthorized activity is detected, a buzzer alarm is activated and the event is displayed on the Blynk IoT dashboard.

An ESP32-CAM is used to provide live video streaming for visual monitoring of the protected area.

## Features

- Motion detection using PIR sensor
- Vibration/tampering detection
- Distance monitoring using ultrasonic sensor
- RFID-based access authentication
- Buzzer alarm for unauthorized activity
- Blynk IoT dashboard for real-time monitoring
- ESP32-CAM live video streaming
- Wi-Fi connectivity
- Temporary sensor disabling after authorized RFID access

## Hardware Used

- ESP32 DevKit V1
- ESP32-CAM AI-Thinker
- HC-SR501 PIR motion sensor
- SW-420 vibration sensor
- HC-SR04 ultrasonic sensor
- MFRC522 RFID reader
- Buzzer
- RFID card
- Connecting wires and prototype board

## Software and Technologies

- Arduino IDE
- Embedded C/C++
- ESP32
- Blynk IoT
- Wi-Fi
- SPI
- RFID
- Sensor interfacing

## System Working

1. The ESP32 initializes the connected sensors, RFID reader, buzzer, and Blynk connection.
2. The RFID reader checks for an access card.
3. An authorized RFID card temporarily disables sensor monitoring for 10 seconds.
4. An unauthorized RFID card triggers the alarm.
5. When normal monitoring is active, the PIR, vibration, and ultrasonic sensors continuously monitor the artifact.
6. If the ultrasonic sensor detects an object within the configured distance threshold or vibration is detected, the alarm is triggered.
7. The buzzer remains active for 5 seconds.
8. Sensor readings and alarm status are displayed on the Blynk IoT dashboard.
9. The ESP32-CAM provides live video streaming through its web server.

## Project Images

### Hardware Prototype

<img src="https://raw.githubusercontent.com/BKM-05/Cyber-Physical-Artifact-Protection-System/main/Prototype.jpg" alt="Hardware Prototype" width="700">

### Blynk IoT Dashboard - Sensor Monitoring

<img src="https://raw.githubusercontent.com/BKM-05/Cyber-Physical-Artifact-Protection-System/main/Blynk%20Dashboard%201.jpg" alt="Blynk Sensor Dashboard" width="700">

### Blynk IoT Dashboard - Camera and Monitoring

<img src="https://raw.githubusercontent.com/BKM-05/Cyber-Physical-Artifact-Protection-System/main/Blynk%20Dashboard%202.jpg" alt="Blynk Camera Dashboard" width="700">

## Key Concepts Demonstrated

- Microcontroller programming
- Sensor interfacing
- RFID authentication
- Embedded systems
- IoT monitoring
- Wi-Fi communication
- Real-time sensor monitoring
- Camera-based visual monitoring
- Blynk IoT dashboard integration

## Security Note

Wi-Fi credentials and Blynk authentication tokens are not included in this repository. Replace the placeholder values in the source code with your own credentials when deploying the system.

## Team Project

Developed as an academic mini project in Electronics and Computer Engineering.
