#ifndef WS2812_H
#define WS2812_H

void set_led_color(unsigned char *buffer, int led, unsigned char r, unsigned char g, unsigned char b);
void set_all_leds(unsigned char *buffer, unsigned char r, unsigned char g, unsigned char b);


#endif // WS2812_H
