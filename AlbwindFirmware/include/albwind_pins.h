#pragma once

#include <Adafruit_NeoPixel.h>

namespace albwind
{
//General configuration    
const int T_SMP         = 1000;
const int DEBOUNCE_TIME = 20;

//Sample pins for 9 pin connector
const int PIN_OIL_SENSE     = 15;
const int PIN_COOL_SENSE    = 16;
const int PIN_LIMA_SENSE    = 18;
const int PIN_GLOW_SENSE    = 17;

//Functional pins to controll alarms
const int PIN_BUZZER    = 7;
const int PIN_ALARM_TEST= 14;
const int PIN_NEOPIXEL  = 4;

//Enum to select right neopixel led
enum LED_INDEX{
    
    LED_GLOW    = 0,
    LED_OIL     = 1,
    LED_COOL    = 2,
    LED_LIMA    = 3
};

//Compare threshold for 9 pin connector
const int OIL_SENSE_THRESHOLD   = 250;
const int COOL_SENSE_THRESHOLD  = 521;
const int LIMA_SENSE_THRESHOLD   = 600;
const int low_LIMA_SENSE_THRESHOLD = 100;
const int GLOW_SENSE_THRESHOLD  = 512;

} // namespace albwind
