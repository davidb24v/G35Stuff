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

G35String lights(G35_PIN, LIGHT_COUNT);

// Simple function to get amount of RAM free
int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

// Take 50 lights and push a R-G-B colours down the string to fill it up
void fill_em_up() {
  int last = LIGHT_COUNT;
  // First light to set is number 50
  // Remainder dividing 50 by 3 is 2 so put RED at position 2 in array
  // we count down (next is 49) so GREEN goes at position 1
  // and finally BLUE at position 0
  int colours[] = {COLOR_BLUE, COLOR_GREEN, COLOR_RED};
  
  // store the colour of the light we're aiming to set
  int colour;

  // While there's stuff to do 
  while ( last > 0 ) {
    colour = colours[last %3];
    // Starting at the first light...
    for(int i=0; i < last; i++) {
      if( i > 0 ) {
        // If we're not at the first light then turn off the one we
        // set last time through the loop
        lights.fill_color(i-1, 1, G35::MAX_INTENSITY, COLOR_BLACK);
      }
      // Set colour to R,G or B based on light number
      lights.fill_color(i, 1, G35::MAX_INTENSITY, colour);
      
      // Adjust the delay to determine how long this takes to do
      // This goes from 5ms at the start to 55ms at the end of the loop
      delay(LIGHT_COUNT+5-last);
    }
    // That's the "last" light set so reduce the number we have to do
    last--;
  }
}

void setup() {
  Serial.begin (115200);
  Serial.println(freeRam());
  lights.enumerate();
  lights.fill_color(0, LIGHT_COUNT, G35::MAX_INTENSITY, COLOR_BLACK);
  fill_em_up();
}

void loop() {
}
