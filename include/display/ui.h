#ifndef UI_H
#define UI_H

#include "led/ws2812.h"
#include <stdint.h>

void ui_init(void);
void ui_show_readings(float temp_c, float rh);
void ui_show_sensor_error(void);
void ui_show_alarm(const char *name, sensor_status_t status);

void ui_show_light(uint16_t light_raw);          // raw ADC value
void ui_show_status(const char *text, uint16_t color); // one-line system status

#endif 