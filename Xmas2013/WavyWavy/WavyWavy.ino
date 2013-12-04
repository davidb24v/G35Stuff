
#include <G35String.h>

//#define TWO_STRINGS

#ifdef TWO_STRINGS
  #define LIGHT_COUNT 50
  #define G35_PIN1 9
  #define G35_PIN2 10
  G35String lights1(G35_PIN1, LIGHT_COUNT);
  G35String lights2(G35_PIN2, LIGHT_COUNT);
  const int middle = 0;
#else
  #define LIGHT_COUNT 49
  #define G35_PIN 9
  G35String lights(G35_PIN, LIGHT_COUNT);
  const int middle = LIGHT_COUNT/2;
#endif

// Simple function to get amount of RAM free
int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

// Colour table for rainbow
const int COLOURS[] = {COLOR_BLUE, COLOR_RED, COLOR_ORANGE, COLOR_YELLOW, COLOR_GREEN};
const int NCOLOURS = sizeof(COLOURS)/sizeof(int);

// Counter - to insert a long pause, occasionally
int pause = 0;

// Width of half a string
const float width = (float) LIGHT_COUNT/2.0;

// 2*pi, roughly
const float twoPi = 6.283185;

// pre-compute 2*pi/(half of LIGHT_COUNT)
const float mul = 2.0*twoPi/(float) LIGHT_COUNT;

int getIntensity(const int bulb, const int centre) {
  float x = mul * (float) (bulb-centre);
  return int(G35::MAX_INTENSITY*(0.95*abs(sin(x))+0.05));
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

void loop() {
  int offset = 0;
  for (int colIndex=0; colIndex<NCOLOURS; colIndex++) {
    int colour = COLOURS[colIndex];
    for (int repeat=0; repeat < 250; repeat++) {
      // Update all bulbs
      #ifdef TWO_STRINGS
      for (int bulb=0; bulb < LIGHT_COUNT; bulb++) {
      #else
      for (int bulb=0; bulb <= LIGHT_COUNT/2; bulb++) {
      #endif
        int intensity = getIntensity(bulb,middle+offset);
        #ifdef TWO_STRINGS
          lights1.set_color(bulb, intensity, colour);
          lights2.set_color(bulb, intensity, colour);
        #else
          lights.set_color(middle+bulb, intensity, colour);
          lights.set_color(middle-bulb, intensity, colour);
        #endif
      }
      delay(100);
      offset++;
    }
  }
}
