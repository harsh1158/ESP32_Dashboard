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
    uint16_t probeId;          // 2 bytes

    char probeName[16];        // 16 bytes
    char pulseStrategy[12];    // 12 bytes

    uint32_t expiryYear;       // 4 bytes

    uint16_t totalCycle;       // 2 bytes
    uint16_t totalPulse;       // 2 bytes
    uint16_t useTime;          // 2 bytes

    uint16_t startThreshold;   // 2 bytes
    uint16_t endThreshold;     // 2 bytes

    uint16_t crc;              // 2 bytes
};

struct ProbeEEPROMRecord
{
    uint16_t probeId;          // 2 bytes
    char probeName[16];        // 16 bytes
    char pulseStrategy[12];    // 12 bytes

    uint32_t expiryYear;       // 4 bytes

    uint16_t totalCycle;       // 2 bytes
    uint16_t totalPulse;       // 2 bytes
    uint16_t useTime;          // 2 bytes

    uint16_t startThreshold;   // 2 bytes
    uint16_t endThreshold;     // 2 bytes

    uint16_t crc;              // 2 bytes
};

#endif