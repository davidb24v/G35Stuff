/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <http://github.com/sowbug>.

  See README for complete attributions.
*/

#include <C2Sparkle.h>

C2Sparkle::C2Sparkle(G35& g35) : LightProgram(g35),
                           light_count_(g35_.get_light_count()),
                           colour1_(COLOR_RED),
                           colour2_(COLOR_GREEN),
                           count_(10),
                           delay_(10) {
  g35_.fill_color(0, light_count_, 255, COLOR_BLACK);
  for (int i=0; i < light_count_; i++) {
    if ( i % 2 ) {
      g35_.set_color(i, 255, color1_));
    } else {
      g35_.set_color(i, 255, color2_));
    }
  }
}

uint32_t C2Sparkle::Do() {

}
