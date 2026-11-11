#include "sensor/sht40.h"



// Calculate CRC8 for SHT40 sensor data
uint8_t sht40_crc8(const uint8_t* data, size_t length){

    uint8_t crc = 0xFF;
    for (uint i = 0; i < length; i++) {
        crc = crc ^ data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ 0x31;
            } else {
                crc = crc << 1;
            }
        }
    }
    return crc;
}