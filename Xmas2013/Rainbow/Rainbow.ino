/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  See README for complete attributions.
*/

#include <G35String.h>

//#define TWO_STRINGS

#ifdef TWO_STRINGS
  #define LIGHT_COUNT 50
  #define G35_PIN1 9
  #define G35_PIN2 10
  G35String lights1(G35_PIN1, LIGHT_COUNT);
  G35String lights2(G35_PIN2, LIGHT_COUNT);
#else
  #define LIGHT_COUNT 49
  #define G35_PIN 9
  G35String lights(G35_PIN, LIGHT_COUNT);
#endif

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
#else
  lights.enumerate();
#endif
}

// Colour table for rainbow
const int COLOURS[] = {COLOR_RED, COLOR_ORANGE, COLOR_YELLOW, COLOR_GREEN, COLOR_BLUE,
                       COLOR_INDIGO, COLOR_VIOLET};
const int NCOLOURS = sizeof(COLOURS)/sizeof(int);

// Counter - to insert a long pause, occasionally
int pause = 0;

// Rainbow, filling out from the centre
void fillRainbow() {
  int wait = random(100);
#ifdef TWO_STRINGS
  // 2 strings: assume 0 is in "middle" of layout and strings extend out
  // horizontally in both directions
  int count = LIGHT_COUNT;
  // Repeat rainbow colours from centre to ends
  int index = 0;
  for (int bulb = 0; bulb < count; bulb++) {
    int colour = COLOURS[index];
    lights1.set_color(bulb, G35::MAX_INTENSITY, colour);
    lights2.set_color(bulb, G35::MAX_INTENSITY, colour);
    index++;
    index = index % NCOLOURS;
    delay(wait);
  }
#else
  int middle = LIGHT_COUNT/2;
  int count = LIGHT_COUNT/2+1;
  int index = 0;
  for (int bulb = 0; bulb < count; bulb++) {
    int colour = COLOURS[index];
    lights.set_color(middle+bulb, G35::MAX_INTENSITY, colour);
    lights.set_color(middle-bulb, G35::MAX_INTENSITY, colour);
    index++;
    index = index % NCOLOURS;
    delay(wait);
  }
#endif
}

void clearToCentre() {
  int wait = random(100);
#ifdef TWO_STRINGS
  // 2 strings: assume 0 is in "middle" of layout and strings extend out
  // horizontally in both directions
  int count = LIGHT_COUNT;
  // Switch off from outside in
  for (int bulb = count-1; bulb >= 0; bulb--) {
    lights1.set_color(bulb, G35::MAX_INTENSITY, COLOR_BLACK);
    lights2.set_color(bulb, G35::MAX_INTENSITY, COLOR_BLACK);
    delay(wait);
  }
#else
  int middle = LIGHT_COUNT/2;
  int count = LIGHT_COUNT/2;
  // Switch off from outside in
  for (int bulb = count; bulb >= 0; bulb--) {
    lights.set_color(middle+bulb, G35::MAX_INTENSITY, COLOR_BLACK);
    lights.set_color(middle-bulb, G35::MAX_INTENSITY, COLOR_BLACK);
    delay(wait);
  }
#endif
}

void loop() {
  fillRainbow();
  pause++;
  // Stay on full display for 10 seconds, every 11th time through
  if ( pause % 11 == 0 ) {
    delay(10000);
  } else {
    delay(500);
  }
  clearToCentre();
  delay(100);
}
