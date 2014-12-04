
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

void blat(const int colour, const int perCent, const int wait) {
  int intensity = perCent*G35::MAX_INTENSITY/100;
  #ifdef TWO_STRINGS
    lights1.fill_color(0, LIGHT_COUNT, intensity, colour);
    lights2.fill_color(0, LIGHT_COUNT, intensity, colour);
  #else
    lights.fill_color(0, LIGHT_COUNT, intensity, colour);
  #endif
  delay(wait);
}

// Colour table for not quite a rainbow
const int COLOURS[] = {COLOR_RED, COLOR_ORANGE, COLOR_YELLOW, COLOR_GREEN, COLOR_BLUE,
                       COLOR_WHITE, COLOR_VIOLET};
const int NCOLOURS = sizeof(COLOURS)/sizeof(int);

void setup() {
#ifdef TWO_STRINGS
  lights1.enumerate();
  lights2.enumerate();
#else
  lights.enumerate();
#endif
}

void loop() {
  for(int i=0; i<NCOLOURS; i++) {
    blat(COLOURS[i],100,1000);
    blat(COLOURS[i],60,750);
    blat(COLOURS[i],15,500);
  }
}
