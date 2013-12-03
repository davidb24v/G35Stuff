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

// Comment out the following line to use the single color version
// Uncomment to get 3 blocks of 16 lights 
//#define MULTI_COLOUR 1

// Define 3 colours (COLOUR1 used for solid colour case)
// Available colours are:
//
//  COLOR_WHITE, COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_CYAN,
//  COLOR_MAGENTA, COLOR_YELLOW, COLOR_PURPLE, COLOR_ORANGE, COLOR_PALE_ORANGE,
//  COLOR_WARMWHITE, COLOR_INDIGO, COLOR_VIOLET	   
//
#define COLOUR1 COLOR_RED
#define COLOUR2 COLOR_GREEN
#define COLOUR3 COLOR_BLUE
//
// You can also use COLOR(r,g,b) where r,g,b are each 0-15
// COLOR(15,0,0) is red etc. etc.

// How many bulbs can be white? Change this for different look and feel -
// small numbers and a small delay work well
#define NWHITE 3

// How many milliseconds to wait before picking the next bulb at random
// This has quite a dramatic effect! 
#define DELAY 10

// keep track of how many white bulbs we have...
int white[NWHITE];
int nwhite = 0;

// Shouldn't need to change anything below here, do have a read though...

// Declare "lights" as a G35string on pin "G35_PIN" with "LIGHT_COUNT" lights
G35String lights(G35_PIN, LIGHT_COUNT);

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

#ifdef MULTI_COLOUR
  // Fill with 3 blocks of solid colour (skip the first 2 bulbs)...  
  lights.fill_color(0, 2, G35::MAX_INTENSITY, COLOR_BLACK);
  lights.fill_color(2+0*16, 16, G35::MAX_INTENSITY, COLOUR1);
  lights.fill_color(2+1*16, 16, G35::MAX_INTENSITY, COLOUR2);
  lights.fill_color(2+2*16, 16, G35::MAX_INTENSITY, COLOUR3);
#else
  // use a single colour
  lights.fill_color(0, LIGHT_COUNT, G35::MAX_INTENSITY, COLOUR1);
#endif

  // Assuming analogue pin 5 (physical pin 28) is floating then this should
  // give us a different pattern every time
  randomSeed(analogRead(5));

}

void loop() {
  
  int bulb, old;
  
  // Every time through the loop we'll pick a bulb at random and make it white
  
#ifdef MULTI_COLOUR
  bulb = random(2,50);
#else
  bulb = random(50);
#endif

  // Paint bulb white
  lights.set_color(bulb, G35::MAX_INTENSITY, COLOR_WHITE);

  nwhite++;
  if ( nwhite > NWHITE ) {
    // reset the oldest white bulb to it's original colour
    old = white[0];
#ifdef MULTI_COLOUR
    if ( old < 18 ) {
      lights.set_color(old, G35::MAX_INTENSITY, COLOUR1);
    } else if ( old < 34 ) {
      lights.set_color(old, G35::MAX_INTENSITY, COLOUR2);
    } else {
      lights.set_color(old, G35::MAX_INTENSITY, COLOUR3);
    }
#else
    lights.set_color(old, G35::MAX_INTENSITY, COLOUR1);
#endif

    // shuffle out the old value...
    for (int i = 0; i < NWHITE-1; i++) white[i] = white[i+1];
    
    // Reduce nwhite
    nwhite--;
  }

  // store the new bulb number
  white[nwhite-1] = bulb;
  
  // Pause a little...
  delay(DELAY);
    
}
