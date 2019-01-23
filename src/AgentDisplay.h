/*
 * LEDmatrix driver for ESP
 * 
 * Supported devices: MAX7219 https://github.com/squix78/MAX7219LedMatrix
 * 
 * Copyright 2019 Tamas Meszaros <mt+git@webit.hu>
 * This file licensed under Mozilla Public License v2.0 http://mozilla.org/MPL/2.0/
 */

#ifndef AGENT_DISPLAY_H
#define AGENT_DISPLAY_H

#include <SPI.h>
#include "LedMatrix.h"

#ifndef LEDMATRIX_DEVICES
#define LEDMATRIX_DEVICES 1
#endif
#ifndef LEDMATRIX_CHAR_WIDTH
#define LEDMATRIX_CHAR_WIDTH 7
#endif
#ifndef LEDMATRIX_DELAY
#define LEDMATRIX_DELAY 70
#endif
#ifndef LEDMATRIX_PIN
#define LEDMATRIX_PIN D8
#endif

#define DISPLAY_TYPE_LEDMATRIX 1

// Logging
#include <AgentLog.h>

class AgentDisplay {
  private:
  LedMatrix *ledMatrix;
  long timer = 0;
  bool enabled = true;
  
  public:
  byte device_type = DISPLAY_TYPE_LEDMATRIX;
  int device_pin;

  AgentDisplay(byte device_type, int device_pin, int device_num);
  void setup(const short brightness);
  void loop();
  void AddText(char *text, byte alignment = TEXT_ALIGN_LEFT_END, byte charWidth = LEDMATRIX_CHAR_WIDTH);
  void SetText(char *text, byte alignment = TEXT_ALIGN_LEFT_END, byte charWidth = LEDMATRIX_CHAR_WIDTH);
  void off();
  void on();
};

#endif