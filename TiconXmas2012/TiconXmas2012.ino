/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2; -*-

   G35: An Arduino library for GE Color Effects G-35 holiday lights.
   Copyright © 2012 The G35 Authors. Use, modification, and distribution are
   subject to the BSD license as described in the accompanying LICENSE file.

   TiconXmas2012.ino is the code that I used for Christmas 2012 at my home.
   It's yet another demonstration of working code.

   By Mike Tsao <github.com/sowbug>. See README for complete attributions.
*/

#include <G35String.h>
#include <G35StringGroup.h>
#include <ProgramRunner.h>
#include <StockPrograms.h>
#include <PlusPrograms.h>

// Arduino with Like A G35 shield (https://github.com/sowbug/like-a-g35).
// First string on pin 8. Second on 9. My roofline isn't quite long enough
// for 100 lights, so I have the final 10 hidden.
G35String lights(9, 49, 49, 0, false);

const int PROGRAM_COUNT = StockProgramGroup::ProgramCount +
  PlusProgramGroup::ProgramCount;

StockProgramGroup stock_programs;
PlusProgramGroup plus_programs;
G35StringGroup string_group;

LightProgram* CreateProgram(uint8_t program_index) {
  program_index = random() % PROGRAM_COUNT;

  if (program_index < StockProgramGroup::ProgramCount) {
    return stock_programs.CreateProgram(string_group, program_index);
  }
  program_index -= StockProgramGroup::ProgramCount;

  if (program_index < PlusProgramGroup::ProgramCount) {
    return plus_programs.CreateProgram(string_group, program_index);
  }
  program_index -= PlusProgramGroup::ProgramCount;

  return NULL;
}

// How long each program should run.
#define PROGRAM_DURATION_SECONDS (90)

ProgramRunner runner(CreateProgram, PROGRAM_COUNT, PROGRAM_DURATION_SECONDS);

// http://www.utopiamechanicus.com/77/better-arduino-random-numbers/
// We assume A0 and A1 are disconnected.
uint32_t seedOut(unsigned int noOfBits) {
  uint32_t seed = 0, limit = 99;
  int bit0 = 0, bit1 = 0;
  while (noOfBits--) {
    for (int i = 0; i < limit; ++i) {
      bit0 = analogRead(0) & 1;
      bit1 = analogRead(1) & 1;
      if (bit1 != bit0)
        break;
    }
    seed = (seed << 1) | bit1;
  }
  return seed;
}

void setup() {
  uint32_t seed = seedOut(32);
  randomSeed(seed);
  seed &= 0xff;
  // random() isn't very random. But this seed generator works quite well.
  while (seed--) {
    random();
  }

  delay(50);
  lights.enumerate();

  string_group.AddString(&lights);
}

void loop() {
  runner.loop();
}
