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
#define LIGHT_COUNT 50

// Arduino pin number for lights (physical pin 15)
#define G35_PIN 9

// Declare "lights" as a G35string on pin "G35_PIN" with "LIGHT_COUNT" lights
G35String lights(G35_PIN, LIGHT_COUNT);

// Delay period between setting r,g,b values (milliseconds)
#define DELAY 20

// Simple function to get amount of RAM free (useful to see how much of the 4K is left)
int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void setup() {
  Serial.begin (115200);
  Serial.println(freeRam());
  
  // Initialise lights
  lights.enumerate();

}

void loop() {
  // Loop over all possible values...
  for (int b=0; b<16; b++ ) {
    for (int g=0; g<16; g++) {
      for (int r=0; r<16; r++) {
        lights.fill_color(0, LIGHT_COUNT, G35::MAX_INTENSITY, COLOR(r,g,b) );
        delay(DELAY);
      }
    }
  }
    
}
