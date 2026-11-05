#include "led/ws2812.h"
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"

#define WS_BITRATE 800000
#define WS_LEDCOUNT 4
#define FLASH_INTERVAL 500000
#define RESET_INTERVAL 100
// brightness
#define GREEN_LEVEL 50
#define RED_LEVEL 50
#define BLUE_LEVEL 50
#define AMBER_RED_LEVEL 50
#define AMBER_GREEN_LEVEL 50

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_t;

#define COLOR_OFF ((rgb_t){0, 0, 0})
#define COLOR_GREEN ((rgb_t){0, GREEN_LEVEL, 0})
#define COLOR_RED ((rgb_t){RED_LEVEL, 0, 0})
#define COLOR_BLUE ((rgb_t){0, 0, BLUE_LEVEL})
#define COLOR_AMBER ((rgb_t){AMBER_RED_LEVEL, AMBER_GREEN_LEVEL, 0})

static PIO led_pio;
static uint led_sm;
static bool initialized = 0;
static bool frame_dirty = 0;

static ws2812_status_t current_status = {
    .system = SYSTEM_OPERATIONAL,
    .temperature = SENSOR_NORMAL,
    .humidity = SENSOR_NORMAL,
    .light = SENSOR_NORMAL
};

static bool blink_on = 0;
static uint64_t last_toggle_us = 0;

static rgb_t get_sensor_color(sensor_status_t status, bool flash_state);

// helper functions

static bool sensor_has_alarm(sensor_status_t status) {
    return (status == SENSOR_HIGH_ALARM || status == SENSOR_LOW_ALARM);
}

static bool any_sensor_alarm(void) {
    return sensor_has_alarm(current_status.temperature) || sensor_has_alarm(current_status.humidity) || sensor_has_alarm(current_status.light);
}

static bool any_sensor_error(void) {
    return current_status.temperature == SENSOR_ERROR || current_status.humidity == SENSOR_ERROR || current_status.light == SENSOR_ERROR;
}

// pio for pixel output
static inline void put_pixel(PIO pio, uint sm, uint32_t pixel_grb) {
    // need to shift by 8 because we have 24 bits of data but have to push in 32 bits
    for (int i = 0; i <= 4; i++)
    {
        pio_sm_put_blocking(pio, sm, pixel_grb << 8u);
    }
}

static void transmit_frame(void) {
    rgb_t pixels[WS_LEDCOUNT];
    // led 1 sys status
    if (current_status.system == SYSTEM_ERROR || any_sensor_error()) {
        pixels[0] = COLOR_AMBER;
    } else {
        pixels[0] = COLOR_GREEN;
    }

    // led 2-4 temp, humidity, light
    pixels[1] = get_sensor_color(current_status.temperature, blink_on);
    pixels[2] = get_sensor_color(current_status.humidity, blink_on);
    pixels[3] = get_sensor_color(current_status.light, blink_on);

    for (int i = 0; i < WS_LEDCOUNT; i++) {
        put_pixel(led_pio, led_sm, (pixels[i].r << 16) | (pixels[i].g << 8) | pixels[i].b);
    }

    sleep_us(RESET_INTERVAL);
}

// color mapping
static rgb_t get_sensor_color(sensor_status_t status, bool flash_state) {
    switch (status) {
        case SENSOR_NORMAL:
            return COLOR_GREEN;

        case SENSOR_LOW_WARNING:
            return COLOR_BLUE;
        
        case SENSOR_HIGH_WARNING:
            return COLOR_RED;
        
        case SENSOR_LOW_ALARM:
            return flash_state ? COLOR_BLUE : COLOR_OFF;

        case SENSOR_HIGH_ALARM:
            return flash_state ? COLOR_RED : COLOR_OFF;

        case SENSOR_ERROR:
            return COLOR_AMBER;
        
        default:
            return COLOR_OFF;
    }
}

// led interactions
bool ws2812_init(PIO pio, uint sm, uint data_pin) {
    if (initialized) {
        return false;
    }

    led_pio = pio;
    led_sm = sm;

    ws2812_program_init(led_pio, led_sm, pio_add_program(led_pio, &ws2812_program), data_pin, WS_BITRATE, false);

    initialized = true;
    return true;
}

void ws2812_set_status(const ws2812_status_t *status) {
    current_status = *status;
    frame_dirty = true;
}

void ws2812_update(void) {
    if (!initialized) {
        return;
    }

// highkey idk how to do ts um
}