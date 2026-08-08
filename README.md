# 🏠 IoT Smart Home — ESP32 & MAX7219 Running Text

<div align="center">

### Standalone Wi-Fi Smart Home Controller with Dynamic LED Matrix Display

![ESP32](https://img.shields.io/badge/Board-ESP32-323330?style=for-the-badge&logo=espressif&logoColor=white)
![Arduino](https://img.shields.io/badge/Platform-Arduino_IDE-00979D?style=for-the-badge&logo=arduino&logoColor=white)
![WiFi](https://img.shields.io/badge/Network-ESP32_Access_Point-00BFFF?style=for-the-badge&logo=wifi&logoColor=white)
![MAX7219](https://img.shields.io/badge/Display-MAX7219-7B61FF?style=for-the-badge)
![MIT App Inventor](https://img.shields.io/badge/Mobile-MIT_App_Inventor-A4C639?style=for-the-badge)

**A standalone ESP32-based smart home controller for wireless device control and real-time LED matrix messaging.**

</div>

---

# 📖 Overview

The **IoT Smart Home ESP32** is a standalone home automation prototype that allows users to control electrical devices and update a scrolling LED message through a local Wi-Fi network.

The ESP32 operates as a **Wi-Fi Access Point (AP)**, creating its own wireless network without requiring an external router or internet connection.

The system provides two main functions:

- 💡 **Electrical Device Control** through a 2-channel relay module.
- 📟 **Dynamic Running Text** through a 4-in-1 MAX7219 LED Matrix.

An Android control application built with **MIT App Inventor** communicates with the ESP32 through simple HTTP GET requests.

The system is designed around a lightweight, local network architecture, making it suitable for demonstrations, educational projects, and basic offline home automation.

---

# ✨ Key Features

### 📡 Standalone Wi-Fi Access Point

The ESP32 creates its own wireless network:

```text
SSID: SmartHome
IP:   192.168.4.1
