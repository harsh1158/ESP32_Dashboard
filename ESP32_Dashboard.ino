#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>

#include "wifi_config.h"
#include "dashboard.h"
#include "login.h"
#include "device.h"
#include "storage.h"
#include "security.h"
#include "AT21CS01.h"
#include "crc.h"

WebServer server(80);
Preferences prefs;
Device device;
AT21CS01 eeprom(18);
bool clientLocked = false;

IPAddress activeClient;

// AT21CS01 physical connection status
bool probeConnected = false;
unsigned long lastProbeCheck = 0;

#include "api.h"

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("AT21CS01 POWER-UP DELAY COMPLETE");

    eeprom.begin(18);

    Serial.println();
    Serial.println("==============================");
    Serial.println("AT21CS01 INITIALIZATION");
    Serial.println("==============================");

    prefs.begin("dashboard", false);
    if(!prefs.isKey("username"))
    {
        prefs.putString("username", "admin");
    }
    if(!prefs.isKey("password"))
    {
        prefs.putString("password", "1234");
    }
    initializeDatabase();
    Serial.println("After initializeDatabase()");
    prefs.begin("dashboard", false);
    Serial.println("Preferences Ready");
    Serial.println();
    Serial.println("==============================");
    Serial.println("ESP32 WiFi Module");
    Serial.println("==============================");

    WiFi.mode(WIFI_STA);
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
    {
        Serial.println("Static IP Configuration Failed!");
    }
    else
    {
        Serial.println("Static IP Configured Successfully");
    }

    Serial.println("Connecting to WiFi...");

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("------------------------------");
    Serial.println("WiFi Connected Successfully");
    Serial.print("IP Address : ");
    Serial.println(WiFi.localIP());

    Serial.print("Signal RSSI : ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");

    Serial.print("MAC Address : ");
    Serial.println(WiFi.macAddress());

    Serial.println("------------------------------");

    /*****************************************************
    * Start HTTP Server
    *****************************************************/

    server.on("/", handleRoot);
    server.on("/login", HTTP_POST, handleLogin);
    server.on("/changeLogin", HTTP_POST, handleChangeLogin);
    server.on("/logout", HTTP_GET, handleLogout);
    server.on("/save", HTTP_POST, handleSave);
    server.on("/read", HTTP_GET, handleRead);
    server.on("/probeStatus", HTTP_GET, handleProbeStatus);
    server.begin();

    Serial.println();
    Serial.println("======================================");
    Serial.println("HTTP Server Started Successfully");
    Serial.print("Open Browser : http://");
    Serial.println(WiFi.localIP());
    Serial.println("======================================");
}

void loop()
{
    server.handleClient();
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi Lost... Reconnecting");

        WiFi.disconnect();
        WiFi.begin(ssid, password);

        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(".");
        }

        Serial.println();
        Serial.println("Reconnected");
    }

    if (millis() - lastProbeCheck >= 500)
    {
        lastProbeCheck = millis();
        uint8_t resetResult = eeprom.reset();
        bool currentConnection = (resetResult == 0x00);

        // Only print when status changes
        if (currentConnection != probeConnected)
        {
            probeConnected = currentConnection;

            Serial.println();
            Serial.println("======================================");

            if (probeConnected)
            {
                Serial.println("AT21CS01 PROBE CONNECTED");
            }
            else {
                Serial.println("AT21CS01 PROBE DISCONNECTED");
            }
            Serial.println("======================================");
        }
    }
    delay(10);
}