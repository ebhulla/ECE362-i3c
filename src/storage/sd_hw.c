#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "pins.h"

static spi_inst_t * sd = spi0;
static bool spi_initialized = false;

void init_spi_sdcard(void)
{
    if (spi_initialized) {
        return;
    }

    gpio_init(SD_CS);
    gpio_set_dir(SD_CS, GPIO_OUT);
    gpio_put(SD_CS, 1);

    gpio_set_function(SD_MISO, GPIO_FUNC_SPI);
    gpio_set_function(SD_SCK, GPIO_FUNC_SPI);
    gpio_set_function(SD_MOSI, GPIO_FUNC_SPI);

    spi_init(sd, 4000000);
    spi_set_format(sd, 8, 0, 0, SPI_MSB_FIRST);

    spi_initialized = true;

}

void disable_sdcard(void)
{
    gpio_put(SD_CS, 1);
}

void enable_sdcard(void)
{
    gpio_put(SD_CS, 0);
}

void sdcard_io_high_speed(void)
{
    spi_set_baudrate(sd, 12000000);
}

void init_sdcard_io(void)
{
    init_spi_sdcard();
    disable_sdcard();
}

/*
keypad: 2-9

lcd skibidi
// 25: spi1_cs (1)
// 26: spi1_sck (4)
// 27: spi1_tx (2)
//28 spi1_rx (3)


17: spi0_cs (3)
20: any gpio (4)
21: any gpio (5)
19: spi0_tx (6)
18: spi0_sck (7)

ws2812: 29

light sensor
15: i2c1 scl
16: i2c1 sda

sht40
32: i2c0 sda
33: i2c0 scl
*/