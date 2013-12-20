#include <G35String.h>

//#define TWO_STRINGS

#ifdef TWO_STRINGS
  #define LIGHT_COUNT 50
  #define G35_PIN1 9
  #define G35_PIN2 10
  #define NUM_BULBS 10
  #define NUM_GROUPS 5
  G35String lights1(G35_PIN1, LIGHT_COUNT);
  G35String lights2(G35_PIN2, LIGHT_COUNT);
#else
  #define LIGHT_COUNT 49
  #define G35_PIN 9
  #define NUM_BULBS 7
  #define NUM_GROUPS 7
  G35String lights(G35_PIN, LIGHT_COUNT);
#endif

// Colour table for rainbow
const int COLOURS[] = {COLOR_RED, COLOR_ORANGE, COLOR_YELLOW, COLOR_GREEN,
                       COLOR_BLUE, COLOR_INDIGO, COLOR_WHITE};
const int NCOLOURS = sizeof(COLOURS)/sizeof(int);

// Maximum delay after each step
const int MAX_DELAY=1000;

// Delay after each step
int wait=MAX_DELAY/2;

// Counter
int counter = 0;

void setup() {
#ifdef TWO_STRINGS
  lights1.enumerate();
  lights2.enumerate();
#else
  lights.enumerate();
#endif

  // Assuming analogue pin 0 is floating then this should
  // give us a different pattern every time, not really but
  // it will do...
  randomSeed(analogRead(0));
}

void loop() {
  // Set delay occasionally
  if ( ++counter % 100 == 0 ) {
    counter = 0;
    wait = 100 + random(MAX_DELAY);
  }
  
  #ifdef TWO_STRINGS
    // Get first colour
    byte first = counter % NCOLOURS;
    // Loop over groups
    for (int group=0; group < NUM_GROUPS; group++) {
      byte colour = (group + first) % NCOLOURS;
      lights1.fill_color(group*NUM_BULBS, NUM_BULBS, G35::MAX_INTENSITY, COLOURS[colour]);
      lights2.fill_color(group*NUM_BULBS, NUM_BULBS, G35::MAX_INTENSITY, COLOURS[colour]);
    }
  #else
    // Get first colour
    byte first = counter % NCOLOURS;
    // Loop over groups
    for (int group=0; group < NUM_GROUPS; group++) {
      byte colour = (group + first) % NCOLOURS;
      lights.fill_color(group*NUM_BULBS, NUM_BULBS, G35::MAX_INTENSITY, COLOURS[colour]);
    }
  #endif

  // Pause
  delay(wait);
}
