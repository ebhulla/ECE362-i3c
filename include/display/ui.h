#ifndef UI_H
#define UI_H

#include "led/ws2812.h"
#include <stdint.h>

typedef enum { 
    
    UI_PAGE_READINGS,  //Temperature, Humdity and light
    UI_PAGE_THRESHOLDS, //Six alarm limits
    UI_PAGE_STATUS //System Status heading

} ui_page_t;

 /*Limits shown on the thresholds page.
 Temperature, Humidity in physical units. Light in raw ADC counts.
 */
typedef struct {
    float temp_low,temp_high;
    float rh_low,rh_high;
    uint16_t light_low,light_high;
} ui_thresholds_t;

void ui_init(void);
void ui_show_readings(float temp_c, float rh);
void ui_show_sensor_error(void);
void ui_show_alarm(const char *name, sensor_status_t status);

void ui_show_light(uint16_t light_raw);          // raw ADC value
void ui_show_status(const char *text, uint16_t color); // one-line system status

void ui_set_page(ui_page_t page);
void ui_show_thresholds(const ui_thresholds_t *th, int selected);

#endif 