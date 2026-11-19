#include "pico/stdlib.h"
#include <stdio.h>
#include "display/lcd.h"
#include "sensor/sht40.h"

int main() {
    stdio_init_all();
    sleep_ms(3000); 
    init_spi_lcd();
    printf("step 1\n");
    printf("Starting SHT40 sensor and LCD display demo...\n");
    LCD_Setup();
    printf("step 2\n");
    printf("LCD setup complete. Clearing screen and initializing SHT40 sensor...\n");
    LCD_Clear(BLACK);
    printf("step 3\n");
    printf("Screen cleared. Initializing SHT40 sensor...\n");
    sht40_init();
    printf("step 4\n");
    printf("SHT40 sensor initialized. Entering main loop to read temperature and humidity...\n");

    for (;;) {
        printf("entering loop\n");
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