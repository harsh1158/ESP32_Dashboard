#ifndef API_H
#define API_H

void handleRoot()
{
    if(!isClientAllowed())
    {
        server.send(
            403,
            "text/html",
            "<h1>403 - Maximum 1 Client Allowed</h1>"
            "<br>"
            "<h2>This ESP32 Dashboard is already in use.</h2>"
        );
        return;
    }

    server.send(200,"text/html",MAIN_page);
}

/*****************************************************
 * Save Data to Flash
 *****************************************************/
void handleSave()
{   
     if(!isClientAllowed())
{
    server.send(403,
                "text/plain",
                "Maximum 1 Client Allowed");

    return;
}
    if (!server.hasArg("id"))
    {
        server.send(400, "text/plain", "Missing ID");
        return;
    }

    device.id = server.arg("id");

    device.expiryYear = server.arg("expiry").toInt();
    device.useTime =
        server.arg("useTime").toInt();

    /*****************************************************
     * Temporary values
     * Later these will come from UART
     *****************************************************/
    createDefaultDevice(device);

    saveDevice(device);

    Serial.println();
    Serial.println("======================================");
    Serial.println("DATA SAVED TO FLASH");
    Serial.println("======================================");

    Serial.print("ID          : ");
    Serial.println(device.id);

    Serial.print("Expiry      : ");
    Serial.println(device.expiryYear);

    Serial.print("Use Time    : ");
    Serial.println(device.useTime);

    Serial.print("Cycle       : ");
    Serial.println(device.cycle);

    Serial.print("Pulse Cycle : ");
    Serial.println(device.pulseCycle);

    Serial.print("Total Pulse : ");
    Serial.println(device.totalPulse);

    Serial.println("======================================");

    server.send(200, "text/plain", "Saved Successfully");
}

void handleRead()
{
     if(!isClientAllowed())
{
    server.send(403,
                "text/plain",
                "Maximum 1 Client Allowed");

    return;
}
    if (!server.hasArg("id"))
    {
        server.send(400, "text/plain", "Missing ID");
        return;
    }

    device.id = server.arg("id");

    readDevice(device);

    String json = "{";

    json += "\"id\":\"" + device.id + "\",";

    json += "\"expiry\":" + String(device.expiryYear) + ",";

    json += "\"useTime\":" + String(device.useTime) + ",";

    json += "\"cycle\":\"" + String(device.cycle) + "\",";

    json += "\"pulse\":\"" + String(device.pulseCycle) + "\",";

    json += "\"total\":\"" + String(device.totalPulse) + "\"";

    json += "}";

    Serial.println();
    Serial.println("======================================");
    Serial.println("DATA READ FROM FLASH");
    Serial.println("======================================");

    Serial.print("ID          : ");
    Serial.println(device.id);

    Serial.print("Expiry      : ");
    Serial.println(device.expiryYear);

    Serial.print("Use Time    : ");
    Serial.println(device.useTime);

    Serial.print("Cycle       : ");
    Serial.println(device.cycle);

    Serial.print("Pulse Cycle : ");
    Serial.println(device.pulseCycle);

    Serial.print("Total Pulse : ");
    Serial.println(device.totalPulse);

    Serial.println("======================================");

    server.send(200, "application/json", json);
}

#endif