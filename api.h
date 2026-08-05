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
    if(!loggedIn)
{
    server.send(401,
                "text/plain",
                "Unauthorized");

    return;
}
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
device.useTime = server.arg("useTime").toInt();

for (int i = 0; i < TOTAL_PROBES; i++)
{
    if (device.id == probeDatabase[i].id)
    {
        device.probeName = probeDatabase[i].probeName;
        device.pulseStrategy = probeDatabase[i].pulseStrategy;
        device.totalCycle = probeDatabase[i].totalCycle;
        device.totalPulse = probeDatabase[i].totalPulse;
        break;
    }
}
saveDevice(device);
Serial.println("===== DATABASE MATCH =====");
Serial.println(device.id);
Serial.println(device.probeName);
Serial.println(device.totalCycle);
Serial.println(device.totalPulse);
Serial.println("==========================");
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

    Serial.print("Probe Name  : ");
    Serial.println(device.probeName);

    Serial.print("Pulse Strategy  : ");
    Serial.println(device.pulseStrategy);
    
    Serial.print("Total Cycle : ");
    Serial.println(device.totalCycle);

    Serial.print("Total Pulse : ");
    Serial.println(device.totalPulse);

    Serial.println("======================================");

    server.send(200, "text/plain", "Saved Successfully");
}

void handleRead()
{
    if(!loggedIn)
{
    server.send(401,
                "text/plain",
                "Unauthorized");

    return;
}

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
    json += "\"probeName\":\"" + device.probeName + "\",";
    json += "\"pulseStrategy\":\"" + device.pulseStrategy + "\",";
    json += "\"totalCycle\":\"" + String(device.totalCycle) + "\",";
    json += "\"totalPulse\":\"" + String(device.totalPulse) + "\"";
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

   Serial.print("Probe Name  : ");
Serial.println(device.probeName);

Serial.print("Pulse Strategy  : ");
Serial.println(device.pulseStrategy);

Serial.print("Total Cycle : ");
Serial.println(device.totalCycle);

Serial.print("Total Pulse : ");
Serial.println(device.totalPulse);

    Serial.println("======================================");

    server.send(200, "application/json", json);
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