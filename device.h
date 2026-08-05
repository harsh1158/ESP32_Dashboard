#ifndef DEVICE_H
#define DEVICE_H

#include <Arduino.h>

struct Device
{
    String id;
    String probeName;
    String pulseStrategy; 
    int expiryYear;
    int totalCycle;
    int totalPulse;
    int useTime;
};

struct DeviceStorage
{
    char probeName[40];
    char pulseStrategy[20];

    int expiryYear;
    int totalCycle;
    int totalPulse;
    int useTime;
};

#endif