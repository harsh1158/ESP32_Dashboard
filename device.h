#ifndef DEVICE_H
#define DEVICE_H

#include <Arduino.h>

struct Device
{
    String id;

    int expiryYear;

    int cycle;

    int pulseCycle;

    int totalPulse;

    int useTime;
};

#endif