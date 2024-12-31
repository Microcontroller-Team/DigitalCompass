# Digital Compass with ESP32 and MPU9250

## Overview

This project implements a digital compass using an ESP32 microcontroller, an MPU9250 sensor, and a 1.3-inch TFT-LCD display. The compass utilizes magnetometer data to calculate headings, supports calibration, and provides a real-time visual representation on the TFT display.

---

## Features

- **Real-Time Compass Display:** Continuously calculates and displays the current heading.
- **Sensor Calibration:** Allows users to calibrate the magnetometer for improved accuracy.
- **Persistent Storage:** Stores calibration data in the ESP32's flash memory to retain accuracy across reboots.
- **Graphical Interface:** Displays the compass direction with an arrow on a circular graphical interface.

---

## Hardware Requirements

- **ESP32-WROOM-32E**
- **MPU9250 Sensor**
- **1.3-inch HD IPS TFT-LCD (ST7789)**
- Push Button (for calibration)
- Jumper wires and breadboard

---

## Software Requirements

Ensure the following libraries are installed in your Arduino IDE:

- [MPU9250_asukiaaa.h](https://github.com/asukiaaa/MPU9250_asukiaaa)
- [Preferences.h](https://github.com/espressif/arduino-esp32/tree/master/libraries/Preferences)
- [Adafruit_GFX.h](https://github.com/adafruit/Adafruit-GFX-Library)
- [Adafruit_ST7789.h](https://github.com/adafruit/Adafruit-ST7789-Library)

---

## Wiring Diagram

| **ESP32 Pin** | **Component** | **Description**      |
| ------------- | ------------- | -------------------- |
| GPIO 21       | MPU9250 SDA   | I²C Data Line        |
| GPIO 22       | MPU9250 SCL   | I²C Clock Line       |
| GPIO 5        | TFT CS        | SPI Chip Select      |
| GPIO 15       | TFT DC        | SPI Data/Command Pin |
| GPIO 4        | TFT Reset     | SPI Reset Pin        |
| GPIO 18       | TFT SCLK      | SPI Clock            |
| GPIO 23       | TFT MOSI      | SPI Data Line        |
| GPIO 19       | Button        | Calibration Button   |

---

## How to Use

### 1. **Setup**

1. Connect the hardware as per the wiring diagram.
2. Install the required libraries in the Arduino IDE.
3. Upload the provided code to your ESP32.

### 2. **Calibration**

- Press and hold the calibration button for **5 seconds** to enter calibration mode.
- Move the MPU9250 sensor in a figure-eight motion for **30 seconds** to record magnetometer data.
- Wait for the "Calibration done!" message on the display.

### 3. **Compass Mode**

- After calibration, the compass heading is displayed in degrees on the TFT screen.
- The heading is visualized as an arrow pointing to the direction on a circular interface.

---

## Code Explanation

The code is divided into key sections:

- **Initialization:**
  - Initializes the MPU9250, ESP32, and TFT display.
  - Loads previously saved calibration data from flash memory.

- **Calibration:**
  - Starts when the button is held for 5 seconds.
  - Collects and calculates minimum and maximum magnetometer values.
  - Saves calibration data to flash memory for future use.

- **Heading Calculation:**
  - Normalizes magnetometer readings based on calibration data.
  - Calculates the heading using the `atan2` function.
  - Displays the heading both numerically and graphically.

---

## Known Issues

- Ensure the MPU9250 sensor is placed away from metal objects and magnetic interference during calibration and operation.
- If the heading does not update, verify wiring and re-calibrate the sensor.

---

## License

This project is licensed under the [MIT License](https://opensource.org/licenses/MIT).

---

## Acknowledgments

- [Adafruit Libraries](https://github.com/adafruit)
- [MPU9250_asukiaaa Library](https://github.com/asukiaaa/MPU9250_asukiaaa)

