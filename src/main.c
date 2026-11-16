#include "pico/stdlib.h"
#include "sensor/sht40.h"
#include <stdio.h>



//Test for CRC8 function
void test_crc8() {
    const uint8_t data[] = {0xBE, 0xEF};
    uint8_t crc = sht40_crc8(data, sizeof(data));
    printf("CRC8: 0x%02X\n", crc);
}
void test_parse() {
    const uint8_t data[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    float temp_c;
    float rh;
    bool parse = sht40_parse(data, &temp_c, &rh);
    printf("Parse: %d, Temp: %.2f, RH: %.2f\n", parse, temp_c, rh);
}

int main() {
    stdio_init_all();
    test_crc8();
    test_parse();
    

    for (;;) {
        printf("Hello world!\n");
        sleep_ms(1000);
    }
}