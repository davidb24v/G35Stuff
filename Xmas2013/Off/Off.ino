/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  See README for complete attributions.
*/

#include <G35String.h>

//#define TWO_STRINGS

// Total # of lights on string (usually 50, 48, or 36). Maximum is 63, because
// the protocol uses 6-bit addressing and bulb #63 is reserved for broadcast
// messages.
#ifdef TWO_STRINGS
  #define LIGHT_COUNT 50
#else
  #define LIGHT_COUNT 49
#endif

// Arduino pin number for lights (physical pin 15)
#ifdef TWO_STRINGS
  #define G35_PIN1 9
  #define G35_PIN2 10
#else
  #define G35_PIN 9
#endif

#ifdef TWO_STRINGS
  G35String lights1(G35_PIN1, LIGHT_COUNT);
  G35String lights2(G35_PIN2, LIGHT_COUNT);
#else
  G35String lights(G35_PIN, LIGHT_COUNT);
#endif

// Define a second in milli-seconds
#define SECONDS 1000

// Simple function to get amount of RAM free
int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void setup() {
  Serial.begin (115200);
  Serial.println(freeRam());
#ifdef TWO_STRINGS
  lights1.enumerate();
  lights2.enumerate();
  lights1.fill_color(0, LIGHT_COUNT, G35::MAX_INTENSITY, COLOR_BLACK);
  lights2.fill_color(0, LIGHT_COUNT, G35::MAX_INTENSITY, COLOR_BLACK);
#else
  lights.enumerate();
  lights.fill_color(0, LIGHT_COUNT, G35::MAX_INTENSITY, COLOR_BLACK);
#endif
}

void loop() {
}

