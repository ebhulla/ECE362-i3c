#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "queue.h"

#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

//////////////////////////////////////////////////////////////////////////////

#define IS_RGBW false
#define NUM_PIXELS 150

#define WS2812_PIN 28 // set to be the pin 

// Check the pin is compatible with the platform
#if WS2812_PIN >= NUM_BANK0_GPIOS
#error Attempting to use a pin>=32 on a platform that does not support it
#endif

// #define ALTERNATING
// #define FADE
#define RING_COUNTER

//////////////////////////////////////////////////////////////////////////////

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b, int brightness);

//////////////////////////////////////////////////////////////////////////////

// function to put the pixel data in 
static inline void put_pixel(PIO pio, uint sm, uint32_t pixel_grb, int led_num) {
    // need to shift by 8 because we have 24 bits of data but have to push in 32 bits
    for (int i = 0; i <= led_num; i++)
    {
        pio_sm_put_blocking(pio, sm, pixel_grb << 8u);
    }
}

static inline void pixel_alternating(PIO pio, uint sm, uint32_t pixel_grb1, uint32_t pixel_grb2, int led_num)
{
    for (int i = 0; i <= led_num ; i++){
        if (i % 2 == 0)
        {
            pio_sm_put_blocking(pio, sm, pixel_grb1 << 8u);
        }
        else
        {
            // uint8_t r = (pixel_grb & 0xFF00) >> 8;
            // uint8_t g = (pixel_grb & 0xFF0000) >> 16;
            // uint8_t b = (pixel_grb & 0xFF); 
            // g = g >> 4;
            // b = b >> 2;
            // uint32_t new_rgb = urgb_u32(r, g, b);
            pio_sm_put_blocking(pio, sm, pixel_grb2 << 8u);
        }
        
    }
}

static inline void reset_leds(PIO pio, uint sm, int led_num)
{   
    uint32_t pixel_grb = urgb_u32(100, 100, 100, 0);

    for(int i = 0; i <= led_num; i++)
    {
        pio_sm_put_blocking(pio, sm, pixel_grb << 8u);
    }
}
static inline void ring_counter(PIO pio, uint sm, uint32_t pixel_grb, int led_num)
{   
    uint32_t off = urgb_u32(100, 100, 100, 0);
    for (int i = 0; i <= led_num; i++)
    {
        if (i == 0)
        {
            pio_sm_put_blocking(pio, sm, pixel_grb << 8);
        }
        for (int j = 0; j < i; j++)
        {
            if (i == 0)
            {
                printf("%d\n", i);
            }
            pio_sm_put_blocking(pio, sm, off << 8);
        }
        pio_sm_put_blocking(pio, sm, pixel_grb << 8);
        sleep_ms(100);
    }
}

static inline void pixel_fade(PIO pio, uint sm, int r, int g, int b, int led_num)
{
    int brightness = 100;
    uint32_t pixel_grb;
    while (brightness >= 0)
    {
        if (brightness > 0)
        {
            for (int i = 0; i <= led_num; i++)
            {   
                pixel_grb = urgb_u32(r, g, b, brightness);
                pio_sm_put_blocking(pio, sm, pixel_grb << 8u);
            }
            brightness--;
        }
        else if (brightness == 0)
        {
            for (int i = 0; i <= led_num; i++)
            {   
                pixel_grb = urgb_u32(0, 0, 0, 0);
                pio_sm_put_blocking(pio, sm, pixel_grb << 8u);
            }
            printf("Lights out\n");
            brightness--;
            sleep_ms(100);
        }
        sleep_ms(10);
    }
}

// create the 32 bits of data for the color
// replace with the white if needed
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b, int brightness) {
        if (brightness == 0)
        {
            r = 0;
            g = 0;
            b = 0;
        }
        else
        {
            int frac = 100 / brightness;
            r = r / frac;
            g = g / frac;
            b = b / frac;
        }
    return
            ((uint32_t) (r) << 8) |
            ((uint32_t) (g) << 16) |
            (uint32_t) (b);
}


int main()
{
    // Configures our microcontroller to 
    // communicate over UART through the TX/RX pins
    stdio_init_all();

    // starting my PIO test code
    
    printf("WS2812 Test, using pin %d\n", WS2812_PIN);
    fflush(stdout);
    // todo get free sm
    PIO pio;
    uint sm;
    uint offset;

    // This will find a free pio and state machine for our program and load it for us
    // We use pio_claim_free_sm_and_add_program_for_gpio_range (for_gpio_range variant)
    // so we will get a PIO instance suitable for addressing gpios >= 32 if needed and supported by the hardware
    bool success = pio_claim_free_sm_and_add_program_for_gpio_range(&ws2812_program, &pio, &sm, &offset, WS2812_PIN, 1, true);
    fflush(stdout);
    if (success)
    {
        printf("Successfully connected to state machine");
    }
    else
    {
        printf("Unsuccessful attempt at connecting to state machine. Please try again.");
    }

    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

    uint32_t color1 = urgb_u32(255, 51, 153, 25);
    uint32_t color2;
    uint32_t color3;
    while (1) {

        #ifdef ALTERNATING
            printf("WS2812 Test, using pin %d\n", WS2812_PIN);
            color1 = urgb_u32(255, 51, 153, 25);
            color2 = urgb_u32(0, 153, 204, 25);
            color3 = urgb_u32(255, 153, 0, 25);
            //printf("Color is red\n");
            pixel_alternating(pio, sm, color1, color2, 10);
            sleep_ms(5000);
            pixel_alternating(pio, sm, color2, color3, 10);
            gpio_put(22, 0);
            sleep_ms(5000);
        #endif

        #ifdef FADE
            pixel_fade(pio, sm, 255, 51, 153, 10);
        #endif

        #ifdef RING_COUNTER
            reset_leds(pio, sm, 10);
            ring_counter(pio, sm, color1, 10);
        #endif
    }

    // This will free resources and unload our program
    pio_remove_program_and_unclaim_sm(&ws2812_program, pio, sm, offset);

    return 0;
}
