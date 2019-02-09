/*
 * LEDmatrix driver for ESP
 * 
 * Supported devices: MAX7219 https://github.com/squix78/MAX7219LedMatrix
 * 
 * TODO handle other display types (type var)
 *
 * Copyright 2019 Tamas Meszaros <mt+git@webit.hu>
 * This file licensed under Mozilla Public License v2.0 http://mozilla.org/MPL/2.0/
 */

#ifdef MOVED_TO_AGENT_DISPLAY_H

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