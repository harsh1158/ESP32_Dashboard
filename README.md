# ESP32 Dashboard

A professional ESP32 Web Dashboard built using the Arduino Framework.

This project provides a responsive web interface for configuring and monitoring an ESP32 device over WiFi.

---

# Project Structure

```
ESP32_Dashboard/
│
├── ESP32_Dashboard.ino      # Main Arduino file
├── api.h                    # REST API Handlers
├── dashboard.h              # HTML Dashboard
├── storage.h                # Flash Storage Functions
├── device.h                 # Device Structure
├── security.h               # Single Client Security
├── wifi_config.h            # WiFi Configuration
└── README.md
```

---

# Hardware Required

- ESP32 Development Board
- USB Cable
- PC/Laptop
- WiFi Router

---

# Software Required

- Arduino IDE 2.x
- ESP32 Arduino Board Package
- Git
- GitHub Account

---

# Arduino Libraries

Install the following libraries:

```
WiFi
WebServer
Preferences
```

These libraries are included with the ESP32 Arduino Core.

---

# ESP32 Board Installation

1. Open Arduino IDE.

2. Go to

```
File
→ Preferences
```

3. Add the ESP32 Board Manager URL

```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

4. Open

```
Tools
→ Board
→ Boards Manager
```

5. Search

```
ESP32
```

6. Install

```
esp32 by Espressif Systems
```

---

# Clone Repository

Using HTTPS

```bash
git clone https://github.com/harsh1158/ESP32_Dashboard.git
```

or using SSH

```bash
git clone git@github.com:harsh1158/ESP32_Dashboard.git
```

---

# Open Project

Open Arduino IDE

Open

```
ESP32_Dashboard.ino
```

---

# Configure WiFi

Edit

```
wifi_config.h
```

Example

```cpp
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_PASSWORD";
```

---

# Upload Project

Select

```
Board :
ESP32 Dev Module
```

Select the correct COM Port

Click

```
Upload
```

---

# Serial Monitor

Baud Rate

```
115200
```

Example Output

```
ESP32 WiFi Module

WiFi Connected Successfully

HTTP Server Started Successfully

Open Browser

http://192.168.1.100
```

---

# Open Dashboard

Open your browser

```
http://192.168.1.100
```

---

# Dashboard Functions

Available Fields

- ID
- Present Date
- Expiry Year
- Cycle
- Pulse Cycle
- Total Pulse
- My Use Time

Buttons

- SUBMIT
- READ

---

# REST APIs

## Save Data

```
POST /save
```

Stores device configuration into ESP32 Flash.

---

## Read Data

```
POST /read
```

Reads device configuration from ESP32 Flash.

---

# Flash Storage

Uses

```
Preferences (NVS)
```

Stored Parameters

- Device ID
- Expiry Year
- Use Time
- Cycle
- Pulse Cycle
- Total Pulse

---

# Security

Current Features

- Single Client Access
- Client IP Verification
