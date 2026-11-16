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

float sht40_raw_to_celsius(uint16_t raw){
    float temp = (raw / 65535.0f) * 175.0f - 45.0f;
    if(temp < -40.0f || temp > 125.0f){
        return fminf(fmaxf(temp, -40.0f), 125.0f);
    }
    return temp;
}

float sht40_raw_to_rh(uint16_t raw){
    float rh = (-6.0f) + (raw / 65535.0f) * 125.0f;
    if(rh  < 0 || rh > 100){
        return fminf(fmaxf(rh, 0.0f), 100.0f);
    }
    return rh;
}

bool sht40_parse(const uint8_t buf[6], float *temp_c, float *rh){
    if(temp_c == NULL || rh == NULL) {
        return false;
    }
    if (sht40_crc8(buf, 2) != buf[2] ||
        sht40_crc8(buf + 3, 2) != buf[5]) {
        return false;
    }

    uint16_t raw_temp = ((uint16_t)buf[0] << 8) | buf[1];
    uint16_t raw_rh = ((uint16_t)buf[3] << 8) | buf[4];
    *temp_c = sht40_raw_to_celsius(raw_temp);
    *rh = sht40_raw_to_rh(raw_rh);
    return true;
}

