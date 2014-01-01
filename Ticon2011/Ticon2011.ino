/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2; -*-

   G35: An Arduino library for GE Color Effects G-35 holiday lights.
   Copyright © 2011 The G35 Authors. Use, modification, and distribution are
   subject to the BSD license as described in the accompanying LICENSE file.

   Ticon2011.ino is the code that I used for Christmas 2011 at my home. It's yet
   another demonstration of working code.

   By Mike Tsao <github.com/sowbug>. See README for complete attributions.
*/

// If defined, then a 38KHz IR receiver is supplying data on pin 11. I didn't
// really use this much; if it detects any recognized remote code, it skips to
// the next program. So in theory people driving by my house who happen to have
// a TV remote in their car could stop and change the program.
//
// Pretty much any of these IR receivers will work. I used a Vishay Semi
// TSOP58038 (http://octopart.com/partsearch#search/requestData&q=TSOP58038).
//#define USE_REMOTE 1

// If defined, then the hardware is an Arduino. If not, then it's a Teensy.
// More described below. I used this switch because I had an Arduino in the
// house when I was writing new programs, then ran outside in the cold with my
// laptop to reprogram the Teensy that was running the real lights.
#define ARDUINO_HW 1

#include <G35String.h>
#include <G35StringGroup.h>
#include <ProgramRunner.h>
#include <StockPrograms.h>
#include <PlusPrograms.h>
#ifdef USE_REMOTE
// Get at https://github.com/shirriff/Arduino-IRremote
#include <IRremote.h>
#endif

#ifdef ARDUINO_HW
// A regular Arduino. First string on pin 13, second on 12.
G35String lights_1(9, 49, 49, 0, false);
//G35String lights_2(12, 41);

#ifdef USE_REMOTE
IRrecv ir(11);  // IR receiver on pin 11.
decode_results ir_results;
#endif

#else
// A Teensy 2.0 that's been flashed with the Arduino Leonardo bootloader (thus
// permanently destroying the HalfKay bootloader that ships with the Teensy).
// The first string is connected to D6, and the second to B4.
G35String lights_1(12, 50, 50, 0, false);
G35String lights_2(8, 41);
#endif

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
#define PROGRAM_DURATION_SECONDS 180

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

#ifdef USE_REMOTE
  ir.enableIRIn();
#endif

  delay(50);
  lights_1.enumerate();
//  lights_2.enumerate();

  lights_1.do_test_patterns();
//  lights_2.do_test_patterns();

  string_group.AddString(&lights_1);
//string_group.AddString(&lights_2);
}

void loop() {
  runner.loop();

#ifdef USE_REMOTE
  if (ir.decode(&ir_results)) {
    runner.switch_program();
    ir.resume();
  }
#endif
}
