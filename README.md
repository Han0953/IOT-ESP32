# 🏠 IoT Smart Home — ESP32 & MAX7219

<div align="center">

### Standalone Wi-Fi Home Automation with Real-Time LED Matrix Running Text

![ESP32](https://img.shields.io/badge/Board-ESP32-323330?style=for-the-badge&logo=espressif&logoColor=white)
![Arduino](https://img.shields.io/badge/Platform-Arduino_IDE-00979D?style=for-the-badge&logo=arduino&logoColor=white)
![WiFi](https://img.shields.io/badge/Network-WiFi_Access_Point-00BFFF?style=for-the-badge&logo=wifi&logoColor=white)
![MAX7219](https://img.shields.io/badge/Display-MAX7219-7B61FF?style=for-the-badge)
![Language](https://img.shields.io/badge/Language-C%2B%2B-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)

**An ESP32-based standalone smart home system capable of controlling electrical devices and displaying dynamic running text through a MAX7219 LED matrix.**

</div>

---

## 📖 Overview

The **IoT Smart Home ESP32** is a standalone home automation prototype designed to control electrical devices through a local Wi-Fi network while simultaneously displaying dynamic messages on a MAX7219 LED matrix.

The ESP32 operates as a **Wi-Fi Access Point (AP)**, allowing smartphones or other Wi-Fi-enabled devices to communicate directly with the ESP32 without requiring an external router or internet connection.

The system provides two primary functions:

- 💡 **Electrical Device Control** using a 2-channel relay module.
- 📟 **Dynamic Running Text** using a 4-module MAX7219 LED Matrix.

A lightweight HTTP server running directly on the ESP32 processes commands from a mobile application or web browser.

The system uses a **non-blocking architecture based on `millis()`**, allowing HTTP requests and LED animation to operate continuously without relying on long `delay()` calls.

---

## ✨ Key Features

### 📡 Standalone Wi-Fi Access Point

The ESP32 creates its own local wireless network:

```text
SSID     : SmartHome
Password : Open Network
IP       : 192.168.4.1
```

No external router or internet connection is required.

### 💡 Dual Relay Control

Two electrical devices can be controlled independently:

- 💡 Lamp
- 🌀 Fan

### 📟 Dynamic MAX7219 Running Text

The 4-module MAX7219 LED matrix displays a continuously scrolling message.

Default message:

```
SELAMAT DATANG DI SMART HOME
```

The message can be changed remotely through an HTTP request.

### ⚡ Non-Blocking Animation

The LED animation uses `millis()` instead of `delay()`, allowing the ESP32 to handle:

```
HTTP Server
     +
Relay Control
     +
LED Matrix Animation
```

simultaneously.

### 🌐 Built-In HTTP API

The ESP32 provides simple HTTP endpoints for:

- Lamp control
- Fan control
- Running text updates
- Server status

---

## 🏗️ System Architecture

```
                    📱 Smartphone
                         │
                         │ Wi-Fi
                         ▼
                ┌─────────────────┐
                │      ESP32      │
                │  Wi-Fi Access   │
                │      Point      │
                │                 │
                │  WebServer :80  │
                └────────┬────────┘
                         │
              ┌──────────┴──────────┐
              │                     │
              ▼                     ▼
       ┌──────────────┐      ┌──────────────┐
       │ 2-Channel    │      │   MAX7219    │
       │    Relay     │      │ LED Matrix   │
       └──────┬───────┘      └──────────────┘
              │
        ┌─────┴─────┐
        ▼           ▼
      💡 Lamp      🌀 Fan
```

---

## 🛠️ Hardware Components

| Component | Quantity | Function |
|---|---|---|
| ESP32 Development Board | 1 | Main controller & Wi-Fi Access Point |
| MAX7219 4-in-1 LED Matrix | 1 | Running text display |
| 2-Channel Relay Module | 1 | Lamp and fan control |
| External 5V Power Supply | 1 | Dedicated MAX7219 power source |
| Jumper Wires | Several | Electrical connections |

---

## 🔌 Pin Configuration

### 📟 MAX7219 LED Matrix

| MAX7219 Pin | Connection |
|---|---|
| VCC | External 5V |
| GND | External PSU GND + ESP32 GND |
| DIN | GPIO 23 |
| CS | GPIO 5 |
| CLK | GPIO 18 |

The display is configured as 4 horizontal modules:

```cpp
const int pinCS = 5;
const int numberOfHorizontalDisplays = 4;
const int numberOfVerticalDisplays = 0;
```

### 💡 Relay Module

| Relay Pin | ESP32 GPIO | Device |
|---|---|---|
| IN1 | GPIO 27 | Lamp |
| IN2 | GPIO 26 | Fan |
| VCC | VIN / 5V | Power |
| GND | GND | Ground |

---

## ⚠️ MAX7219 Power Requirements

The MAX7219 LED matrix should be powered using an external regulated 5V power supply rather than relying on the ESP32 5V rail.

```
              External 5V PSU
              ┌───────────────┐
              │               │
             +5V             GND
              │               │
              ▼               ▼
        MAX7219 VCC      MAX7219 GND
                              │
                              │
                       ESP32 GND
```

The external power supply and ESP32 must share a common ground.

Insufficient power can cause:

- Display flickering
- Display corruption
- ESP32 brownouts
- Random resets
- Unstable LED matrix behavior

---

## 📚 Required Libraries

Install the following libraries through Arduino IDE → Library Manager.

| Library | Purpose |
|---|---|
| WiFi.h | ESP32 Wi-Fi Access Point |
| WebServer.h | HTTP Web Server |
| SPI.h | SPI communication |
| Adafruit_GFX | Graphics and text rendering |
| Max72xxPanel | MAX7219 LED matrix control |

The following libraries are provided by the ESP32 Arduino core:

```
WiFi
WebServer
SPI
```

> **Note:** This project uses Adafruit_GFX and Max72xxPanel. It does not use MD_Parola or MD_MAX72XX.

---

## 📟 MAX7219 Configuration

The display uses four horizontally connected modules:

```cpp
const int pinCS = 5;
const int numberOfHorizontalDisplays = 4;
const int numberOfVerticalDisplays = 0;
```

Display intensity:

```cpp
matrix.setIntensity(7);
```

Display rotation:

```cpp
matrix.setRotation(4);
```

The rotation value may need to be adjusted depending on the physical orientation of the MAX7219 modules.

---

## 📝 Running Text System

The default running text is:

```cpp
String runningText = "SELAMAT DATANG DI SMART HOME";
```

The initial position is set to the right side of the display:

```cpp
scroll_x = matrix.width();
```

The text continuously moves toward the left:

```
             MAX7219 DISPLAY
        ┌─────────────────────┐
        │ SELAMAT DATANG ...  │
        └─────────────────────┘
                    ◄────────
                    Scrolling
```

Once the entire message leaves the display, the position is reset to the right side.

---

## ⚡ Non-Blocking Animation

The project uses `millis()` instead of `delay()`.

```cpp
unsigned long lastScrollTime = 0;
const int SCROLL_SPEED = 50;
```

The animation is updated only when the configured interval has elapsed:

```cpp
if (millis() - lastScrollTime > SCROLL_SPEED) {
    lastScrollTime = millis();

    matrix.fillScreen(LOW);
    matrix.setCursor(scroll_x, 0);
    matrix.print(runningText);
    matrix.write();

    scroll_x--;
}
```

This allows the HTTP server to remain responsive while the LED matrix is continuously animated.

---

## 🌐 HTTP API

The ESP32 WebServer provides the following endpoints:

| Endpoint | Function |
|---|---|
| `/` | Check server status |
| `/lampu_on` | Turn lamp ON |
| `/lampu_off` | Turn lamp OFF |
| `/kipas_on` | Turn fan ON |
| `/kipas_off` | Turn fan OFF |
| `/ubah_text?pesan=...` | Change running text |

### 📡 API Examples

**Server Status**
```
http://192.168.4.1/
```
Response:
```
Server ESP32 Aktif!
```

**Turn Lamp ON**
```
http://192.168.4.1/lampu_on
```
Response:
```
Lampu ON
```

**Turn Lamp OFF**
```
http://192.168.4.1/lampu_off
```
Response:
```
Lampu OFF
```

**Turn Fan ON**
```
http://192.168.4.1/kipas_on
```
Response:
```
Kipas ON
```

**Turn Fan OFF**
```
http://192.168.4.1/kipas_off
```
Response:
```
Kipas OFF
```

**Change Running Text**
```
http://192.168.4.1/ubah_text?pesan=SMART%20HOME
```

The ESP32 converts the received text to uppercase before displaying it.

Example:
```
Input:
Smart Home

Output:
SMART HOME
```

If the `pesan` parameter is missing, the server returns HTTP status 400:
```
Error: Argumen 'pesan' tidak ditemukan
```

---

## 📱 Mobile Control

The system can be controlled using an Android application developed with MIT App Inventor.

The application sends HTTP GET requests over the local Wi-Fi network.

```
┌───────────────────────┐
│    Android App        │
│                       │
│  💡 Lamp ON           │
│  💡 Lamp OFF          │
│  🌀 Fan ON            │
│  🌀 Fan OFF           │
│  📟 Change Text       │
└──────────┬────────────┘
           │
           │ HTTP GET
           ▼
┌───────────────────────┐
│       ESP32           │
│   192.168.4.1         │
└───────────────────────┘
```

A standard web browser can also be used to test the API without the Android application.

---

## 🔄 System Workflow

```
                    START
                      │
                      ▼
             Initialize ESP32
                      │
                      ▼
             Configure Relays
                      │
                      ▼
            Initialize MAX7219
                      │
                      ▼
          Create Wi-Fi Access Point
                  "SmartHome"
                      │
                      ▼
             Start Web Server
                      │
                      ▼
                  Main Loop
                      │
             ┌────────┴────────┐
             ▼                 ▼
       Handle Client      Update Display
             │                 │
             ▼                 ▼
       Process HTTP        Check millis()
       Command                 │
             │                 ▼
       ┌─────┼─────┐       Move Text
       ▼     ▼     ▼           │
     Lamp   Fan   Text          │
       │     │     │            │
       └─────┴─────┴────────────┘
                      │
                      ▼
                    Repeat
```

---

## 🧠 Main Control Logic

The main loop continuously performs two tasks:

```cpp
void loop() {

    // Handle HTTP requests
    server.handleClient();

    // Update LED animation
    if (millis() - lastScrollTime > SCROLL_SPEED) {

        lastScrollTime = millis();

        matrix.fillScreen(LOW);
        matrix.setCursor(scroll_x, 0);
        matrix.print(runningText);
        matrix.write();

        scroll_x--;

        if (scroll_x <= -(runningText.length() * 6)) {
            scroll_x = matrix.width();
        }
    }
}
```

The HTTP server and display animation therefore operate within the same non-blocking loop.

---

## 🚀 Installation & Setup

### 1. Clone the Repository

```bash
git clone https://github.com/Han0953/ESP32-Smart-Home.git
cd ESP32-Smart-Home
```

### 2. Open the Firmware

Open the `.ino` file using Arduino IDE.

Example:
```
SmartHome.ino
```

### 3. Install Required Libraries

Open:
```
Arduino IDE
→ Library Manager
```

Install:
```
Adafruit GFX Library
Max72xxPanel
```

Make sure the ESP32 board package is also installed.

### 4. Configure the Board

| Setting | Value |
|---|---|
| Board | ESP32 Dev Module |
| CPU Frequency | 240 MHz |
| Flash Frequency | 80 MHz |
| Partition Scheme | Default |
| Port | ESP32 COM Port |

### 5. Upload the Firmware

Connect the ESP32 through USB and upload the firmware.

Open the Serial Monitor at:
```
115200 baud
```

Expected output:
```
Membuat Access Point: SmartHome
Alamat IP AP: 192.168.4.1
HTTP server dimulai
```

---

## 📱 Connecting to the System

After uploading:

1. Open Wi-Fi settings on your smartphone.
2. Connect to:
   ```
   SmartHome
   ```
3. Open the Android application or a browser.
4. Access:
   ```
   http://192.168.4.1
   ```

The system is now ready to receive commands.

---

## 📂 Repository Structure

```
📦 ESP32-Smart-Home
│
├── 📄 SmartHome.ino
│   └── Main ESP32 Firmware
│
├── 📁 hardware
│   ├── 🖼 wiring_diagram.png
│   ├── 🖼 circuit_schematic.png
│   └── 🖼 block_diagram.png
│
├── 📁 assets
│   ├── 🖼 prototype.jpg
│   ├── 🖼 max7219_display.jpg
│   ├── 🖼 android_controller.jpg
│   └── 🎞 demo.gif
│
├── 📄 README.md
└── 📄 LICENSE
```

---

## 📸 Project Preview

| Smart Home Prototype | MAX7219 Display |
|---|---|
| *Add Image* | *Add Image* |

| Android Controller | Relay Control |
|---|---|
| *Add Image* | *Add Image* |

---

## 🧪 Testing Checklist

| Test | Expected Result |
|---|---|
| ESP32 powered ON | Access Point starts |
| Connect to SmartHome | Smartphone connects |
| Open 192.168.4.1 | Server responds |
| `/lampu_on` | Lamp relay activates |
| `/lampu_off` | Lamp relay deactivates |
| `/kipas_on` | Fan relay activates |
| `/kipas_off` | Fan relay deactivates |
| `/ubah_text?pesan=...` | Running text changes |
| Leave system running | LED animation continues |
| Send HTTP commands during animation | Server remains responsive |

---

## ⚠️ Safety & Power Notes

### MAX7219 Power

Use a dedicated regulated 5V power supply for the LED matrix.

Always connect:
```
External PSU GND ───── ESP32 GND
```

### Relay Safety

- If the relay is connected to AC mains equipment, proper electrical isolation, insulation, enclosure, and appropriately rated components are required.
- For educational demonstrations, low-voltage DC loads are strongly recommended.
- Never work with exposed mains wiring while the system is powered.

---

## 🔧 Troubleshooting

### MAX7219 Does Not Display Correctly

Check:
```
✔ VCC → External 5V
✔ GND → Common Ground
✔ DIN → GPIO 23
✔ CS  → GPIO 5
✔ CLK → GPIO 18
✔ Four modules connected correctly
```

Also verify:
```cpp
matrix.setRotation(4);
```

Adjust the rotation value if the physical orientation of the display is different.

### ESP32 Randomly Resets

Possible causes:
- Insufficient MAX7219 power
- Unstable 5V supply
- Missing common ground
- Relay switching noise
- Excessive current draw

### Running Text Is Reversed

Check the display rotation:
```cpp
matrix.setRotation(4);
```

Try another rotation value depending on the orientation of your MAX7219 module.

### HTTP Commands Do Not Work

Verify:
```
✔ Smartphone is connected to SmartHome
✔ ESP32 IP is 192.168.4.1
✔ HTTP server has started
✔ Endpoint spelling is correct
```

Example:
```
http://192.168.4.1/lampu_on
```

---

## 🚀 Future Improvements

- 📱 Improved Android Control Dashboard
- 🔐 Password-Protected Wi-Fi
- 🔑 API Authentication
- 📡 MQTT Communication
- 🌐 Internet Connectivity
- ☁️ Cloud Monitoring
- 🌡 Temperature & Humidity Monitoring
- 💡 Automatic Lighting
- ⏰ Scheduled Device Control
- 📊 Sensor Data Logging
- 🏠 Multi-Room Control
- 🧠 AI-Assisted Automation
- 🔔 Push Notifications
- 📈 Web-Based Monitoring Dashboard

---

## 📜 License

This project was developed for educational and experimental purposes.

Released under the MIT License.

You are free to use, modify, and improve this project with proper attribution.

<div align="center">

### 🏠 Connect. Control. Automate.

**Designed & Developed by Rehan**

*ESP32 • IoT • Embedded Systems • Home Automation*

⭐ If you found this project useful, consider giving it a Star!

</div>
