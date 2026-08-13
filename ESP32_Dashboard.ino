#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>

#include "wifi_config.h"
#include "dashboard.h"
#include "login.h"
#include "device.h"
#include "storage.h"
#include "security.h"
#include "AT21CS01.h"
#include "crc.h"

WebServer server(80);
Preferences prefs;
Device device;
AT21CS01 eeprom(18);
bool clientLocked = false;

IPAddress activeClient;

#include "api.h"

void testAT21CS01()
{
    Serial.println();
    Serial.println("==============================");
    Serial.println("AT21CS01 EEPROM TEST");
    Serial.println("==============================");

    uint8_t deviceAddress = 0xA0;
    uint8_t startAddress = 0x10;

    uint8_t writeData[] = {
        0xA5,
        0x5A,
        0xAA,
        0x55,
        0x12,
        0x34,
        0x56,
        0x78
    };

    uint8_t count = sizeof(writeData);

    uint8_t readData[8];

    Serial.println("Writing EEPROM...");

    eeprom.eepromWrite(
        deviceAddress,
        startAddress,
        writeData,
        count
    );
    delay(10);
    Serial.println("WRITE COMPLETED");
    Serial.println();
    Serial.println("Reading EEPROM...");
    eeprom.randomRead(
        deviceAddress,
        startAddress,
        readData,
        count
    );
    bool passed = true;
    Serial.println();
    Serial.println("READ DATA:");
    for (uint8_t i = 0; i < count; i++)
    {
        Serial.print("ADDR 0x");
        if ((startAddress + i) < 0x10)
            Serial.print("0");
        Serial.print(startAddress + i, HEX);
        Serial.print(" = 0x");
        if (readData[i] < 0x10)
            Serial.print("0");

        Serial.println(readData[i], HEX);
        if (readData[i] != writeData[i])
        {
            passed = false;
        }
    }
    Serial.println();
    if (passed)
    {
        Serial.println("==============================");
        Serial.println("READ / WRITE TEST PASSED");
        Serial.println("==============================");
    }
    else
    {
        Serial.println("==============================");
        Serial.println("READ / WRITE TEST FAILED");
        Serial.println("==============================");
    }
}

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("AT21CS01 POWER-UP DELAY COMPLETE");

    eeprom.begin(18);

    Serial.println();
    Serial.println("==============================");
    Serial.println("AT21CS01 INITIALIZATION");
    Serial.println("==============================");

    uint8_t resetResult = eeprom.reset();

    Serial.print("RESET RESULT = 0x");
    Serial.println(resetResult, HEX);

    if (resetResult == 0x00)
    {
        Serial.println("AT21CS01 DEVICE DETECTED");
    }
    else
    {
        Serial.println("AT21CS01 DEVICE NOT DETECTED");
    }

    testAT21CS01();
    testEEPROMDeviceStorage();
    //testRealProbeStorage();
    testSmartProbeConfiguration();
    testSmartProbeDetectionData();
    testSmartProbeStartDetectionData();
    testSmartProbeRuntimeData();
    testReadProbeFromAT21CS01();
    testProbe1CompleteInitialization();

    Serial.println("******** SETUP START ********");
    Serial.println("Before prefs.begin()");
    prefs.begin("dashboard", false);

    Serial.println("Before initializeDatabase()");

    Serial.println("Preferences Cleared");

    if(!prefs.isKey("username"))
    {
        prefs.putString("username", "admin");
    }

    if(!prefs.isKey("password"))
    {
        prefs.putString("password", "1234");
    }
    initializeDatabase();

    Serial.println("After initializeDatabase()");
    prefs.begin("dashboard", false);
    Serial.println("Preferences Ready");
    Serial.println();
    Serial.println("==============================");
    Serial.println("ESP32 WiFi Module");
    Serial.println("==============================");

    WiFi.mode(WIFI_STA);
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
    {
        Serial.println("Static IP Configuration Failed!");
    }
    else
    {
        Serial.println("Static IP Configured Successfully");
    }

    Serial.println("Connecting to WiFi...");

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("------------------------------");
    Serial.println("WiFi Connected Successfully");
    Serial.print("IP Address : ");
    Serial.println(WiFi.localIP());

    Serial.print("Signal RSSI : ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");

    Serial.print("MAC Address : ");
    Serial.println(WiFi.macAddress());

    Serial.println("------------------------------");

    /*****************************************************
 * Start HTTP Server
 *****************************************************/

server.on("/", handleRoot);
server.on("/login", HTTP_POST, handleLogin);
server.on("/changeLogin", HTTP_POST, handleChangeLogin);
server.on("/logout", HTTP_GET, handleLogout);
server.on("/save", HTTP_POST, handleSave);
server.on("/read", HTTP_GET, handleRead);
server.begin();

Serial.println();
Serial.println("======================================");
Serial.println("HTTP Server Started Successfully");
Serial.print("Open Browser : http://");
Serial.println(WiFi.localIP());
Serial.println("======================================");

}

void testEEPROMDeviceStorage()
{
    Serial.println();
    Serial.println("==============================");
    Serial.println("REAL DEVICE STORAGE TEST");
    Serial.println("==============================");

    const uint8_t DEVICE_ADDRESS = 0xA0;
    const uint8_t MEMORY_ADDRESS = 0x20;

    // Get REAL probe information
    const ProbeData &probe = probeDatabase[0];

    DeviceStorage writeData;

    memset(&writeData, 0, sizeof(DeviceStorage));

    Serial.print("DeviceStorage size = ");
    Serial.println(sizeof(DeviceStorage));

    strncpy(
        writeData.probeName,
        probe.probeName,
        sizeof(writeData.probeName) - 1
    );

    strncpy(
        writeData.pulseStrategy,
        probe.pulseStrategy,
        sizeof(writeData.pulseStrategy) - 1
    );

    writeData.expiryYear = 2030;
    writeData.totalCycle = probe.totalCycle;
    writeData.totalPulse = probe.totalPulse;
    writeData.useTime = 6;

    Serial.println("SOURCE DATA:");

    Serial.print("Probe ID       : ");
    Serial.println(probe.id);

    Serial.print("Probe Name     : ");
    Serial.println(writeData.probeName);

    Serial.print("Pulse Strategy : ");
    Serial.println(writeData.pulseStrategy);

    Serial.print("Expiry Year    : ");
    Serial.println(writeData.expiryYear);

    Serial.print("Total Cycle    : ");
    Serial.println(writeData.totalCycle);

    Serial.print("Total Pulse    : ");
    Serial.println(writeData.totalPulse);

    Serial.print("Use Time       : ");
    Serial.println(writeData.useTime);

    // ---------------------------------
    // WRITE
    // ---------------------------------

    Serial.println();
    Serial.println("Writing DeviceStorage byte-by-byte...");

    uint8_t *writeBuffer =
        (uint8_t *)&writeData;

    for (uint16_t i = 0; i < sizeof(DeviceStorage); i++)
    {
        eeprom.singleByteWrite(
            DEVICE_ADDRESS,
            MEMORY_ADDRESS + i,
            writeBuffer[i]
        );

        delay(10);
    }

    Serial.println("WRITE COMPLETED");
    Serial.println();

    delay(100);

    // ---------------------------------
    // READ
    // ---------------------------------

    Serial.println();
    Serial.println("Reading DeviceStorage byte-by-byte...");

    uint8_t readBuffer[sizeof(DeviceStorage)];

    memset(
        readBuffer,
        0,
        sizeof(readBuffer)
    );

    for (uint16_t i = 0; i < sizeof(DeviceStorage); i++)
    {
        eeprom.randomRead(
            DEVICE_ADDRESS,
            MEMORY_ADDRESS + i,
            &readBuffer[i],
            1
        );

        delay(5);
    }
    Serial.println();
Serial.println("EEPROM RAW BYTES:");

for (uint16_t i = 0; i < sizeof(DeviceStorage); i++)
{
    Serial.print("ADDR 0x");

    if ((MEMORY_ADDRESS + i) < 0x10)
        Serial.print("0");

    Serial.print(MEMORY_ADDRESS + i, HEX);

    Serial.print(" = 0x");

    if (readBuffer[i] < 0x10)
        Serial.print("0");

    Serial.println(readBuffer[i], HEX);
}
    // ---------------------------------
    // DECODE
    // ---------------------------------

    DeviceStorage readData;

    memcpy(
        &readData,
        readBuffer,
        sizeof(DeviceStorage)
    );

    Serial.println();
    Serial.println("READ DATA:");

    Serial.print("Probe Name     : ");
    Serial.println(readData.probeName);

    Serial.print("Pulse Strategy : ");
    Serial.println(readData.pulseStrategy);

    Serial.print("Expiry Year    : ");
    Serial.println(readData.expiryYear);

    Serial.print("Total Cycle    : ");
    Serial.println(readData.totalCycle);

    Serial.print("Total Pulse    : ");
    Serial.println(readData.totalPulse);

    Serial.print("Use Time       : ");
    Serial.println(readData.useTime);

    // ---------------------------------
    // VERIFY
    // ---------------------------------

    bool passed = true;

    if (strcmp(
        writeData.probeName,
        readData.probeName
    ) != 0)
        passed = false;

    if (strcmp(
        writeData.pulseStrategy,
        readData.pulseStrategy
    ) != 0)
        passed = false;

    if (writeData.expiryYear != readData.expiryYear)
        passed = false;

    if (writeData.totalCycle != readData.totalCycle)
        passed = false;

    if (writeData.totalPulse != readData.totalPulse)
        passed = false;

    if (writeData.useTime != readData.useTime)
        passed = false;

    Serial.println();
    Serial.println("==============================");

    if (passed)
    {
        Serial.println("DEVICE STORAGE TEST PASSED");
    }
    else
    {
        Serial.println("DEVICE STORAGE TEST FAILED");
    }

    Serial.println("==============================");
}

void testSmartProbeConfiguration()
{
    Serial.println();
    Serial.println("==============================");
    Serial.println("SMART.C PROBE CONFIG TEST");
    Serial.println("==============================");

    const uint8_t MEMORY_ADDRESS = 0x10;
    const uint8_t CONFIG_LENGTH = 28;

    uint8_t buffer[CONFIG_LENGTH];
    memset(buffer, 0, sizeof(buffer));

    // ---------------------------------
    // Probe 1 configuration
    // ---------------------------------

    uint16_t probeID = 1;
    uint16_t pulseCount = 80;
    uint16_t cycleCount = 8;

    // smart.c requirement:
    // Start threshold = 0
    uint16_t startThreshold = 0;

    // Probe 1 = 80 pulses
    uint16_t endThreshold = 80;

    // From smart.c
    uint32_t endThresholdTime = 900;
    uint32_t expiryThresholdTime = 3600;

    // ---------------------------------
    // Build configuration buffer
    // ---------------------------------

    // ID
    buffer[0] = probeID & 0xFF;
    buffer[1] = (probeID >> 8) & 0xFF;

    // Pulse count
    buffer[2] = pulseCount & 0xFF;
    buffer[3] = (pulseCount >> 8) & 0xFF;

    // Cycle count
    buffer[4] = cycleCount & 0xFF;
    buffer[5] = (cycleCount >> 8) & 0xFF;

    // Start threshold
    buffer[6] = startThreshold & 0xFF;
    buffer[7] = (startThreshold >> 8) & 0xFF;

    // End threshold
    buffer[8] = endThreshold & 0xFF;
    buffer[9] = (endThreshold >> 8) & 0xFF;

    // End threshold time = 900 seconds
    buffer[10] = endThresholdTime & 0xFF;
    buffer[11] = (endThresholdTime >> 8) & 0xFF;
    buffer[12] = (endThresholdTime >> 16) & 0xFF;
    buffer[13] = (endThresholdTime >> 24) & 0xFF;

    // Expiry threshold = 3600 seconds
    buffer[14] = expiryThresholdTime & 0xFF;
    buffer[15] = (expiryThresholdTime >> 8) & 0xFF;
    buffer[16] = (expiryThresholdTime >> 16) & 0xFF;
    buffer[17] = (expiryThresholdTime >> 24) & 0xFF;

    // ---------------------------------
    // Creation time
    // ---------------------------------
    //
    // We are keeping this zero for this
    // first storage-format test.
    //
    // Later we will connect the RTC.
    //

    for (int i = 18; i < 26; i++)
    {
        buffer[i] = 0;
    }

    // ---------------------------------
    // CRC
    // ---------------------------------

    uint16_t crc =
        modbus_crc16(buffer, CONFIG_LENGTH - 2);

    buffer[26] = crc & 0xFF;
    buffer[27] = (crc >> 8) & 0xFF;

    // ---------------------------------
    // WRITE
    // ---------------------------------

    Serial.println("Writing 28-byte configuration...");

    for (uint8_t i = 0; i < CONFIG_LENGTH; i++)
    {
        eeprom.singleByteWrite(
            0xA0,
            MEMORY_ADDRESS + i,
            buffer[i]
        );

        delay(10);
    }

    Serial.println("WRITE COMPLETED");

    // ---------------------------------
    // READ
    // ---------------------------------

    uint8_t readBuffer[CONFIG_LENGTH];

    memset(
        readBuffer,
        0,
        sizeof(readBuffer)
    );

    Serial.println();
    Serial.println("Reading configuration...");

    for (uint8_t i = 0; i < CONFIG_LENGTH; i++)
    {
        eeprom.randomRead(
            0xA0,
            MEMORY_ADDRESS + i,
            &readBuffer[i],
            1
        );

        delay(5);
    }

    // ---------------------------------
    // RAW BYTES
    // ---------------------------------

    Serial.println();
    Serial.println("SMART.C RAW CONFIGURATION:");

    for (uint8_t i = 0; i < CONFIG_LENGTH; i++)
    {
        Serial.print("ADDR 0x");

        if ((MEMORY_ADDRESS + i) < 0x10)
            Serial.print("0");

        Serial.print(
            MEMORY_ADDRESS + i,
            HEX
        );

        Serial.print(" = 0x");

        if (readBuffer[i] < 0x10)
            Serial.print("0");

        Serial.println(
            readBuffer[i],
            HEX
        );
    }

    // ---------------------------------
    // VERIFY
    // ---------------------------------

    bool passed = true;

    for (uint8_t i = 0; i < CONFIG_LENGTH; i++)
    {
        if (buffer[i] != readBuffer[i])
        {
            passed = false;

            Serial.print("MISMATCH AT BYTE ");
            Serial.println(i);
        }
    }

    Serial.println();
    Serial.println("==============================");

    if (passed)
    {
        Serial.println(
            "SMART.C CONFIG TEST PASSED"
        );
    }
    else
    {
        Serial.println(
            "SMART.C CONFIG TEST FAILED"
        );
    }
    Serial.println("==============================");
}

void testSmartProbeDetectionData()
{
    Serial.println();
    Serial.println("==============================");
    Serial.println("SMART PROBE DETECTION DATA TEST");
    Serial.println("==============================");

    const uint8_t DEVICE_ADDRESS = 0xA0;
    const uint8_t DETECTION_ADDRESS = 0x30;
    const uint8_t DATA_LENGTH = 12;

    uint8_t writeBuffer[DATA_LENGTH];
    uint8_t readBuffer[DATA_LENGTH];

    memset(writeBuffer, 0, sizeof(writeBuffer));
    memset(readBuffer, 0, sizeof(readBuffer));

    // ---------------------------------
    // PROBE 1
    // ---------------------------------

    uint16_t probeId = 1;

    // Probe ID
    writeBuffer[0] = probeId & 0xFF;
    writeBuffer[1] = (probeId >> 8) & 0xFF;

    // Detection time = 0 initially
    // Bytes 2 to 9 remain 0

    // CRC
    uint16_t crc =
        modbus_crc16(writeBuffer, DATA_LENGTH - 2);

    writeBuffer[10] = crc & 0xFF;
    writeBuffer[11] = (crc >> 8) & 0xFF;

    Serial.println("SOURCE DATA:");
    Serial.print("Probe ID : ");
    Serial.println(probeId);

    Serial.print("CRC      : 0x");
    Serial.println(crc, HEX);

    // ---------------------------------
    // WRITE
    // ---------------------------------

    Serial.println();
    Serial.println("Writing detection data...");

    for (uint8_t i = 0; i < DATA_LENGTH; i++)
    {
        eeprom.singleByteWrite(
            DEVICE_ADDRESS,
            DETECTION_ADDRESS + i,
            writeBuffer[i]
        );

        delay(10);
    }

    Serial.println("WRITE COMPLETED");

    // ---------------------------------
    // READ
    // ---------------------------------

    Serial.println();
    Serial.println("Reading detection data...");

    for (uint8_t i = 0; i < DATA_LENGTH; i++)
    {
        eeprom.randomRead(
            DEVICE_ADDRESS,
            DETECTION_ADDRESS + i,
            &readBuffer[i],
            1
        );

        delay(5);
    }

    // ---------------------------------
    // RAW DATA
    // ---------------------------------

    Serial.println();
    Serial.println("EEPROM RAW BYTES:");

    for (uint8_t i = 0; i < DATA_LENGTH; i++)
    {
        Serial.print("ADDR 0x");

        if ((DETECTION_ADDRESS + i) < 0x10)
            Serial.print("0");

        Serial.print(DETECTION_ADDRESS + i, HEX);

        Serial.print(" = 0x");

        if (readBuffer[i] < 0x10)
            Serial.print("0");

        Serial.println(readBuffer[i], HEX);
    }

    // ---------------------------------
    // CRC VERIFY
    // ---------------------------------

    uint16_t storedCRC =
        readBuffer[10] |
        ((uint16_t)readBuffer[11] << 8);

    uint16_t calculatedCRC =
        modbus_crc16(readBuffer, DATA_LENGTH - 2);

    Serial.println();
    Serial.print("Stored CRC     : 0x");
    Serial.println(storedCRC, HEX);

    Serial.print("Calculated CRC : 0x");
    Serial.println(calculatedCRC, HEX);

    // ---------------------------------
    // VERIFY
    // ---------------------------------

    bool passed = true;

    for (uint8_t i = 0; i < DATA_LENGTH; i++)
    {
        if (writeBuffer[i] != readBuffer[i])
        {
            passed = false;
        }
    }

    if (storedCRC != calculatedCRC)
    {
        passed = false;
    }

    Serial.println();
    Serial.println("==============================");

    if (passed)
    {
        Serial.println("SMART PROBE DETECTION TEST PASSED");
    }
    else
    {
        Serial.println("SMART PROBE DETECTION TEST FAILED");
    }

    Serial.println("==============================");
}

void testSmartProbeStartDetectionData()
{
    Serial.println();
    Serial.println("==============================");
    Serial.println("SMART PROBE START DETECTION DATA TEST");
    Serial.println("==============================");

    const uint8_t DEVICE_ADDRESS = 0xA0;
    const uint8_t START_DETECTION_ADDRESS = 0x40;
    const uint8_t START_DETECTION_LENGTH = 12;

    uint8_t buffer[START_DETECTION_LENGTH];
    uint8_t readBuffer[START_DETECTION_LENGTH];

    memset(buffer, 0, sizeof(buffer));
    memset(readBuffer, 0, sizeof(readBuffer));

    // ---------------------------------
    // SOURCE DATA
    // ---------------------------------

    uint16_t probeID = 1;

    /*
     * Start Detection Data format from smart.c:
     *
     * Byte 0-1   : Probe ID
     * Byte 2-9   : Date/Time information
     * Byte 10-11 : CRC16
     *
     * For this first test we keep date/time = 0.
     */

    buffer[0] = probeID & 0xFF;
    buffer[1] = (probeID >> 8) & 0xFF;

    // Date/time bytes = 0 for initial test
    for (int i = 2; i < 10; i++)
    {
        buffer[i] = 0x00;
    }

    uint16_t crc =
        modbus_crc16(buffer, START_DETECTION_LENGTH - 2);

    buffer[10] = crc & 0xFF;
    buffer[11] = (crc >> 8) & 0xFF;

    Serial.println("SOURCE DATA:");
    Serial.print("Probe ID : ");
    Serial.println(probeID);

    Serial.print("CRC      : 0x");

    if (crc < 0x1000)
        Serial.print("0");

    Serial.println(crc, HEX);

    // ---------------------------------
    // WRITE
    // ---------------------------------

    Serial.println();
    Serial.println("Writing start detection data...");

    for (uint8_t i = 0; i < START_DETECTION_LENGTH; i++)
    {
        eeprom.singleByteWrite(
            DEVICE_ADDRESS,
            START_DETECTION_ADDRESS + i,
            buffer[i]
        );

        delay(10);
    }

    Serial.println("WRITE COMPLETED");

    delay(100);

    // ---------------------------------
    // READ
    // ---------------------------------

    Serial.println();
    Serial.println("Reading start detection data...");

    for (uint8_t i = 0; i < START_DETECTION_LENGTH; i++)
    {
        eeprom.randomRead(
            DEVICE_ADDRESS,
            START_DETECTION_ADDRESS + i,
            &readBuffer[i],
            1
        );

        delay(5);
    }

    // ---------------------------------
    // RAW EEPROM DATA
    // ---------------------------------

    Serial.println();
    Serial.println("EEPROM RAW BYTES:");

    for (uint8_t i = 0; i < START_DETECTION_LENGTH; i++)
    {
        Serial.print("ADDR 0x");

        uint8_t address =
            START_DETECTION_ADDRESS + i;

        if (address < 0x10)
            Serial.print("0");

        Serial.print(address, HEX);

        Serial.print(" = 0x");

        if (readBuffer[i] < 0x10)
            Serial.print("0");

        Serial.println(readBuffer[i], HEX);
    }

    // ---------------------------------
    // CRC CHECK
    // ---------------------------------

    uint16_t storedCRC =
        readBuffer[10] |
        ((uint16_t)readBuffer[11] << 8);

    uint16_t calculatedCRC =
        modbus_crc16(
            readBuffer,
            START_DETECTION_LENGTH - 2
        );

    Serial.println();

    Serial.print("Stored CRC     : 0x");

    if (storedCRC < 0x1000)
        Serial.print("0");

    Serial.println(storedCRC, HEX);

    Serial.print("Calculated CRC : 0x");

    if (calculatedCRC < 0x1000)
        Serial.print("0");

    Serial.println(calculatedCRC, HEX);

    // ---------------------------------
    // VERIFY
    // ---------------------------------

    bool passed = true;

    for (uint8_t i = 0; i < START_DETECTION_LENGTH; i++)
    {
        if (buffer[i] != readBuffer[i])
        {
            passed = false;
        }
    }

    if (storedCRC != calculatedCRC)
    {
        passed = false;
    }

    Serial.println();
    Serial.println("==============================");

    if (passed)
    {
        Serial.println("SMART PROBE START DETECTION TEST PASSED");
    }
    else
    {
        Serial.println("SMART PROBE START DETECTION TEST FAILED");
    }

    Serial.println("==============================");
}

void testSmartProbeRuntimeData()
{
    Serial.println();
    Serial.println("==============================");
    Serial.println("SMART PROBE RUNTIME DATA TEST");
    Serial.println("==============================");

    const uint8_t DEVICE_ADDRESS = 0xA0;
    const uint8_t RUNTIME_ADDRESS = 0x50;
    const uint8_t RUNTIME_LENGTH = 8;

    uint8_t buffer[8];
    uint8_t readBuffer[8];

    memset(buffer, 0, sizeof(buffer));
    memset(readBuffer, 0, sizeof(readBuffer));

    // -------------------------------------------------
    // SOURCE DATA
    // -------------------------------------------------

    uint16_t probeID = 1;

    // Simulate that 25 pulses have already been applied
    uint16_t pulseCount = 25;

    // 0x00 = Probe OK / Not expired / Not used
    uint16_t probeStatus = 0;

    // -------------------------------------------------
    // BUILD RUNTIME DATA
    // -------------------------------------------------

    put_u16(&buffer[0], probeID);
    put_u16(&buffer[2], pulseCount);
    put_u16(&buffer[4], probeStatus);

    uint16_t crc =
        modbus_crc16(buffer, RUNTIME_LENGTH - 2);

    put_u16(&buffer[6], crc);

    Serial.println("SOURCE DATA:");

    Serial.print("Probe ID      : ");
    Serial.println(probeID);

    Serial.print("Pulse Count   : ");
    Serial.println(pulseCount);

    Serial.print("Probe Status  : 0x");
    Serial.println(probeStatus, HEX);

    Serial.print("CRC           : 0x");
    Serial.println(crc, HEX);

    // -------------------------------------------------
    // WRITE
    // -------------------------------------------------

    Serial.println();
    Serial.println("Writing runtime data...");

    eeprom.eepromWrite(
        DEVICE_ADDRESS,
        RUNTIME_ADDRESS,
        buffer,
        RUNTIME_LENGTH
    );

    delay(20);

    Serial.println("WRITE COMPLETED");

    // -------------------------------------------------
    // READ
    // -------------------------------------------------

    Serial.println();
    Serial.println("Reading runtime data...");

    eeprom.randomRead(
        DEVICE_ADDRESS,
        RUNTIME_ADDRESS,
        readBuffer,
        RUNTIME_LENGTH
    );

    // -------------------------------------------------
    // RAW EEPROM BYTES
    // -------------------------------------------------

    Serial.println();
    Serial.println("EEPROM RAW BYTES:");

    for (uint8_t i = 0; i < RUNTIME_LENGTH; i++)
    {
        Serial.print("ADDR 0x");

        if ((RUNTIME_ADDRESS + i) < 0x10)
            Serial.print("0");

        Serial.print(
            RUNTIME_ADDRESS + i,
            HEX
        );

        Serial.print(" = 0x");

        if (readBuffer[i] < 0x10)
            Serial.print("0");

        Serial.println(
            readBuffer[i],
            HEX
        );
    }

    // -------------------------------------------------
    // DECODE
    // -------------------------------------------------

    uint16_t readProbeID =
        get_u16(&readBuffer[0]);

    uint16_t readPulseCount =
        get_u16(&readBuffer[2]);

    uint16_t readProbeStatus =
        get_u16(&readBuffer[4]);

    uint16_t storedCRC =
        get_u16(&readBuffer[6]);

    uint16_t calculatedCRC =
        modbus_crc16(
            readBuffer,
            RUNTIME_LENGTH - 2
        );

    Serial.println();

    Serial.print("Read Probe ID      : ");
    Serial.println(readProbeID);

    Serial.print("Read Pulse Count   : ");
    Serial.println(readPulseCount);

    Serial.print("Read Probe Status  : 0x");
    Serial.println(readProbeStatus, HEX);

    Serial.print("Stored CRC         : 0x");
    Serial.println(storedCRC, HEX);

    Serial.print("Calculated CRC     : 0x");
    Serial.println(calculatedCRC, HEX);

    // -------------------------------------------------
    // VERIFY
    // -------------------------------------------------

    bool passed = true;

    if (readProbeID != probeID)
        passed = false;

    if (readPulseCount != pulseCount)
        passed = false;

    if (readProbeStatus != probeStatus)
        passed = false;

    if (storedCRC != calculatedCRC)
        passed = false;

    Serial.println();
    Serial.println("==============================");

    if (passed)
    {
        Serial.println(
            "SMART PROBE RUNTIME DATA TEST PASSED"
        );
    }
    else
    {
        Serial.println(
            "SMART PROBE RUNTIME DATA TEST FAILED"
        );
    }

    Serial.println("==============================");
}

void testReadProbeFromAT21CS01()
{
    Serial.println();
    Serial.println("======================================");
    Serial.println("AT21CS01 PROBE READ TEST");
    Serial.println("======================================");

    Device device;

    // We are testing Probe-1
    device.id = "1";

    Serial.println("Reading Probe-1 from AT21CS01...");

    if (!readProbeFromAT21CS01(device))
    {
        Serial.println("AT21CS01 PROBE READ FAILED");
        Serial.println("======================================");
        return;
    }

    Serial.println();
    Serial.println("READ DATA FROM AT21CS01:");
    Serial.print("Probe ID          : ");
    Serial.println(device.id);

    Serial.print("Probe Name        : ");
    Serial.println(device.probeName);

    Serial.print("Pulse Strategy    : ");
    Serial.println(device.pulseStrategy);

    Serial.print("Pulse Count       : ");
    Serial.println(device.totalPulse);

    Serial.print("Cycle Count       : ");
    Serial.println(device.totalCycle);

    Serial.print("Expiry Year       : ");
    Serial.println(device.expiryYear);

    Serial.print("Use Time          : ");
    Serial.println(device.useTime);

    Serial.println();
    Serial.println("======================================");
    Serial.println("AT21CS01 PROBE READ TEST PASSED");
    Serial.println("======================================");
}

void testProbe1CompleteInitialization()
{
    Serial.println();
    Serial.println("======================================");
    Serial.println("PROBE-1 COMPLETE INITIALIZATION TEST");
    Serial.println("======================================");

    const uint8_t DEVICE_ADDRESS = 0xA0;

    const uint8_t CONFIG_ADDRESS  = 0x10;
    const uint8_t DETECT_ADDRESS  = 0x30;
    const uint8_t START_ADDRESS   = 0x40;
    const uint8_t RUNTIME_ADDRESS = 0x50;

    const uint8_t CONFIG_LENGTH  = 28;
    const uint8_t EVENT_LENGTH   = 12;
    const uint8_t RUNTIME_LENGTH = 8;

    uint8_t buffer[28];

    // =================================================
    // 1. PROBE CONFIGURATION
    // =================================================

    memset(buffer, 0, sizeof(buffer));

    uint16_t probeID       = 1;
    uint16_t pulseCount    = 80;
    uint16_t cycleCount    = 8;
    uint16_t startThreshold = 0;
    uint16_t endThreshold   = 80;
    uint32_t endTime        = 900;
    uint32_t expiryTime     = 3600;

    put_u16(&buffer[0], probeID);
    put_u16(&buffer[2], pulseCount);
    put_u16(&buffer[4], cycleCount);
    put_u16(&buffer[6], startThreshold);
    put_u16(&buffer[8], endThreshold);

    // 32-bit values
    buffer[10] = endTime & 0xFF;
    buffer[11] = (endTime >> 8) & 0xFF;
    buffer[12] = (endTime >> 16) & 0xFF;
    buffer[13] = (endTime >> 24) & 0xFF;

    buffer[14] = expiryTime & 0xFF;
    buffer[15] = (expiryTime >> 8) & 0xFF;
    buffer[16] = (expiryTime >> 16) & 0xFF;
    buffer[17] = (expiryTime >> 24) & 0xFF;

    // Creation time = 0 for this initialization test
    for (int i = 18; i < 26; i++)
        buffer[i] = 0;

    uint16_t configCRC =
        modbus_crc16(buffer, CONFIG_LENGTH - 2);

    put_u16(&buffer[26], configCRC);

    Serial.println();
    Serial.println("CONFIGURATION:");
    Serial.print("Probe ID          : ");
    Serial.println(probeID);

    Serial.print("Pulse Count       : ");
    Serial.println(pulseCount);

    Serial.print("Cycle Count       : ");
    Serial.println(cycleCount);

    Serial.print("Start Threshold   : ");
    Serial.println(startThreshold);

    Serial.print("End Threshold     : ");
    Serial.println(endThreshold);

    Serial.print("End Time          : ");
    Serial.println(endTime);

    Serial.print("Expiry Time       : ");
    Serial.println(expiryTime);

    Serial.print("CRC               : 0x");
    Serial.println(configCRC, HEX);

    Serial.println();
    Serial.println("Writing configuration...");

    eeprom.eepromWrite(
        DEVICE_ADDRESS,
        CONFIG_ADDRESS,
        buffer,
        CONFIG_LENGTH
    );

    delay(20);

    Serial.println("CONFIGURATION WRITE COMPLETED");

    // =================================================
    // 2. DETECTION DATA = EMPTY
    // =================================================

    memset(buffer, 0, sizeof(buffer));

    put_u16(&buffer[0], probeID);

    uint16_t detectionCRC =
        modbus_crc16(buffer, EVENT_LENGTH - 2);

    put_u16(&buffer[10], detectionCRC);

    eeprom.eepromWrite(
        DEVICE_ADDRESS,
        DETECT_ADDRESS,
        buffer,
        EVENT_LENGTH
    );

    delay(20);

    Serial.println("DETECTION DATA INITIALIZED");

    // =================================================
    // 3. START DETECTION DATA = EMPTY
    // =================================================

    memset(buffer, 0, sizeof(buffer));

    put_u16(&buffer[0], probeID);

    uint16_t startCRC =
        modbus_crc16(buffer, EVENT_LENGTH - 2);

    put_u16(&buffer[10], startCRC);

    eeprom.eepromWrite(
        DEVICE_ADDRESS,
        START_ADDRESS,
        buffer,
        EVENT_LENGTH
    );

    delay(20);

    Serial.println("START DETECTION DATA INITIALIZED");

    // =================================================
    // 4. RUNTIME DATA = INITIAL STATE
    // =================================================

    memset(buffer, 0, sizeof(buffer));

    put_u16(&buffer[0], probeID);
    put_u16(&buffer[2], 0);       // Applied pulses
    put_u16(&buffer[4], 0);       // Status

    uint16_t runtimeCRC =
        modbus_crc16(buffer, RUNTIME_LENGTH - 2);

    put_u16(&buffer[6], runtimeCRC);

    eeprom.eepromWrite(
        DEVICE_ADDRESS,
        RUNTIME_ADDRESS,
        buffer,
        RUNTIME_LENGTH
    );

    delay(20);

    Serial.println("RUNTIME DATA INITIALIZED");

    // =================================================
    // COMPLETE
    // =================================================

    Serial.println();
    Serial.println("======================================");
    Serial.println("PROBE-1 INITIALIZATION WRITE PASSED");
    Serial.println("======================================");
}

void loop()
{
    server.handleClient();
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi Lost... Reconnecting");
        WiFi.disconnect();
        WiFi.begin(ssid, password);

        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(".");
        }
        Serial.println();
        Serial.println("Reconnected");
    }
    delay(50);
}