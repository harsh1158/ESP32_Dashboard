#ifndef CRC_H
#define CRC_H

#include <Arduino.h>

static uint16_t modbus_crc16(const uint8_t *data, uint16_t length)
{
    uint16_t crc = 0xFFFF;

    for (uint16_t i = 0; i < length; i++)
    {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++)
        {
            if (crc & 0x0001)
                crc = (crc >> 1) ^ 0xA001;
            else
                crc >>= 1;
        }
    }
    return crc;
}

static void put_u16(uint8_t *buffer, uint16_t value)
{
    buffer[0] = value & 0xFF;
    buffer[1] = (value >> 8) & 0xFF;
}

static uint16_t get_u16(const uint8_t *buffer)
{
    return ((uint16_t)buffer[1] << 8) | buffer[0];
}

#endif