#ifndef STORAGE_H
#define STORAGE_H

#include "device.h"
#include "probe_database.h"
#include "crc.h"
#include <string.h>
#include <Preferences.h>
#include "AT21CS01.h"

extern AT21CS01 eeprom;
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

    // Probe ID
    data.probeId = device.id.toInt();

    // Probe name
    strncpy(
        data.probeName,
        device.probeName.c_str(),
        sizeof(data.probeName) - 1
    );

    // Pulse strategy
    strncpy(
        data.pulseStrategy,
        device.pulseStrategy.c_str(),
        sizeof(data.pulseStrategy) - 1
    );

    // Probe values
   // Internal product settings
// Expiry = 2 years
// Use time = 24 hours
data.expiryYear = 2;
data.totalCycle = device.totalCycle;
data.totalPulse = device.totalPulse;
data.useTime = 24;

    // Internal thresholds
    data.startThreshold = 0;
    data.endThreshold = 0;

    // CRC will be added in the next step
    data.crc = 0;

    saveStruct(device.id, data);
}

bool saveProbeToAT21CS01(const char *c_probeId)
{
    uint16_t probe_id = atoi(c_probeId);
    const uint8_t DEVICE_ADDRESS = 0xA0;

    const uint8_t CONFIG_ADDRESS  = 0x10;
    const uint8_t DETECT_ADDRESS  = 0x30;
    const uint8_t START_ADDRESS   = 0x40;
    const uint8_t RUNTIME_ADDRESS = 0x50;

    const uint8_t CONFIG_LENGTH  = 28;
    const uint8_t EVENT_LENGTH   = 12;
    const uint8_t RUNTIME_LENGTH = 8;

    uint8_t buffer[28];

    /*
     * =================================================
     * FIND PROBE IN MASTER DATABASE
     * =================================================
     */

    const ProbeData *probe = nullptr;
    Serial.println("Compare Probe Id");
    Serial.println(c_probeId);
    for (int i = 0; i < TOTAL_PROBES; i++)
    {
        Serial.println(probeDatabase[i].id);
        if (atoi(c_probeId) == atoi(probeDatabase[i].id))
        {
            Serial.println("Probe Found");
            probe = &probeDatabase[i];
            break;
        }
    }

    if (probe == nullptr)
    {
        Serial.println("ERROR: Probe not found in database");
        return false;
    }

    /*
     * =================================================
     * BUILD CONFIGURATION DATA
     * =================================================
     *
     * EEPROM 0x10 - 0x2B
     *
     * 0-1    Probe ID
     * 2-3    Pulse Count
     * 4-5    Cycle Count
     * 6-7    Start Threshold
     * 8-9    End Threshold
     * 10-13  End Time
     * 14-17  Expiry Time
     * 18-25  Creation Time
     * 26-27  CRC
     */

    memset(buffer, 0, sizeof(buffer));

    put_u16(&buffer[0], probe_id);
    put_u16(&buffer[2], probe->totalPulse);
    put_u16(&buffer[4], probe->totalCycle);
    put_u16(&buffer[6], 0);                 // Start threshold
    put_u16(&buffer[8], probe->totalPulse); // End threshold
    put_u32(&buffer[10], 900);              // endThresholdTime = 900 seconds
    put_u32(&buffer[14], 63072000UL);       // Expiry time = 2 years = 2 × 365 × 24 × 60 × 60 seconds
    
    // Creation time = 0 for now
    for (int i = 18; i <= 25; i++)
    {
        buffer[i] = 0;
    }

    /*
     * =================================================
     * CALCULATE CRC
     * =================================================
     */

    uint16_t crc = modbus_crc16(buffer, CONFIG_LENGTH - 2);
    put_u16(&buffer[26], crc);

    /*
     * =================================================
     * PRINT CONFIGURATION
     * =================================================
     */

    Serial.println();
    Serial.println("======================================");
    Serial.println("AT21CS01 PROBE SAVE");
    Serial.println("======================================");

    Serial.print("Probe ID          : ");
    Serial.println(probe_id);

    Serial.print("Probe Name        : ");
    Serial.println(probe->probeName);

    Serial.print("Pulse Strategy    : ");
    Serial.println(probe->pulseStrategy);

    Serial.print("Pulse Count       : ");
    Serial.println(probe->totalPulse);

    Serial.print("Cycle Count       : ");
    Serial.println(probe->totalCycle);

    Serial.print("Start Threshold   : ");
    Serial.println(0);

    Serial.print("End Threshold     : ");
    Serial.println(probe->totalPulse);

    Serial.print("End Time          : ");
    Serial.println(900);

    Serial.print("Expiry Time       : ");
    Serial.println(63072000UL);

    Serial.print("CRC               : 0x");
    Serial.println(crc, HEX);

    /*
     * =================================================
     * CONFIGURATION WRITE
     * =================================================
     *
     * IMPORTANT:
     *
     * AT21CS01 page size = 8 bytes.
     *
     * Therefore 28 bytes MUST NOT be written
     * in one transaction.
     *
     * We write:
     *
     * 0x10 - 0x17 = 8 bytes
     * 0x18 - 0x1F = 8 bytes
     * 0x20 - 0x27 = 8 bytes
     * 0x28 - 0x2B = 4 bytes
     */

    Serial.println();
    Serial.println("Writing configuration to AT21CS01...");

    // -----------------------------------------------
    // CONFIG PAGE 1
    // -----------------------------------------------
    eeprom.eepromWrite(DEVICE_ADDRESS, 0x10, &buffer[0], 8);
    delay(10);

    // -----------------------------------------------
    // CONFIG PAGE 2
    // -----------------------------------------------
    eeprom.eepromWrite(DEVICE_ADDRESS, 0x18, &buffer[8], 8);
    delay(10);

    // -----------------------------------------------
    // CONFIG PAGE 3
    // -----------------------------------------------
    eeprom.eepromWrite(DEVICE_ADDRESS, 0x20, &buffer[16], 8);
    delay(10);

    // -----------------------------------------------
    // CONFIG PAGE 4
    // -----------------------------------------------
    eeprom.eepromWrite(DEVICE_ADDRESS, 0x28, &buffer[24], 4);
    delay(10);

    Serial.println();
    Serial.println("EEPROM DATA Writing :");
    for (int i = 0; i < 28; i++)
    {
        Serial.printf("ADDR 0x%02X = 0x%02X\n", 0x10 + i, buffer[i]);
    }

    // =================================================
    // DEBUG: READ CONFIGURATION BACK AFTER SAVE
    // =================================================
    delay(1000);

    uint8_t verifyBuffer[28];
    memset(verifyBuffer, 0, sizeof(verifyBuffer));

    Serial.println();
    Serial.println("======================================");
    Serial.println("VERIFY CONFIGURATION AFTER EEPROM WRITE");
    Serial.println("======================================");

    noInterrupts();

    eeprom.randomRead(DEVICE_ADDRESS, 0x10, &verifyBuffer[0], 8);
    eeprom.randomRead(DEVICE_ADDRESS, 0x18, &verifyBuffer[8], 8);
    eeprom.randomRead(DEVICE_ADDRESS, 0x20, &verifyBuffer[16], 8);
    eeprom.randomRead(DEVICE_ADDRESS, 0x28, &verifyBuffer[24], 4);
    interrupts();

    delay(10);

    Serial.println();
    Serial.println("EEPROM DATA READ :");

    for (int i = 0; i < 28; i++)
    {
        Serial.printf("ADDR 0x%02X = 0x%02X\n", 0x10 + i, verifyBuffer[i]);
    }

    uint16_t verifyStoredCRC = get_u16(&verifyBuffer[26]);
    uint16_t verifyCalculatedCRC = modbus_crc16(verifyBuffer, 26);

    Serial.printf("Stored CRC     : 0x%04X\n", verifyStoredCRC);
    Serial.printf("Calculated CRC : 0x%04X\n", verifyCalculatedCRC);

    if (verifyStoredCRC == verifyCalculatedCRC)
    {
        Serial.println("SAVE VERIFY CRC : PASS");
    }
    else
    {
        Serial.println("SAVE VERIFY CRC : FAIL");
    }

    Serial.println("======================================");

    /*
     * =================================================
     * DETECTION DATA - 0x30
     * =================================================
     */

    memset(buffer, 0, sizeof(buffer));

    put_u16(&buffer[0], probe_id);
    crc = modbus_crc16(buffer, EVENT_LENGTH - 2);
    put_u16(&buffer[10], crc);

    // 8-byte page
    eeprom.eepromWrite(DEVICE_ADDRESS, 0x30, &buffer[0], 8);
    delay(10);

    // Remaining 4 bytes
    eeprom.eepromWrite(DEVICE_ADDRESS, 0x38, &buffer[8], 4);
    delay(10);

    /*
     * =================================================
     * START DETECTION DATA - 0x40
     * =================================================
     */

    memset(buffer, 0, sizeof(buffer));
    put_u16(&buffer[0], probe_id);
    crc = modbus_crc16(buffer, EVENT_LENGTH - 2);
    put_u16(&buffer[10], crc);

    // 8-byte page
    eeprom.eepromWrite(DEVICE_ADDRESS, 0x40, &buffer[0], 8);
    delay(10);

    // Remaining 4 bytes
    eeprom.eepromWrite(DEVICE_ADDRESS, 0x48, &buffer[8], 4);
    delay(10);

    /*
     * =================================================
     * RUNTIME DATA - 0x50
     * =================================================
     */

    memset(buffer, 0, sizeof(buffer));
    put_u16(&buffer[0], probe_id);

    put_u16(&buffer[2], 0);     // Initial pulse count
    put_u16(&buffer[4], 0);     // Initial status

    crc = modbus_crc16(buffer, RUNTIME_LENGTH - 2);
    put_u16(&buffer[6], crc);

    eeprom.eepromWrite(DEVICE_ADDRESS, RUNTIME_ADDRESS, buffer, 8);
    delay(10);
    
    /*
     * =================================================
     * COMPLETE
     * =================================================
     */

    Serial.println("AT21CS01 PROBE SAVE COMPLETED");
    Serial.println("======================================");
    return true;
}

bool readProbeFromAT21CS01(Device &device)
{
    uint8_t buffer[28];

    uint16_t probeID;
    uint16_t pulseCount;
    uint16_t cycleCount;
    uint16_t startThreshold;
    uint16_t endThreshold;

    uint32_t endTime;
    uint32_t expiryTime;

    uint16_t storedCRC;
    uint16_t calculatedCRC;

    // =================================================
    // RESET AT21CS01 BUS
    // =================================================

    Serial.println("Resetting AT21CS01 bus...");

    eeprom.reset();

    delayMicroseconds(1000);

    // =================================================
    // READ EEPROM
    //
    // IMPORTANT:
    // AT21CS01 uses precise single-wire timing.
    // Disable ESP32 interrupts during the actual
    // EEPROM transaction so WiFi interrupts cannot
    // disturb the timing.
    // =================================================

    Serial.println("Reading AT21CS01 configuration...");

    noInterrupts();

    eeprom.randomRead(0xA0, 0x10, &buffer[0], 8);
    eeprom.randomRead(0xA0, 0x18, &buffer[8], 8);
    eeprom.randomRead(0xA0, 0x20, &buffer[16], 8);
    eeprom.randomRead(0xA0, 0x28, &buffer[24], 4);

    interrupts();

    // Give the bus a little recovery time
    delayMicroseconds(1000);

    // =================================================
    // PRINT RAW EEPROM DATA
    // =================================================

    Serial.println();
    Serial.println("======================================");
    Serial.println("AT21CS01 PROBE READ - RAW CHUNK TEST");
    Serial.println("======================================");

    Serial.println();
    Serial.println("EEPROM CONFIGURATION RAW DATA:");

    for (int i = 0; i < 28; i++)
    {
        Serial.printf(
            "ADDR 0x%02X = 0x%02X\n",
            0x10 + i,
            buffer[i]
        );
    }

    // =================================================
    // GET STORED CRC
    // =================================================

    storedCRC =
        ((uint16_t)buffer[26]) |
        ((uint16_t)buffer[27] << 8);

    // =================================================
    // CALCULATE CRC
    //
    // CRC covers bytes 0..25
    // =================================================

    calculatedCRC =
        modbus_crc16(buffer, 26);

    Serial.printf(
        "Stored CRC     : 0x%04X\n",
        storedCRC
    );

    Serial.printf(
        "Calculated CRC : 0x%04X\n",
        calculatedCRC
    );

    // =================================================
    // CRC VALIDATION
    // =================================================

    if (storedCRC != calculatedCRC)
    {
        Serial.println("ERROR: CRC MISMATCH");
        Serial.println("======================================");

        return false;
    }

    // =================================================
    // DECODE PROBE ID
    // =================================================

    probeID =
        ((uint16_t)buffer[0]) |
        ((uint16_t)buffer[1] << 8);

    // =================================================
    // DECODE PULSE COUNT
    // =================================================

    pulseCount =
        ((uint16_t)buffer[2]) |
        ((uint16_t)buffer[3] << 8);

    // =================================================
    // DECODE CYCLE COUNT
    // =================================================

    cycleCount =
        ((uint16_t)buffer[4]) |
        ((uint16_t)buffer[5] << 8);

    // =================================================
    // DECODE START THRESHOLD
    // =================================================

    startThreshold =
        ((uint16_t)buffer[6]) |
        ((uint16_t)buffer[7] << 8);

    // =================================================
    // DECODE END THRESHOLD
    // =================================================

    endThreshold =
        ((uint16_t)buffer[8]) |
        ((uint16_t)buffer[9] << 8);

    // =================================================
    // DECODE END TIME
    // =================================================

    endTime =
        ((uint32_t)buffer[10]) |
        ((uint32_t)buffer[11] << 8) |
        ((uint32_t)buffer[12] << 16) |
        ((uint32_t)buffer[13] << 24);

    // =================================================
    // DECODE EXPIRY TIME
    // =================================================

    expiryTime =
        ((uint32_t)buffer[14]) |
        ((uint32_t)buffer[15] << 8) |
        ((uint32_t)buffer[16] << 16) |
        ((uint32_t)buffer[17] << 24);

    // =================================================
    // FIND PROBE INFORMATION FROM DATABASE
    // =================================================
    device.id = String(probeID);

    for (int i = 0; i < TOTAL_PROBES; i++)
    {
        if (device.id == String(probeDatabase[i].id))
        {
            device.probeName =
                probeDatabase[i].probeName;

            device.pulseStrategy =
                probeDatabase[i].pulseStrategy;

            break;
        }
    }
    // =================================================
    // UPDATE DEVICE DATA
    // =================================================
    device.totalPulse = pulseCount;
    device.totalCycle = cycleCount;

    device.useTime = 24;
    device.expiryYear = 2;

    // =================================================
    // DEBUG OUTPUT
    // =================================================

    Serial.println();
    Serial.println("CONFIGURATION DECODED SUCCESSFULLY");

    Serial.print("Probe ID      : ");
    Serial.println(probeID);

    Serial.print("Pulse Count   : ");
    Serial.println(pulseCount);

    Serial.print("Cycle Count   : ");
    Serial.println(cycleCount);

    Serial.print("Start Thresh  : ");
    Serial.println(startThreshold);

    Serial.print("End Thresh    : ");
    Serial.println(endThreshold);

    Serial.print("End Time      : ");
    Serial.println(endTime);

    Serial.print("Expiry Time   : ");
    Serial.println(expiryTime);

    Serial.println("======================================");

    return true;
}

void readDevice(Device &device)
{
    DeviceStorage data;

    readStruct(device.id, data);

    device.id = String(data.probeId);

    device.probeName =
        String(data.probeName);

    device.pulseStrategy =
        String(data.pulseStrategy);

    device.expiryYear =
        data.expiryYear;

    device.totalCycle =
        data.totalCycle;

    device.totalPulse =
        data.totalPulse;

    device.useTime =
        data.useTime;
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

        temp.expiryYear = 2;
        temp.totalCycle = probeDatabase[i].totalCycle;
        temp.totalPulse = probeDatabase[i].totalPulse;
        temp.useTime = 24;
        saveDevice(temp);
    }
    saveString("database_init", "1");
    Serial.println("Flash Database Created Successfully");
}

#endif