#include <stdio.h>
#include "display/ui.h"
#include "display/lcd.h"

#define ROW_TITLE 10
#define ROW_TEMP 50
#define ROW_RH 90
#define ROW_ALARM 200
#define ALARM_H 30
#define ROW_LIGHT 130
#define ROW_STATUS 280


void ui_init(void){
    init_spi_lcd();
    LCD_Setup();
    LCD_Clear(BLACK);
    LCD_DrawString(10,ROW_TITLE,CYAN,BLACK,"Environment Monitor",16,0);
}

void ui_show_readings(float temp_c,float rh){
    char line[20];
    snprintf(line,sizeof(line),"Temp: %5.1f C   ",temp_c);
    LCD_DrawString(10,ROW_TEMP,WHITE,BLACK,line,16,0);
    snprintf(line,sizeof(line),"RH:   %5.1f %%   ",rh);
    LCD_DrawString(10,ROW_RH,WHITE,BLACK,line,16,0);
}


void ui_show_sensor_error(void){
    LCD_DrawString(10,ROW_TEMP,RED,BLACK,"TEMP: ---sensor error---",16,0);
    LCD_DrawString(10,ROW_RH,RED,BLACK,"RH:   ---sensor error---",16,0);
}

//Draws or clears the alarm banner for one sensor (Temp, RH , Light)
void ui_show_alarm(const char *name, sensor_status_t status){
    char line[32];
    uint16_t bg = BLACK;
    uint16_t fc = WHITE;
    const char *what = ""; // what to display in the banner

    switch(status){
        case SENSOR_HIGH_ALARM:
            bg = RED;
            what = "HIGH ALARM";
            break;
        case SENSOR_LOW_ALARM:
            bg = BLUE;
            what = "LOW ALARM";
            break;
        case SENSOR_HIGH_WARNING:
            bg = YELLOW;
            what = "HIGH WARNING";
            break;
        case SENSOR_LOW_WARNING:
            bg = YELLOW;
            what = "LOW WARNING";
            break;
        case SENSOR_ERROR:
            bg = RED;
            what = "SENSOR ERROR";
            break;
        default: break;
    }
    LCD_DrawFillRectangle(0,ROW_ALARM,LCD_W - 1,ROW_ALARM + ALARM_H,bg);
    if(status != SENSOR_NORMAL){
        snprintf(line,sizeof(line),"%s: %s",name,what);
        LCD_DrawString(10,ROW_ALARM + 7,fc,bg,line,16,1); //1 is for transparent background, 0 is for solid background
    }

}

void ui_show_light(uint16_t light_raw){
    char buff[32];
    snprintf(buff,sizeof(buff),"Light: %4u",light_raw);
    LCD_DrawString(10,ROW_LIGHT,WHITE,BLACK,buff,16,0);
}

void ui_show_status(const char *text, uint16_t color){
    LCD_DrawFillRectangle(0,ROW_STATUS,LCD_W - 1,ROW_STATUS+16,BLACK);

    LCD_DrawString(0,ROW_STATUS,WHITE,color,text,16,0);

}