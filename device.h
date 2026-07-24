#ifndef DEVICE_H
#define DEVICE_H

#include <Arduino.h>

struct Device
{
    String id;
    String probeName;

    int expiryYear;
    int totalCycle;
    int totalPulse;
    int useTime;
};

struct DeviceStorage
{
    int expiryYear;
    int totalCycle;
    int totalPulse;
    int useTime;
};

#endif