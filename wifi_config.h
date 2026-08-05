#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

const char* ssid     = "Embedix";
const char* password = "Uni#rel@off123";

// Change these
IPAddress local_IP(192, 168, 1, 100);      // ESP32 IP
IPAddress gateway(192, 168, 1, 1);         // Hotspot gateway 
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);

#endif