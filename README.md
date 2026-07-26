# ESP32 Dashboard

An ESP32-based Web Dashboard developed using the Arduino Framework.

The project provides a browser-based interface to configure probe data, store configuration in ESP32 Flash Memory, and retrieve the saved configuration whenever required.

---

# Features

- Login Authentication
- Change Username & Password
- Automatic Logout after Password Change
- Single Client Access (Only One Client Allowed)
- Probe Selection
- Save Configuration to ESP32 Flash Memory
- Read Configuration from Flash Memory
- Responsive Web Dashboard
- REST API Communication
- Serial Debug Logs

---

# Hardware Required

- ESP32 Development Board
- USB Cable
- PC / Laptop

---

# Software Required

## 1. Arduino IDE

Download and install the latest Arduino IDE.

https://www.arduino.cc/en/software

---

## 2. ESP32 Board Package

Open Arduino IDE

Go to

Tools → Board → Boards Manager

Search

ESP32

Install

ESP32 by Espressif Systems

---

# Required Libraries

No external libraries are required.

The project only uses the libraries included with the ESP32 Arduino Core.

```
WiFi.h
WebServer.h
Preferences.h
```

---

# Project Files

```
ESP32_Dashboard
│
├── ESP32_Dashboard.ino      Main Program
├── api.h                    HTTP API Handlers
├── dashboard.h              Dashboard HTML/CSS/JavaScript
├── login.h                  Login Page HTML/CSS/JavaScript
├── device.h                 Device Structures
├── probe_database.h         Probe Database
├── security.h               Login & Client Security
├── storage.h                Flash Storage Functions
├── wifi_config.h            WiFi Configuration
└── README.md
```

---

# How to Run the Project

## Step 1

Clone the repository

```
git clone https://github.com/harsh1158/ESP32_Dashboard.git
```

or download the ZIP file and extract it.

---

## Step 2

Open

```
ESP32_Dashboard.ino
```

using Arduino IDE.

---

## Step 3

Open

```
wifi_config.h
```

Modify the WiFi credentials.

Example

```cpp
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
```

If Static IP is required, modify

```cpp
IPAddress local_IP(...);
IPAddress gateway(...);
IPAddress subnet(...);
IPAddress primaryDNS(...);
IPAddress secondaryDNS(...);
```

---

## Step 4

Connect the ESP32 using USB.

---

## Step 5

Select the correct board

```
Tools

↓

Board

↓

ESP32 Dev Module
```

(or the appropriate ESP32 board)

---

## Step 6

Select the correct COM Port

```
Tools

↓

Port

↓

Select ESP32 Port
```

---

## Step 7

Click

```
Upload
```

---

## Step 8

Open

```
Serial Monitor
```

Select the baud rate used in the project (for example, **115200**).

---

## Step 9

After successful WiFi connection, the ESP32 prints its IP address.

Example

```
Connected Successfully

IP Address :

192.168.1.100
```

Open a browser and enter

```
http://192.168.1.100
```

(Replace with the IP shown on the Serial Monitor if different.)

---

# Login

The login page appears first.

Enter the configured username and password.

After successful login, the Dashboard opens.

---

# Dashboard

The dashboard allows the user to

- Select Probe
- View Probe Information
- Change Expiry Year
- Change Use Time
- Save Configuration
- Read Configuration
- Change Login Credentials
- Logout

---

# Flash Memory

The following fields are permanently stored in ESP32 Flash Memory.

- Expiry Year
- Total Cycle
- Total Pulse
- Use Time

The saved values remain available even after restarting or powering off the ESP32.

---

# Browser Support

- Google Chrome
- Microsoft Edge
- Mozilla Firefox

---

# Tested On

- ESP32 Dev Module
- Arduino IDE 2.x
- ESP32 Arduino Core
- Ubuntu Linux
