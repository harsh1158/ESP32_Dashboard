#ifndef STORAGE_H
#define STORAGE_H

#include "device.h"
#include "probe_database.h"
#include <string.h>
#include <Preferences.h>

extern Preferences prefs;

void saveStruct(String key, DeviceStorage &data)
{
    prefs.putBytes(key.c_str(),
                   &data,
                   sizeof(DeviceStorage));
}

void readStruct(String key, DeviceStorage &data)
{
    memset(&data, 0, sizeof(DeviceStorage));

    size_t len =
        prefs.getBytes(key.c_str(),
                       &data,
                       sizeof(DeviceStorage));

    if(len != sizeof(DeviceStorage))
    {
        data.expiryYear = 1;
        data.totalCycle = 0;
        data.totalPulse = 0;
        data.useTime = 6;
    }
}

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
    DeviceStorage data;

memset(&data, 0, sizeof(DeviceStorage));

strncpy(data.probeName,
        device.probeName.c_str(),
        sizeof(data.probeName) - 1);

strncpy(data.pulseStrategy,
        device.pulseStrategy.c_str(),
        sizeof(data.pulseStrategy) - 1);

data.expiryYear = device.expiryYear;
data.totalCycle = device.totalCycle;
data.totalPulse = device.totalPulse;
data.useTime    = device.useTime;

saveStruct(device.id, data);
}

void readDevice(Device &device)
{
    DeviceStorage data;

    readStruct(device.id, data);

    device.probeName = String(data.probeName);
    device.pulseStrategy = String(data.pulseStrategy);

    device.expiryYear = data.expiryYear;
    device.totalCycle = data.totalCycle;
    device.totalPulse = data.totalPulse;
    device.useTime    = data.useTime;
}

void initializeDatabase()
{
    if (readString("database_init", "0") == "1")
    {
        Serial.println("Flash Database Already Exists");
        return;
    }

    Serial.println("Creating Flash Database...");

    Device temp;

    for (int i = 0; i < TOTAL_PROBES; i++)
    {
        temp.id = probeDatabase[i].id;
temp.probeName = probeDatabase[i].probeName;
temp.pulseStrategy = probeDatabase[i].pulseStrategy;

temp.expiryYear = 1;
temp.totalCycle = probeDatabase[i].totalCycle;
temp.totalPulse = probeDatabase[i].totalPulse;
temp.useTime = 6;

        saveDevice(temp);
    }

    saveString("database_init", "1");

    Serial.println("Flash Database Created Successfully");
}

#endif