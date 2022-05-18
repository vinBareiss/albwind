#ifndef ALBWIND_UTIL_H
#define ALBWIND_UTIL_H

#include <albwind_pins.h>
/*******************************************************
 * 
 * This file declates functions
 * 
 *******************************************************/

namespace albwind
{
    const uint32_t COLOR_NONE = Adafruit_NeoPixel::Color(0, 0, 0);
    const uint32_t COLOR_RED = Adafruit_NeoPixel::Color(200, 0, 0);
    const uint32_t COLOR_GREEN = Adafruit_NeoPixel::Color(0, 200, 0);
    const uint32_t COLOR_BLUE = Adafruit_NeoPixel::Color(0, 0, 200);
    const uint32_t COLOR_YELLOW = Adafruit_NeoPixel::Color(200, 200, 0);
    const uint32_t COLOR_WHITE = Adafruit_NeoPixel::Color(200, 200, 200);

    struct Alert
    {
        int old_value;
        long int last_edge;
        bool active;
    };

    enum Edge
    {
        Falling,
        Rising,
        Debounce,
        None
    };

    extern Adafruit_NeoPixel neopixel;
    
    void turn_on_buzzer();
    void turn_off_buzzer();
    void write_led(albwind::LED_INDEX led, uint32_t color);
    void set_alert(Alert &,LED_INDEX,bool state, bool with_buzz = true);
    void confirm_alert();
} // namespace albwind

#endif