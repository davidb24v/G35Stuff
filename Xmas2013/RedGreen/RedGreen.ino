/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  See README for complete attributions.
*/

#include <G35String.h>

// Total # of lights on string (usually 50, 48, or 36). Maximum is 63, because
// the protocol uses 6-bit addressing and bulb #63 is reserved for broadcast
// messages.
#define LIGHT_COUNT 49

// Arduino pin number for lights (physical pin 15)
#define G35_PIN 9

G35String lights(G35_PIN, LIGHT_COUNT);

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
  lights.enumerate();
}

// Set even lights (0, 2, ...) to "c1", odd lights to "c2"
void alternateColours(const int c1, const int c2) {
  if ( c1 == c2 ) {
    lights.fill_color(0, LIGHT_COUNT, G35::MAX_INTENSITY, c1);
  } else {
    // Set even lights to c1, odd lights to c2
    for (int i=0; i < LIGHT_COUNT; i++) {
      if ( i % 2 == 0 ) {
        lights.fill_color(i, 1, G35::MAX_INTENSITY, c1);
      } else {
        lights.fill_color(i, 1, G35::MAX_INTENSITY, c2);
      }
    }
  }
}

void loop() {
  // Set all lights to red...
  lights.fill_color(0, LIGHT_COUNT, G35::MAX_INTENSITY, COLOR_RED);
  // Wait for 5 seconds
  delay(5*SECONDS);

  // Set all lights to green...
  lights.fill_color(0, LIGHT_COUNT, G35::MAX_INTENSITY, COLOR_GREEN);
  // Wait for 5 seconds
  delay(5*SECONDS);

  // Set odd lights to red, even to green
  alternateColours(COLOR_RED,COLOR_GREEN);
  delay(5*SECONDS);

  // Set odd lights to green, even lights to red
  alternateColours(COLOR_GREEN,COLOR_RED);
  delay(5*SECONDS);
  
  // Yawn... Now switch red/green 10 times
  for (int count=0; count < 10; count++) {
    alternateColours(COLOR_RED,COLOR_GREEN);
    delay(1*SECONDS);
    alternateColours(COLOR_GREEN,COLOR_RED);
    delay(1*SECONDS);
  }

}
