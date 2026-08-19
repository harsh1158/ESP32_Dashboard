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
    uint16_t probeId;          

    char probeName[16];        
    char pulseStrategy[12];    

    uint32_t expiryYear;       

    uint16_t totalCycle;       
    uint16_t totalPulse;       
    uint16_t useTime;          

    uint16_t startThreshold;   
    uint16_t endThreshold;     

    uint16_t crc;              
};

struct ProbeEEPROMRecord
{
    uint16_t probeId;          
    char probeName[16];        
    char pulseStrategy[12];    

    uint32_t expiryYear;       

    uint16_t totalCycle;       
    uint16_t totalPulse;       
    uint16_t useTime;          

    uint16_t startThreshold;   
    uint16_t endThreshold;     

    uint16_t crc;              
};

#endif