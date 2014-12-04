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

// Delay after each bulb change
const int BETWEEN_STEPS=50;

// Delay before switching off "last" bulbs
const int BETWEEN_BULBS=2;

void setup() {
#ifdef TWO_STRINGS
  lights1.enumerate();
  lights2.enumerate();
#else
  lights.enumerate();
#endif
}

void loop() {
  // Loop over bulbs
  #ifdef TWO_STRINGS
    // Forward loop
    for (int red=0; red < LIGHT_COUNT; red++) {
      int green = (LIGHT_COUNT-1) - red;
      // Turn on lights
      lights1.set_color(red, G35::MAX_INTENSITY, COLOR_RED);
      lights1.set_color(green, G35::MAX_INTENSITY, COLOR_GREEN);
      lights2.set_color(red, G35::MAX_INTENSITY, COLOR_RED);
      lights2.set_color(green, G35::MAX_INTENSITY, COLOR_GREEN);
      // Switch off "last" bulb
      delay(BETWEEN_BULBS);
      if (red > 0) {
        lights1.set_color(red-1, G35::MAX_INTENSITY, COLOR_BLACK);
        lights2.set_color(red-1, G35::MAX_INTENSITY, COLOR_BLACK);
      }
      if (green < LIGHT_COUNT) {
        lights1.set_color(green+1, G35::MAX_INTENSITY, COLOR_BLACK);
        lights2.set_color(green+1, G35::MAX_INTENSITY, COLOR_BLACK);
      }
      delay(BETWEEN_STEPS);
    }
    
    // Reverse loop
    for (int green=0; green < LIGHT_COUNT; green++) {
      int red = (LIGHT_COUNT-1) - green;
      // Turn on lights
      lights1.set_color(red, G35::MAX_INTENSITY, COLOR_RED);
      lights1.set_color(green, G35::MAX_INTENSITY, COLOR_GREEN);
      lights2.set_color(red, G35::MAX_INTENSITY, COLOR_RED);
      lights2.set_color(green, G35::MAX_INTENSITY, COLOR_GREEN);
      // Switch off "last" bulb
      delay(BETWEEN_BULBS);
      if (red < LIGHT_COUNT) {
        lights1.set_color(red+1, G35::MAX_INTENSITY, COLOR_BLACK);
        lights2.set_color(red+1, G35::MAX_INTENSITY, COLOR_BLACK);
      }
      if (green > 0) {
        lights1.set_color(green-1, G35::MAX_INTENSITY, COLOR_BLACK);
        lights2.set_color(green-1, G35::MAX_INTENSITY, COLOR_BLACK);
      }
      delay(BETWEEN_STEPS);
    }
  #else
    // Forward loop
    for (int red=0; red < LIGHT_COUNT; red++) {
      int green = (LIGHT_COUNT-1) - red;
      // Turn on lights
      lights.set_color(red, G35::MAX_INTENSITY, COLOR_RED);
      lights.set_color(green, G35::MAX_INTENSITY, COLOR_GREEN);
      // Switch off "last" bulb
      delay(BETWEEN_BULBS);
      if (red > 0) lights.set_color(red-1, G35::MAX_INTENSITY, COLOR_BLACK);
      if (green < LIGHT_COUNT) lights.set_color(green+1, G35::MAX_INTENSITY, COLOR_BLACK);
      delay(BETWEEN_STEPS);
    }
    
    // Reverse loop
    for (int green=0; green < LIGHT_COUNT; green++) {
      int red = (LIGHT_COUNT-1) - green;
      // Turn on lights
      lights.set_color(red, G35::MAX_INTENSITY, COLOR_RED);
      lights.set_color(green, G35::MAX_INTENSITY, COLOR_GREEN);
      // Switch off "last" bulb
      delay(BETWEEN_BULBS);
      if (red < LIGHT_COUNT) lights.set_color(red+1, G35::MAX_INTENSITY, COLOR_BLACK);
      if (green > 0) lights.set_color(green-1, G35::MAX_INTENSITY, COLOR_BLACK);
      delay(BETWEEN_STEPS);
    }
  #endif
}
