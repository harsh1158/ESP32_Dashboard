#ifndef API_H
#define API_H

#include "probe_database.h"

bool loggedIn = false;

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
if(loggedIn)
{
    server.send(200,"text/html",MAIN_page);
}
else
{
    server.send(200,"text/html",LOGIN_PAGE);
}
}

/*****************************************************
 * Save Data to Flash
 *****************************************************/
void handleSave()
{   
    if(!loggedIn) {
        server.send(401, "text/plain", "Unauthorized");
        return;
    }
    if(!isClientAllowed()) {
        server.send(403, "text/plain", "Maximum 1 Client Allowed");
        return;
    }
    if (!server.hasArg("id")) {
        server.send(400, "text/plain", "Missing ID");
        return;
    }
    Device tmpDevice;
    tmpDevice.id = server.arg("id");
    // Dashboard dropdown values are ignored internally.
    // Product configuration is fixed to:
    tmpDevice.expiryYear = 2;          // Expiry = 2 years
    tmpDevice.useTime = 24;            // Use Time = 24 hours

    for (int i = 0; i < TOTAL_PROBES; i++) {
        if (tmpDevice.id == probeDatabase[i].id) {
            tmpDevice.probeName = probeDatabase[i].probeName;
            tmpDevice.pulseStrategy = probeDatabase[i].pulseStrategy;
            tmpDevice.totalCycle = probeDatabase[i].totalCycle;
            tmpDevice.totalPulse = probeDatabase[i].totalPulse;
            break;
        }
    }
    
    // Internal Flash data storing
    saveDevice(tmpDevice);

    // Store into the ATS21CS01
    bool eepromSaved = saveProbeToAT21CS01(tmpDevice.id.c_str());         
    if (!eepromSaved) {
        Serial.println("ERROR: AT21CS01 SAVE FAILED");
        server.send(500, "text/plain", "Flash Saved But AT21CS01 Save Failed");
        return;
    }

    Serial.println("===== DATABASE MATCH =====");
    Serial.println(tmpDevice.id);
    Serial.println(tmpDevice.probeName);
    Serial.println(tmpDevice.totalCycle);
    Serial.println(tmpDevice.totalPulse);
    Serial.println("==========================");
    Serial.println();
    Serial.println("======================================");
    Serial.println("DATA SAVED TO FLASH");
    Serial.println("======================================");
    Serial.print("ID          : ");
    Serial.println(tmpDevice.id);

    Serial.print("Expiry      : ");
    Serial.println(tmpDevice.expiryYear);

    Serial.print("Use Time    : ");
    Serial.println(tmpDevice.useTime);

    Serial.print("Probe Name  : ");
    Serial.println(tmpDevice.probeName);

    Serial.print("Pulse Strategy  : ");
    Serial.println(tmpDevice.pulseStrategy);
    
    Serial.print("Total Cycle : ");
    Serial.println(tmpDevice.totalCycle);

    Serial.print("Total Pulse : ");
    Serial.println(tmpDevice.totalPulse);

    Serial.println("======================================");

    String response = "{";
    response += "\"probeName\":\"" + tmpDevice.probeName + "\",";
    response += "\"pulseStrategy\":\"" + tmpDevice.pulseStrategy + "\",";
    response += "\"totalCycle\":" + String(tmpDevice.totalCycle) + ",";
    response += "\"totalPulse\":" + String(tmpDevice.totalPulse) + ",";
    response += "\"expiry\":" + String(tmpDevice.expiryYear) + ",";
    response += "\"useTime\":" + String(tmpDevice.useTime);
    response += "}";

    server.send(200, "application/json", response);
}

void handleRead()
{
    if (!loggedIn)
    {
        server.send(
            401,
            "text/plain",
            "Unauthorized"
        );
        return;
    }

    if (!isClientAllowed())
    {
        server.send(
            403,
            "text/plain",
            "Maximum 1 Client Allowed"
        );
        return;
    }

    if (!server.hasArg("id"))
    {
        server.send(
            400,
            "text/plain",
            "Missing ID"
        );
        return;
    }

    // -------------------------------------------------
    // Get selected Probe ID from Dashboard
    // -------------------------------------------------
    device.id = server.arg("id");

    Serial.println();
    Serial.println("======================================");
    Serial.println("AT21CS01 PROBE READ");
    Serial.println("======================================");

    Serial.print("Requested Probe ID : ");
    Serial.println(device.id);

    // -------------------------------------------------
    // READ PROBE FROM AT21CS01
    // -------------------------------------------------
    Serial.println();
    Serial.println("========== DEBUG FIRST READ ==========");

    Serial.print("Requested ID : ");
    Serial.println(device.id);

    Serial.println("STEP 1: Before EEPROM reset");
    eeprom.reset();
    Serial.println("STEP 2: EEPROM reset completed");

    delay(20);

    Serial.println("STEP 3: Starting readProbeFromAT21CS01()");

    bool eepromReadOK = readProbeFromAT21CS01(device);

    Serial.println("STEP 4: readProbeFromAT21CS01() returned");

    Serial.print("READ RESULT : ");
    Serial.println(eepromReadOK ? "TRUE" : "FALSE");

    Serial.println("======================================");

    if (!eepromReadOK)
    {
        Serial.println("ERROR: AT21CS01 READ FAILED");
        Serial.println("======================================");

        server.send(
            500,
            "text/plain",
            "AT21CS01 Probe Read Failed"
        );

        return;
    }

    // -------------------------------------------------
    // Print data read from AT21CS01
    // -------------------------------------------------
    Serial.println();
    Serial.println("======================================");
    Serial.println("DATA READ FROM AT21CS01");
    Serial.println("======================================");

    Serial.print("ID              : ");
    Serial.println(device.id);

    Serial.print("Probe Name      : ");
    Serial.println(device.probeName);

    Serial.print("Pulse Strategy  : ");
    Serial.println(device.pulseStrategy);

    Serial.print("Expiry Year     : ");
    Serial.println(device.expiryYear);

    Serial.print("Total Cycle     : ");
    Serial.println(device.totalCycle);

    Serial.print("Total Pulse     : ");
    Serial.println(device.totalPulse);

    Serial.print("Use Time        : ");
    Serial.println(device.useTime);

    Serial.println("======================================");

    // -------------------------------------------------
    // Return data to Dashboard
    // -------------------------------------------------
    String json = "{";

    json += "\"id\":\"" + device.id + "\",";
    json += "\"expiry\":" + String(device.expiryYear) + ",";
    json += "\"useTime\":" + String(device.useTime) + ",";
    json += "\"probeName\":\"" + device.probeName + "\",";
    json += "\"pulseStrategy\":\"" + device.pulseStrategy + "\",";
    json += "\"totalCycle\":" + String(device.totalCycle) + ",";
    json += "\"totalPulse\":" + String(device.totalPulse) + ",";
    json += "\"expired\":" + String(probeExpired ? "true" : "false");

    json += "}";

    server.send(
        200,
        "application/json",
        json
    );
}

void handleProbeStatus()
{
    if (!loggedIn)
    {
        server.send(
            401,
            "text/plain",
            "Unauthorized"
        );
        return;
    }
    String response = "{";
    response += "\"connected\":";
    response += (probeConnected ? "true" : "false");
    response += "}";

    server.send( 200, "application/json", response);
}

void handleLogin()
{
    String username = server.arg("username");
    String password = server.arg("password");

    String storedUsername =
    prefs.getString("username");

    String storedPassword =
    prefs.getString("password");
    if(username == storedUsername &&
    password == storedPassword)
    {
    loggedIn = true;

    server.send(200,
                "text/plain",
                "Login Success");
}
    else
    {
        server.send(401,
                    "text/plain",
                    "Invalid Username or Password");
    }
}

void handleLogout()
{
    loggedIn = false;

    server.send(200,
                "text/plain",
                "Logout Success");
}

void handleChangeLogin()
{
    if(!loggedIn)
    {
        server.send(401, "text/plain", "Unauthorized");
        return;
    }

    String currentPassword = server.arg("currentPassword");
    String newUsername     = server.arg("newUsername");
    String newPassword     = server.arg("newPassword");
    String confirmPassword = server.arg("confirmPassword");

    String storedPassword = prefs.getString("password");

    if(currentPassword != storedPassword)
    {
        server.send(400,
                    "text/plain",
                    "Current Password Incorrect");
        return;
    }

    if(newUsername.length() == 0)
    {
        server.send(400,
                    "text/plain",
                    "Username Cannot Be Empty");
        return;
    }

    if(newPassword.length() == 0)
    {
        server.send(400,
                    "text/plain",
                    "Password Cannot Be Empty");
        return;
    }

    if(newPassword != confirmPassword)
    {
        server.send(400,
                    "text/plain",
                    "Passwords Do Not Match");
        return;
    }

    prefs.putString("username", newUsername);
    prefs.putString("password", newPassword);

    server.send(200,
                "text/plain",
                "Login Updated Successfully");
}

#endif