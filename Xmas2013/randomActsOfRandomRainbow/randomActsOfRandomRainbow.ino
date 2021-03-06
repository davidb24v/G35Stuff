
#include <G35String.h>

//#define TWO_STRINGS

#ifdef TWO_STRINGS
  #define LIGHT_COUNT 50
  #define G35_PIN1 9
  #define G35_PIN2 10
  G35String lights1(G35_PIN1, LIGHT_COUNT);
  G35String lights2(G35_PIN2, LIGHT_COUNT);
#else
  #define LIGHT_COUNT 34
  #define G35_PIN 9
  G35String lights(G35_PIN, LIGHT_COUNT);
#endif

// Simple function to get amount of RAM free
int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

// Colour table for rainbow
const int COLOURS[] = {COLOR_RED, COLOR_ORANGE, COLOR_YELLOW, COLOR_GREEN,
                       COLOR_BLUE, COLOR_INDIGO, COLOR_WHITE};
const int NCOLOURS = sizeof(COLOURS)/sizeof(int);

// Storage for bitmap of lights 7x8 bits = 56 (we need 50)
const int bulbBytes = 7;
#ifdef TWO_STRINGS
byte bulbs1[bulbBytes];
byte bulbs2[bulbBytes];
#else
byte bulbs[bulbBytes];
#endif

// Macros to work out which bit of which byte to set..
#define GETBIT int bit = bulb % 8
#define GETIND int ind = bulb / 8

// Delays after each bulb change and after each string is done
#define BETWEEN_BULBS 50
#define AFTER_STRING 3000

// Clear bitmap
void clearBulbs(byte bulbs[]) {
  for (int i=0; i < bulbBytes; i++) {
    bulbs[i] = 0x00;
  }
}

void setBulb(byte bulbs[], const int bulb) {
  GETBIT;
  GETIND;
  bulbs[ind] |= 1 << bit;
}

void unSetBulb(byte bulbs[], const int bulb) {
  GETBIT;
  GETIND;
  bulbs[ind] &= ~(1 << bit);
}

bool bulbIsUnset(byte bulbs[], const int bulb) {
  GETBIT;
  GETIND;
  return (bulbs[ind] & (1 << bit)) == 0;
}

bool bulbIsSet(byte bulbs[], const int bulb) {
  GETBIT;
  GETIND;
  return (bulbs[ind] & (1 << bit)) > 0;
}

int countBits(byte bulbs[]) {
  int count=0;
  for (int bulb=0; bulb < LIGHT_COUNT; bulb++) {
    GETBIT;
    GETIND;
    if ( bulbs[ind] & (1 << bit) ) count++;
  }
  return count;
}

void setup() {
  Serial.begin (115200);
  Serial.println(freeRam());
#ifdef TWO_STRINGS
  lights1.enumerate();
  lights2.enumerate();
  clearBulbs(bulbs1);
  clearBulbs(bulbs2);
#else
  lights.enumerate();
  clearBulbs(bulbs);
#endif

  // Assuming analogue pin 0 is floating then this should
  // give us a different pattern every time, not really but
  // it will do...
  randomSeed(analogRead(0));
}

void loop() {
  // Which colour to display
  int colour;

  #ifdef TWO_STRINGS
    // Randomly switch on to current colour
    while ( countBits(bulbs1) < LIGHT_COUNT ||
            countBits(bulbs2) < LIGHT_COUNT ) {

      // Set the bulb on first string
      int bulb = random(LIGHT_COUNT);
      colour = random(NCOLOURS);
      if ( bulbIsUnset(bulbs1, bulb) ) {
        setBulb(bulbs1, bulb);
        lights1.set_color(bulb, G35::MAX_INTENSITY, COLOURS[colour]);
        delay(BETWEEN_BULBS);
      }

      // Set the bulb on second string
      bulb = random(LIGHT_COUNT);
      colour = random(NCOLOURS);
      if ( bulbIsUnset(bulbs2, bulb) ) {
        setBulb(bulbs2, bulb);
        lights2.set_color(bulb, G35::MAX_INTENSITY, COLOURS[colour]);
        delay(BETWEEN_BULBS);
      }
    }
    delay(AFTER_STRING);
    
    // Randomly switch off
    while ( countBits(bulbs1) > 0 ||
            countBits(bulbs2) > 0 ) {

      // Set the bulb on first string
      int bulb = random(LIGHT_COUNT);
      if ( bulbIsSet(bulbs1, bulb) ) {
        unSetBulb(bulbs1, bulb);
        lights1.set_color(bulb, G35::MAX_INTENSITY, COLOR_BLACK);
        delay(BETWEEN_BULBS);
      }

      // Set the bulb on second string
      bulb = random(LIGHT_COUNT);
      if ( bulbIsSet(bulbs2, bulb) ) {
        unSetBulb(bulbs2, bulb);
        lights2.set_color(bulb, G35::MAX_INTENSITY, COLOR_BLACK);
        delay(BETWEEN_BULBS);
      }
    }
  #else
    // Randomly switch on to current colour
    while ( countBits(bulbs) < LIGHT_COUNT ) {
      int bulb = random(LIGHT_COUNT);
      colour = random(NCOLOURS);
      if ( bulbIsUnset(bulbs, bulb) ) {
        setBulb(bulbs, bulb);
        lights.set_color(bulb, G35::MAX_INTENSITY, COLOURS[colour]);
        delay(BETWEEN_BULBS);
      }
    }
    delay(AFTER_STRING);

    // Randomly switch off again...
    while ( countBits(bulbs) > 0 ) {
      int bulb = random(LIGHT_COUNT);
      if ( bulbIsSet(bulbs, bulb) ) {
        unSetBulb(bulbs, bulb);
        lights.set_color(bulb, G35::MAX_INTENSITY, COLOR_BLACK);
        delay(BETWEEN_BULBS);
      }
    }
    
  #endif

}
