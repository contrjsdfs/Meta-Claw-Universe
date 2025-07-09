#include "ws2812.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LED_COUNT 60

void set_led_color(unsigned char *buffer, int led, unsigned char r, unsigned char g, unsigned char b) {
    if (led < 0 || led >= LED_COUNT) return;

    int offset = led * 24;
    for (int i = 0; i < 8; i++) {
        buffer[offset + i] = (g & (1 << (7-i))) ? 0xFC : 0xC0;
        buffer[offset + 8 + i] = (r & (1 << (7-i))) ? 0xFC : 0xC0;
        buffer[offset + 16 + i] = (b & (1 << (7-i))) ? 0xFC : 0xC0;
    }
}

void set_all_leds(unsigned char *buffer, unsigned char r, unsigned char g, unsigned char b) {
    for (int i = 0; i < LED_COUNT; i++) {
        set_led_color(buffer, i, r, g, b);
    }
}
