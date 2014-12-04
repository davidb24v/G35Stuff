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
#define LIGHT_COUNT 34

// Arduino pin number for lights (physical pin 15)
#define G35_PIN 9

// Available colours are:
//
//  COLOR_WHITE, COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_CYAN,
//  COLOR_MAGENTA, COLOR_YELLOW, COLOR_PURPLE, COLOR_ORANGE, COLOR_PALE_ORANGE,
//  COLOR_WARMWHITE, COLOR_INDIGO, COLOR_VIOLET	   
//
// You can also use COLOR(r,g,b) where r,g,b are each 0-15
// COLOR(15,0,0) is red etc. etc.

// How many main colours
#define NCOLOURS 8

// How often to switch main colours (milli-seconds)
#define COLOUR_CYCLE 5000

int COLOURS[NCOLOURS] = {0x907, COLOR_RED, COLOR_GREEN, 
                         COLOR_BLUE, COLOR_YELLOW,
                         COLOR_WARMWHITE, COLOR_BLACK,
                         COLOR_ORANGE};

// Track when colours change
long lastChange;

// Current main colour
int COLOUR;

// How many bulbs can be white? Change this for different look and feel -
// small numbers and a small delay work well
#define NWHITE 3

// How many milliseconds to wait before picking the next bulb at random
// This has quite a dramatic effect! 
#define DELAY 10

// keep track of how many white bulbs we have...
int white[NWHITE];
int nwhite = 0;

// Declare "lights" as a G35string on pin "G35_PIN" with "LIGHT_COUNT" lights
G35String lights(G35_PIN, LIGHT_COUNT);

void setup() {
  
  // Initialise lights
  lights.enumerate();

  // Start with first colour and note start time
  COLOUR = 0;
  lights.fill_color(0, LIGHT_COUNT, G35::MAX_INTENSITY, 
                    COLOURS[COLOUR]);
  lastChange = millis();
  
  // Assuming analogue pin 5 (physical pin 28) is floating then this should
  // give us a different pattern every time
  randomSeed(analogRead(5));

}

void loop() {
  
  int bulb, old;
  
  // Every time through the loop we'll pick a bulb at random and make it white
  bulb = random(LIGHT_COUNT);

  // Paint bulb white
  lights.set_color(bulb, G35::MAX_INTENSITY, COLOR_WHITE);
  
  // Time to switch colour?
  if ( millis()-lastChange > COLOUR_CYCLE ) {
   lastChange += COLOUR_CYCLE;
   COLOUR++;
   COLOUR = COLOUR % NCOLOURS; 
  }
  
  nwhite++;
  if ( nwhite > NWHITE ) {
    // reset the oldest white bulb to it's original colour
    old = white[0];
    lights.set_color(old, G35::MAX_INTENSITY, 
                     COLOURS[COLOUR]);

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
