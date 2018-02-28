/*
 * NeoPixel wall clock. 
 * Displays time using a RGB led strip.
 * 
 * 
 */

#include <Arduino.h>

#include <DS3231.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6

#define NUMPIXELS 12

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

DS3231  rtc(SDA, SCL);

Time  t;

byte h = 99, m = 99;
byte oldH = 99, oldM = 99;
float p = 1;

void setup() {
  randomSeed(analogRead(0));
  pixels.begin();
  rtc.begin();

  //rtc.setDOW(TUESDAY);     // Set Day-of-Week to SUNDAY
  //rtc.setTime(22, 29, 0);     // Set the time to 00:00:00 (24hr format)
  //rtc.setDate(4, 5, 2016);   // Set the date to January 1st, 2014
  allOff();
}

void loop() {
  oldM = m;
  oldH = h;
  t = rtc.getTime();
  h = t.hour;
  m = ((t.min * 0.2) + 0.5);
  
  if (m > 6) {
    h++;
  }

  if (h > 11) {
    h = h - 12;
    hourlyFunTime();
  }
  if (m > 11) {
    m = m - 12;
  }
  if (m != oldM) {
    SmoothstepM();
  }
  if (h != oldH) {
    SmoothstepH();
  }

  allOff();
  if (h == m) {
    light(h, 255, 109, 107, p);
  } else {
    light(h, 255, 133, 30, p);
    light(m, 255, 84, 183, p);
  }

  pixels.show();
  delay(300);
}


void light(byte number, byte R, byte G, byte B, float power) {
  pixels.setPixelColor(number, pixels.Color((R * power), (G * power), (B * power)));
}

void allOff() {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }
}

void pulse() {
  light(h, 255, 133, 30, p);
}

void SmoothstepM() {
  for (float j = p; j > 0; j = (j - 0.001)) {
    light(h, 255, 133, 30, p);
    light(oldM, 255, 84, 183, j);
    pixels.show();
    delay(10);
  }
  for (float t = 0; t < p; t = (t + 0.001)) {
    light(h, 255, 133, 30, p);
    light(m, 255, 84, 183, t);
    pixels.show();
    delay(10);
  }
}

void SmoothstepH() {
  for (float j = p; j > 0; j = (j - 0.001)) {
    light(m, 255, 84, 183, p);
    light(oldH, 255, 133, 30, j);
    pixels.show();
    delay(10);
  }
  for (float t = 0; t < p; t = (t + 0.001)) {
    light(m, 255, 84, 183, p);
    light(h, 255, 133, 30, t);
    pixels.show();
    delay(10);
  }
}

void hourlyFunTime() {
  for (int i = 0; i < NUMPIXELS; i++) {
    light(i, 255, 133, random(0, 255), p);
    delay(300);
  }

}



