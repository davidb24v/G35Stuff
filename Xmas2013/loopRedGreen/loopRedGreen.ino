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
#define BETWEEN_BULBS 100

void setup() {
#ifdef TWO_STRINGS
  lights1.enumerate();
  lights2.enumerate();
#else
  lights.enumerate();
#endif
}

void loop() {
  // Roll in one colour from each end
  for (int left=0; left < LIGHT_COUNT; left++) {
    int right = (LIGHT_COUNT-1)-left;
    #ifdef TWO_STRINGS
      lights1.set_color(left, G35::MAX_INTENSITY, COLOR_RED);
      lights2.set_color(left, G35::MAX_INTENSITY, COLOR_RED);
      lights1.set_color(right, G35::MAX_INTENSITY, COLOR_GREEN);
      lights2.set_color(right, G35::MAX_INTENSITY, COLOR_GREEN);
    #else
      lights.set_color(left, G35::MAX_INTENSITY, COLOR_RED);
      lights.set_color(right, G35::MAX_INTENSITY, COLOR_GREEN);
    #endif
    delay(BETWEEN_BULBS);
  }
}
