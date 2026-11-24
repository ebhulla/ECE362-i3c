#ifndef SHT40_H
#define SHT40_H
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "hardware/i2c.h"

typedef enum {
    SHT40_OK,
    SHT40_ERR_NO_ACK,   // write got no ACK or timed out
    SHT40_ERR_READ,     // read failed or returned the wrong number of bytes
    SHT40_ERR_CRC       // data arrived but a checksum was wrong
} sht40_status_t;

sht40_status_t sht40_read(float *temp_c, float *rh);

uint8_t sht40_crc8(const uint8_t* data, size_t length);
float sht40_raw_to_celsius(uint16_t raw);
float sht40_raw_to_rh(uint16_t raw);
bool  sht40_parse(const uint8_t buf[6], float *temp_c, float *rh);
void sht40_init(void);
void sht40_run_tests(void);

#endif