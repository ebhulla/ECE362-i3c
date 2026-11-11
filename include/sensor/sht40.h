#ifndef SHT40_H
#define SHT40_H
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "hardware/i2c.h"

uint8_t sht40_crc8(const uint8_t* data, size_t length);

#endif