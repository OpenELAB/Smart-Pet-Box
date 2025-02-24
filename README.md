&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Welcome to the Pet Box Smart Project 🐾💡, The Pet Box project integrates IoT technology, multiple sensors, and automation control systems to create a comfortable, safe, and healthy living environment for your pets. By monitoring and adjusting environmental parameters such as temperature, humidity, CO2 concentration, and light intensity in real time, the Pet Box ensures that your pets always have the ideal living environment.

Whether it’s automatically controlling temperature and humidity, using infrared temperature sensors to prevent overheating, or remotely viewing the pet box’s activity via a camera, the smart pet box helps pet owners manage their pets' environment more easily. 🌿🐶🐱

## 🌟 Table of Contents 📜

- 💡 [Project Overview](#project-overview)
- ✨ [Features](#features)
- 📐 [Hardware Design and Circuit Connections](#hardware-design-and-circuit-connections)
- 🗂️ [BOM (Bill of Materials)](#bom-bill-of-materials)
- 🛠️ [Software Environment Setup](#software-environment-setup)
- 🎇 [How to Use](#how-to-use)
- 🌐 [IoT Platform and Data Upload](#iot-platform-and-data-upload)
- 📝 [Notes and Common Issues](#notes-and-common-issues)
- 🚀 [Future Outlook](#future-outlook)

---

## 💡 Project Overview

The **Pet Box** project uses various sensors and an automation control system to monitor and control the pet's living environment. Using **M5StickC Plus2** as the main controller, and combining temperature, humidity, CO2 sensors, light sensors, and infrared temperature sensors, the project ensures your pet’s environment is always comfortable.

All the data is uploaded to the cloud platform, and remote control is available via a mobile app, making it easier for pet owners to adjust their pets' living conditions, such as controlling humidifiers, heaters, and lights remotely.

---

## ✨ Features

- **Real-time Environment Monitoring**: Monitors temperature, humidity, CO2 concentration, light intensity, and more, providing instant feedback.
- **Automatic Temperature and Humidity Control**: Based on pre-set values, it controls devices like the humidifier and heater to maintain an ideal environment.
- **Overheat Protection**: Monitors the bottom temperature using infrared sensors to prevent overheating of the heating pad.
- **Remote Control and Data Viewing**: Allows pet owners to view live data and control devices like humidifiers, fans, and heating pads via a mobile app.
- **Camera Monitoring**: Enables pet owners to monitor their pets’ activities inside the box via the app.

---

## 📐 Hardware Design and Circuit Connections

### Main Hardware Components

- **M5StickC Plus2**: ESP32 microcontroller that handles reading sensor data and controlling devices.
- **SCD40 CO2 Sensor**: Measures temperature, humidity, and CO2 concentration in the pet box environment.
- **MLX90614 Infrared Temperature Sensor**: Monitors the temperature at the bottom of the box to prevent overheating.
- **PaHub 2.1**: I2C expansion module for connecting multiple sensor modules.
- **PbHub**: GPIO expansion module used to control relays, fans, heaters, and other devices.
- **Light Sensor**: Measures light intensity to adjust lighting in the pet box.
- **OV2640 Camera Module**: Captures images of the pet box environment and pet activity.
- **Relays**: Controls devices like the humidifier, heater, fan, and UVA light.

### Circuit Connections

- **M5StickC Plus2** connects to **PaHub 2.1** via I2C, powered by USB-C.
- **SCD40 CO2 Sensor** is connected to PaHub channel 0 for environmental data collection.
- **MLX90614 Infrared Temperature Sensor** is connected to PaHub channel 2 to monitor the bottom temperature.
- **PbHub** connects to relays for controlling devices:
  - Channel 0: Light Sensor.
  - Channel 2: Camera Module (for powering the camera).
  - Channels 3–5: Controls Humidifier, Heater, UVA Light, and optionally a Fan.

---

## 🗂️ BOM (Bill of Materials)

| Component           | Specification                | Quantity |
|---------------------|------------------------------|----------|
| **M5StickC Plus2**   | ESP32 microcontroller         | 1        |
| **SCD40 CO2 Sensor** | Temperature, humidity, CO2 sensor | 1    |
| **MLX90614 Sensor**  | Infrared temperature sensor   | 1        |
| **PaHub 2.1**        | I2C expansion module          | 1        |
| **PbHub**            | GPIO expansion module         | 1        |
| **Relay Modules**    | For controlling devices       | 3        |
| **Light Sensor**     | Measures light intensity      | 1        |
| **OV2640 Camera**    | Captures pet box environment images | 1   |

---

## 🛠️ Software Environment Setup

1. **Development Environment**: Use **VSCode** with the **PlatformIO** plugin to set up and develop your code.
2. **Library Installation**:
   - **M5Unified**: Simplifies controlling M5Stack devices.
   - **SensirionI2CScd4x**: Used to interface with the SCD40 CO2 sensor.
   - **Adafruit MLX90614 Library**: For operating the MLX90614 infrared sensor.
   - **PubSubClient**: For MQTT communication.
   - **ArduinoJson**: Handles and parses JSON data.
   - **M5Unit-HUB & M5Unit-PbHub**: For interacting with PaHub and PbHub modules.
3. **Import Project**: Open the project folder in VSCode and import the Arduino project to start working.

---

## 🎇 How to Use

### Assemble the Hardware:

1. Connect all the sensors and devices to the PaHub and PbHub modules according to the circuit connections above.
2. Power the M5StickC Plus2 and ensure everything is connected properly.

### Configure and Debug:

1. Open the mobile app and connect it to the Pet Box.
2. Check the real-time data (temperature, humidity, CO2, light) and adjust the settings as needed.
3. You can turn devices like the heater, humidifier, and fan on or off manually, or set automatic control.

### Automatic Control:

1. Set the target temperature and humidity.
2. The system will automatically adjust the devices to reach the desired settings.

### Remote Control:

1. Use the cloud platform and app to monitor and adjust the pet box environment remotely, even when you're not at home.

---

## 🌐 IoT Platform and Data Upload

1. **Platform Selection**: We use **Alibaba Cloud IoT** to upload data and manage devices remotely.
2. **Device Connection and Binding**: Follow the setup tutorial to bind your device with the cloud platform.
3. **Data Upload**: Sensor data is uploaded via MQTT protocol in JSON format.

```cpp
// Function to upload data
void mqttIntervalPost(float temperature, float humidity, int co2, int lightValueAnalog) {
    char param[256];
    sprintf(param, "{\"temperature\":%0.2f, \"humidity\":%0.2f, \"co2\":%d, \"lightValueAnalog\":%d}", temperature, humidity, co2, lightValueAnalog);
    client.publish(ALINK_TOPIC_PROP_POST, param);
}
```

---

##📝 Notes and Common Issues
- **Sensor Initialization**: The SCD40 CO2 sensor may take approximately 6 seconds to initialize and provide accurate readings.
- **I2C Bus Speed**: The MLX90614 sensor requires the I2C bus speed to be set to 100kHz to work correctly.
- **Device Conflicts**: Ensure that PaHub and PbHub modules have unique I2C addresses to prevent conflicts.
- **Cloud Data Reception**: Ensure a stable connection and check MQTT settings if data isn’t received on the cloud platform.

##🚀 Future Outlook
- **Adding More Sensors**: Integrating additional sensors like PM2.5 or gas sensors to further enhance the pet box’s monitoring capabilities.
- **Optimizing Control Algorithms**: Improving the temperature and humidity control algorithms to make the pet box more intelligent and adaptable to changing conditions.

