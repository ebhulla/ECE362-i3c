#ifndef WS2812_H
#define WS2812_H

#include <stdbool.h>
#include "hardware/pio.h"

typedef enum { // LED 2, 3, 4, temp, humidity, light
    SENSOR_NORMAL,
    SENSOR_HIGH_WARNING,
    SENSOR_LOW_WARNING,
    SENSOR_HIGH_ALARM,
    SENSOR_LOW_ALARM,
    SENSOR_ERROR
} sensor_status_t;

typedef enum { // LED 1 system
    SYSTEM_OPERATIONAL,
    SYSTEM_ERROR
} system_status_t;

typedef struct {
    system_status_t system;
    sensor_status_t temperature;
    sensor_status_t humidity;
    sensor_status_t light;
} ws2812_status_t;

bool ws2812_init(PIO pio, uint sm, uint data_pin); // pio state machine and data
void ws2812_set_status(const ws2812_status_t *status);
void ws2812_update(void);

#endif