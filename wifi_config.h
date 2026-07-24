#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

const char* ssid     = "harsh";
const char* password = "87654321";

// Change these
IPAddress local_IP(10, 50, 233, 100);      // ESP32 IP
IPAddress gateway(10, 50, 233, 1);         // Hotspot gateway (usually .1)
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);

#endif