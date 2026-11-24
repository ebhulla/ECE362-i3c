#include "pico/stdlib.h"
#include <stdio.h>
#include "display/lcd.h"
#include "sensor/sht40.h"
#include "display/ui.h"

//static const uint pins[] = {17, 18, 19, 20, 21};
//static const char *names[] = {"CS", "SCK", "MOSI", "DC", "RESET"};


int main() {

    // stdio_init_all();
    // sleep_ms(3000);

    // for(int i = 0; i < 5; i++){
    //     gpio_init(pins[i]);
    //     gpio_set_dir(pins[i],true);
    //     gpio_put(pins[i],0);
    // }

    // for(;;){
    //     for (int i = 0; i < 5; i++){
    //         printf("name is %s and pin is %d",names[i],pins[i]);
    //         gpio_put(pins[i],1);
    //         sleep_ms(3000);
    //         gpio_put(pins[i],0);
    //     }
    // }

    stdio_init_all();
    sleep_ms(3000); 
    ui_init();
    sht40_init();
    printf("step 4\n");
    printf("SHT40 sensor initialized. Entering main loop to read temperature and humidity...\n");

    for (;;) {
        printf("entering loop\n");
        float t, rh;
        // char line[32];
        sht40_status_t St = sht40_read(&t,&rh);
        if (St == SHT40_OK) {
            printf("T=%.2f C RH = %.2f%%\n",t,rh);
            //snprintf(line, sizeof line, "Temp: %.1f C  ", t);
            ui_show_readings(t,rh);
            //snprintf(line, sizeof line, "RH:   %.1f %%  ", rh);
            
        } else {
            printf("sht40_read failed, status=%d\n", St);
            ui_show_sensor_error();
        }
        sleep_ms(1000);
    }


}