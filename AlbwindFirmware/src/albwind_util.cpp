#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "albwind_util.h"
#include "albwind_pins.h"

namespace albwind
{

    //Stack objects
    Adafruit_NeoPixel neopixel;

    int num_alerts = 0;
    bool alarmTest = false;

    void turn_on_buzzer()
    {
        digitalWrite(PIN_BUZZER, HIGH);
    }

    void turn_off_buzzer()
    {
        digitalWrite(PIN_BUZZER, LOW);
    }

    void write_led(LED_INDEX led_index, uint32_t c)
    {
        neopixel.setPixelColor(led_index, c);
        neopixel.show();
    }

    void set_alert(Alert & alert, LED_INDEX alert_led, bool state, bool with_buzz = true)
    {
        if (state == true) // Turn on alert
        {
            if(alert.active == true){
                Serial.println("Retundant alert activation");
                return;
            }

            alert.active = true;
            num_alerts++;
            if(with_buzz)
                turn_on_buzzer();
            neopixel.setPixelColor(alert_led, albwind::COLOR_RED);
            neopixel.show();

        }

        if (state == false) // turn on alert
        {
            if(alert.active == false ) {
                Serial.println("Retundant alert deactivation");
                return;
            }
            alert.active = false;
            num_alerts--;
            neopixel.setPixelColor(alert_led, albwind::COLOR_NONE);
            neopixel.show();

            if(num_alerts <= 0) {
                num_alerts = 0;
                turn_off_buzzer();
            }
        }
    }

    void confirm_alert() {
        if(num_alerts != 0) {
            turn_off_buzzer();
            Serial.println("Alarm confirmed");
        }
        else  {
            if(alarmTest) return;
            Serial.println("Systemcheck activated");
            alarmTest = true;
            turn_on_buzzer();
            neopixel.setPixelColor(albwind::LED_LIMA, albwind::COLOR_WHITE);
            neopixel.setPixelColor(albwind::LED_GLOW, albwind::COLOR_WHITE);
            neopixel.setPixelColor(albwind::LED_OIL, albwind::COLOR_WHITE);
            neopixel.setPixelColor(albwind::LED_COOL, albwind::COLOR_WHITE);
            neopixel.show();

            delay(500);
            turn_off_buzzer();

            neopixel.setPixelColor(albwind::LED_LIMA, albwind::COLOR_YELLOW);
            neopixel.setPixelColor(albwind::LED_GLOW, albwind::COLOR_YELLOW);
            neopixel.setPixelColor(albwind::LED_OIL, albwind::COLOR_YELLOW);
            neopixel.setPixelColor(albwind::LED_COOL, albwind::COLOR_YELLOW);
            neopixel.show();

            delay(500);
            neopixel.setPixelColor(albwind::LED_LIMA, albwind::COLOR_RED);
            neopixel.setPixelColor(albwind::LED_GLOW, albwind::COLOR_RED);
            neopixel.setPixelColor(albwind::LED_OIL, albwind::COLOR_RED);
            neopixel.setPixelColor(albwind::LED_COOL, albwind::COLOR_RED);
            neopixel.show();

            delay(500);

            neopixel.setPixelColor(albwind::LED_LIMA, albwind::COLOR_NONE);
            neopixel.setPixelColor(albwind::LED_GLOW, albwind::COLOR_NONE);
            neopixel.setPixelColor(albwind::LED_OIL, albwind::COLOR_NONE);
            neopixel.setPixelColor(albwind::LED_COOL, albwind::COLOR_NONE);
            neopixel.show();

            delay(500);

            neopixel.setPixelColor(albwind::LED_LIMA, albwind::COLOR_GREEN);
            neopixel.setPixelColor(albwind::LED_GLOW, albwind::COLOR_GREEN);
            neopixel.setPixelColor(albwind::LED_OIL, albwind::COLOR_GREEN);
            neopixel.setPixelColor(albwind::LED_COOL, albwind::COLOR_GREEN);
            neopixel.show();
            turn_on_buzzer();

            delay(250);
            neopixel.setPixelColor(albwind::LED_LIMA, albwind::COLOR_NONE);
            neopixel.setPixelColor(albwind::LED_GLOW, albwind::COLOR_NONE);
            neopixel.setPixelColor(albwind::LED_OIL, albwind::COLOR_NONE);
            neopixel.setPixelColor(albwind::LED_COOL, albwind::COLOR_NONE);
            neopixel.show();
            turn_off_buzzer();
            Serial.println("Systemcheck finished");

            alarmTest = false;
        }
    }

} // namespace albwind
