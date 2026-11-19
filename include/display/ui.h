#ifndef UI_H
#define UI_H

#include "led/ws2812.h"

void ui_init(void);
void ui_show_readings(float temp_c, float rh);
void ui_show_sensor_error(void);
void ui_show_alarm(const char *name, sensor_status_t status);

#endif 