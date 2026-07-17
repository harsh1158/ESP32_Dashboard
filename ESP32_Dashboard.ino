#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>

#include "wifi_config.h"
#include "dashboard.h"
#include "device.h"
#include "storage.h"
#include "security.h"

WebServer server(80);
Preferences prefs;
Device device;
bool clientLocked = false;

IPAddress activeClient;

#include "api.h"

void setup()
{
    Serial.begin(115200);
    Serial.println("******** SETUP START ********");
    Serial.println("Before prefs.begin()");
prefs.begin("dashboard", false);

Serial.println("Before initializeDatabase()");
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
server.on("/save", HTTP_POST, handleSave);
server.on("/read", HTTP_GET, handleRead);
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

    delay(50);
}