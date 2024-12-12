# Fire Detection and Water Spraying Robot

This project is an IoT-based fire detection and extinguishing system using the ESP8266 microcontroller. The system detects fire using three flame sensors and guides a robot to extinguish the fire by spraying water using a pump.

## Features

- **Fire Detection:** Detects fire from the left, right, or front using flame sensors.
- **Autonomous Movement:** Controls motors to guide the robot toward the fire.
- **Water Spraying:** Activates a water pump to extinguish the fire upon detection.
- **ThingSpeak Integration:** Sends real-time sensor data (left, front, and right) to ThingSpeak every 3 seconds.

---

## Hardware Requirements

- ESP8266 NodeMCU
- Flame Sensors (3x)
- L298N Motor Driver Module
- DC Motors (2x)
- DC Water Pump
- Power Supply (matching the pump and motor requirements)
- Connecting Wires

---

## Software Requirements

- Arduino IDE
- ThingSpeak Library

---

## Circuit Diagram

1. **Flame Sensors**
   - Connect `D0`, `D1`, `D2` to left, front, and right flame sensors respectively.

2. **Motor Driver (L298N)**
   - Enable Pins: `D7` (enA), `D8` (enB)
   - Input Pins: `D3`, `D4`, `D5`, `D6` (for motor control)

3. **Water Pump**
   - Connect pump to `IN1` of the L298N motor driver.

---

## Code Overview

### Functions

- **Flame Sensor Readings:** Reads the digital values from the flame sensors to detect fire.
- **Motor Control:** Moves the robot forward, left, or right based on the detected fire location.
- **Pump Control:** Activates the water pump when a fire is detected.
- **ThingSpeak Integration:** Sends sensor data to ThingSpeak every 3 seconds.
