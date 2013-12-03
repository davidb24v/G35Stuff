/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <http://github.com/sowbug>.

  See README for complete attributions.
*/

#ifndef INCLUDE_G35_PROGRAMS_C2SPARKLE_H
#define INCLUDE_G35_PROGRAMS_C2SPARKLE_H

#include <LightProgram.h>

// Coded by Dave from an idea by ChrisB
// Two alternating colours with random white flashiness
// 
class Stereo : public LightProgram {
 public:
  C2Sparkle(G35& g35);
  uint32_t Do();

 private:
  const uint8_t light_count_;
  const uint8_t half_light_count_;
  const uint8_t colour1_, colour2_;
  const uint8_t delay_;
  const uint8_t count_;
  int nwhite[count_];
};

#endif  // INCLUDE_G35_PROGRAMS_C2SPARKLE_H
