#include "pico/stdlib.h"
#include <stdio.h>
#include "display/lcd.h"
#include "sensor/sht40.h"

int main() {
    stdio_init_all();
    init_spi_lcd();
    LCD_Setup();
    LCD_Clear(BLACK);
    sht40_init();

    for (;;) {
        float t, rh;
        char line[32];
        if (sht40_read(&t, &rh)) {
            snprintf(line, sizeof line, "Temp: %.1f C  ", t);
            LCD_DrawString(10, 20, WHITE, BLACK, line, 16, 0);
            snprintf(line, sizeof line, "RH:   %.1f %%  ", rh);
            LCD_DrawString(10, 50, WHITE, BLACK, line, 16, 0);
        } else {
            LCD_DrawString(10, 20, RED, BLACK, "Sensor error   ", 16, 0);
        }
        sleep_ms(1000);
    }
}