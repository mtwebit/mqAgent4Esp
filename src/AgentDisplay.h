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

/************************************************
 * Implementations
 ************************************************/


AgentDisplay::AgentDisplay(byte device_type, int device_pin, int device_num) {
  this->device_type = device_type;
  this->device_pin = device_pin;
  this->ledMatrix = new LedMatrix(device_num, device_pin);
}

void AgentDisplay::setup(const short brightness) {
  ledMatrix->init();
  ledMatrix->setIntensity(brightness); // range is 0-15
  enabled = true;
}

void AgentDisplay::loop() {
  if (!enabled) return;
  if (millis() - timer < LEDMATRIX_DELAY) return;
  timer = millis();
  // ledMatrix->clear();
  ledMatrix->scrollTextLeft();
//  delay(LEDMATRIX_DELAY);
  ledMatrix->drawText();
  ledMatrix->commit();
}

void AgentDisplay::AddText(char *text, byte alignment /* = TEXT_ALIGN_LEFT_END*/, byte charWidth/* = LEDMATRIX_CHAR_WIDTH*/) {
  Log.notice("[DISP] Queuing text %s to display.", text);
  ledMatrix->setTextAlignment(alignment);
  ledMatrix->setCharWidth(charWidth);
  ledMatrix->setNextText(text);
  ledMatrix->drawText();
  ledMatrix->commit();
  enabled = true;
}

void AgentDisplay::SetText(char *text, byte alignment/* = TEXT_ALIGN_LEFT_END*/, byte charWidth/* = LEDMATRIX_CHAR_WIDTH*/) {
  Log.notice("[DISP] Setting text %s to display.", text);
  ledMatrix->clear();
  ledMatrix->setTextAlignment(alignment);
  ledMatrix->setCharWidth(charWidth);
  ledMatrix->setText(text);
  ledMatrix->drawText();
  ledMatrix->commit();
  enabled = true;
}

void AgentDisplay::off() {
  if (!enabled) return;
  Log.notice("[DISP] Turning off.");
  ledMatrix->clear();
  ledMatrix->commit();
  enabled = false;
}

void AgentDisplay::on() {
  if (enabled) return;
  Log.notice("[DISP] Turning on.");
  ledMatrix->drawText();
  ledMatrix->commit();
  enabled = true;
}


#endif