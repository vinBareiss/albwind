#include <Arduino.h>
#include <albwind_pins.h>
#include "albwind_util.h"

//#define DEBUG

using albwind::Alert;
using albwind::Edge;
using albwind::LED_INDEX;

void error_state()
{
  while (1)
  {
    Serial.println("Error state");
  }
}

int num_alert;
Alert oil_alert, COOL_alert, glow_alert, LIMA_alert, test_alert;
Alert lower_LIMA_alert;

void setup()
{
  //Init Alerts:
  test_alert = {.old_value = 1024, .last_edge = 0, .active = false};
  oil_alert = {.old_value = 1024, .last_edge = 0, .active = false};
  COOL_alert = {.old_value = 1024, .last_edge = 0, .active = false};

  glow_alert = {.old_value = 0, .last_edge = 0, .active = false};
  LIMA_alert = {.old_value = 0, .last_edge = 0, .active = false};
  lower_LIMA_alert = {.old_value = 0, .last_edge = 0, .active = false};

  //Init GPIO Pins
  pinMode(albwind::PIN_OIL_SENSE, INPUT_PULLUP);
  pinMode(albwind::PIN_ALARM_TEST, INPUT_PULLUP);
  pinMode(albwind::PIN_COOL_SENSE, INPUT_PULLUP);

  pinMode(albwind::PIN_LIMA_SENSE, INPUT);
  pinMode(albwind::PIN_GLOW_SENSE, INPUT);

  pinMode(albwind::PIN_BUZZER, OUTPUT);

  //Init Neopixel
  albwind::neopixel = Adafruit_NeoPixel(7, albwind::PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);
  albwind::neopixel.begin();
  albwind::neopixel.clear();
  digitalWrite(albwind::PIN_BUZZER, LOW);

  //Hello Blink
  for (size_t i = 0; i < 3; i++)
  {
    albwind::neopixel.setPixelColor(0, albwind::COLOR_GREEN);
    albwind::neopixel.setPixelColor(1, albwind::COLOR_GREEN);
    albwind::neopixel.setPixelColor(2, albwind::COLOR_GREEN);
    albwind::neopixel.setPixelColor(3, albwind::COLOR_GREEN);
    albwind::neopixel.show();
    delay(300);
    albwind::neopixel.clear();
    albwind::neopixel.show();
    delay(300);
  }

//Run Test Neopixel Sequence
#ifdef DEBUG
  albwind::neopixel.clear();
  albwind::neopixel.setPixelColor(0, 0, 0, 100);
  albwind::neopixel.show();
  delay(1000);
  albwind::neopixel.setPixelColor(1, 0, 100, 0);
  albwind::neopixel.show();
  delay(1000);
  albwind::neopixel.setPixelColor(2, 100, 0, 0);
  albwind::neopixel.show();
  delay(1000);
  albwind::neopixel.setPixelColor(3, 100, 100, 100);
  albwind::neopixel.show();

  digitalWrite(albwind::PIN_BUZZER, HIGH);
  delay(1000);
  digitalWrite(albwind::PIN_BUZZER, LOW);

  delay(500);
  albwind::neopixel.clear();

  albwind::neopixel.setPixelColor(albwind::LED_LIMA, albwind::COLOR_YELLOW);
  albwind::neopixel.setPixelColor(albwind::LED_GLOW, albwind::COLOR_WHITE);
  albwind::neopixel.setPixelColor(albwind::LED_OIL, albwind::COLOR_WHITE);
  albwind::neopixel.setPixelColor(albwind::LED_COOL, albwind::COLOR_RED);
  albwind::neopixel.show();
#endif
}

albwind::Edge detect_edge(Alert &alert, int value, int thresh)
{
  int old_value = alert.old_value;
  alert.old_value = value;

  if ((value < thresh) == (old_value < thresh))
  { //No sign change
    return Edge::None;
  }

  //Detect debounce
  /*if (abs(millis() - alert.last_edge) < albwind::DEBOUNCE_TIME) //Abs to deal with wraparound from millis()
  {
    return Edge::Debounce;
  }*/

  alert.last_edge = millis();

  //Falling edge
  if (value < thresh && old_value >= thresh)
  {
    return Edge::Falling;
  }

  //Rising edge
  if (value > thresh && old_value <= thresh)
  {
    return Edge::Rising;
  }
}

bool first_oil = true;
void loop()
{
  //Handle alarm test btn
  if (detect_edge(test_alert, analogRead(albwind::PIN_ALARM_TEST), 512) == Edge::Falling)
  {
    albwind::confirm_alert();
  }

  // Handle oil alert
  switch (detect_edge(oil_alert, analogRead(albwind::PIN_OIL_SENSE), albwind::OIL_SENSE_THRESHOLD))
  {
  case Edge::Rising:
    albwind::set_alert(oil_alert, LED_INDEX::LED_OIL, false);
    break;
  case Edge::Falling:
    albwind::set_alert(oil_alert, LED_INDEX::LED_OIL, true, !first_oil);
    first_oil = false;
    break;
  }

  // Handle glow alert
  switch (detect_edge(glow_alert, analogRead(albwind::PIN_GLOW_SENSE), albwind::GLOW_SENSE_THRESHOLD))
  {
  case Edge::Rising:
    albwind::neopixel.setPixelColor(albwind::LED_GLOW, albwind::COLOR_YELLOW);
    albwind::neopixel.show();
    break;
  case Edge::Falling:
    albwind::neopixel.setPixelColor(albwind::LED_GLOW, albwind::COLOR_NONE);
    albwind::neopixel.show();
    break;
  }

  // Handle COOL alert
  switch (detect_edge(COOL_alert, analogRead(albwind::PIN_COOL_SENSE), albwind::COOL_SENSE_THRESHOLD))
  {
  case Edge::Rising:
    albwind::set_alert(COOL_alert, LED_INDEX::LED_COOL, false);
    break;
  case Edge::Falling:
    albwind::set_alert(COOL_alert, LED_INDEX::LED_COOL, true);
    break;
  }

  // Handle LIMA alert
  switch (detect_edge(LIMA_alert, analogRead(albwind::PIN_LIMA_SENSE), albwind::LIMA_SENSE_THRESHOLD))
  {
  case Edge::Rising:
    albwind::set_alert(LIMA_alert, LED_INDEX::LED_LIMA, false);
    break;
  case Edge::Falling:
    albwind::set_alert(LIMA_alert, LED_INDEX::LED_LIMA, true);
    break;
  }

  switch (detect_edge(lower_LIMA_alert, analogRead(albwind::PIN_LIMA_SENSE), albwind::low_LIMA_SENSE_THRESHOLD))
  {
  case Edge::Rising:
    albwind::set_alert(LIMA_alert, LED_INDEX::LED_LIMA, true);
    break;
  case Edge::Falling:
    albwind::set_alert(LIMA_alert, LED_INDEX::LED_LIMA, false);
    break;
  }
}