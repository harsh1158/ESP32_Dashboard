#ifndef STORAGE_H
#define STORAGE_H

#include "device.h"
#include <Preferences.h>

extern Preferences prefs;

void saveString(String key, String value)
{
    prefs.putString(key.c_str(), value);
}

String readString(String key, String defaultValue = "")
{
    return prefs.getString(key.c_str(), defaultValue);
}

void saveInt(String key, int value)
{
    prefs.putInt(key.c_str(), value);
}

int readInt(String key, int defaultValue = 0)
{
    return prefs.getInt(key.c_str(), defaultValue);
}

void saveDevice(Device &device)
{
    saveString(device.id + "_expiry", String(device.expiryYear));

    saveInt(device.id + "_cycle", device.cycle);

    saveInt(device.id + "_pulse", device.pulseCycle);

    saveInt(device.id + "_total", device.totalPulse);

    saveInt(device.id + "_useTime", device.useTime);
}

void readDevice(Device &device)
{
    device.expiryYear =
        readString(device.id + "_expiry", "1").toInt();

    device.cycle =
        readInt(device.id + "_cycle", 0);

    device.pulseCycle =
        readInt(device.id + "_pulse", 0);

    device.totalPulse =
        readInt(device.id + "_total", 0);

    device.useTime =
        readInt(device.id + "_useTime", 6);
}

void createDefaultDevice(Device &device)
{
    if(device.id=="ID001")
    {
        device.cycle=1000;
        device.pulseCycle=10;
    }
    else if(device.id=="ID002")
    {
        device.cycle=1500;
        device.pulseCycle=15;
    }
    else if(device.id=="ID003")
    {
        device.cycle=2000;
        device.pulseCycle=20;
    }
    else if(device.id=="ID004")
    {
        device.cycle=2500;
        device.pulseCycle=25;
    }
    else if(device.id=="ID005")
    {
        device.cycle=3000;
        device.pulseCycle=30;
    }
    else
    {
        device.cycle=0;
        device.pulseCycle=0;
    }

    device.totalPulse=
    device.cycle*
    device.pulseCycle;
}

void initializeDatabase()
{    
    if(readString("database_init","0")=="1")
    {
        Serial.println("Flash Database Already Exists");
        return;
    }

    Serial.println("Creating Flash Database...");

    Device temp;

    for(int i=1;i<=10;i++)
    {
        char id[6];
        sprintf(id,"ID%03d",i);

        temp.id=id;

        temp.expiryYear=1;

        temp.useTime=6;

        createDefaultDevice(temp);

        saveDevice(temp);
    }

    saveString("database_init","1");

    Serial.println("Flash Database Created Successfully");
}

#endif