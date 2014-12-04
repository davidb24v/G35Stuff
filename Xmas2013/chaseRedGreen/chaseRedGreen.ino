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
  lights1.fill_color(0, LIGHT_COUNT/2-1, G35::MAX_INTENSITY, COLOR_RED);
  lights1.fill_color(LIGHT_COUNT/2, LIGHT_COUNT, G35::MAX_INTENSITY, COLOR_GREEN);
  lights2.enumerate();
  lights2.fill_color(0, LIGHT_COUNT/2-1, G35::MAX_INTENSITY, COLOR_RED);
  lights2.fill_color(LIGHT_COUNT/2, LIGHT_COUNT, G35::MAX_INTENSITY, COLOR_GREEN);
#else
  lights.enumerate();
  lights.fill_color(0, LIGHT_COUNT/2-1, G35::MAX_INTENSITY, COLOR_RED);
  lights.fill_color(LIGHT_COUNT/2, LIGHT_COUNT, G35::MAX_INTENSITY, COLOR_GREEN);
#endif
  // Assuming analogue pin 0 is floating then this should
  // give us a different pattern every time, not really but
  // it will do...
  randomSeed(analogRead(0));
}

long time=0;
int delayTime = BETWEEN_BULBS;

void loop() {
  // Change delay time every 10 seconds
  if ( millis()-time > 10000 ) {
    time = millis();
    delayTime = random(2*BETWEEN_BULBS);
  }
  for (int bulb=0; bulb < LIGHT_COUNT; bulb++) {
    #ifdef TWO_STRINGS
      lights1.set_color(bulb, G35::MAX_INTENSITY, COLOR_GREEN);
      lights1.set_color((bulb+LIGHT_COUNT/2) % LIGHT_COUNT, G35::MAX_INTENSITY, COLOR_RED);
      lights2.set_color(bulb, G35::MAX_INTENSITY, COLOR_GREEN);
      lights2.set_color((bulb+LIGHT_COUNT/2) % LIGHT_COUNT, G35::MAX_INTENSITY, COLOR_RED);
    #else
      lights.set_color(bulb, G35::MAX_INTENSITY, COLOR_GREEN);
      lights.set_color((bulb+LIGHT_COUNT/2) % LIGHT_COUNT, G35::MAX_INTENSITY, COLOR_RED);
    #endif
    //delay(BETWEEN_BULBS);
    delay(delayTime);
  }
}
