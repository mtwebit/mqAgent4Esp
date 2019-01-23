/*
 * Logging functions based on https://github.com/thijse/Arduino-Log
 * 
 * Copyright 2019 Tamas Meszaros <mt+git@webit.hu>
 * This file licensed under Mozilla Public License v2.0 http://mozilla.org/MPL/2.0/
 */

#ifndef AGENT_LOG_H
#define AGENT_LOG_H

#include <ArduinoLog.h>
#include "HardwareSerial.h"

class AgentLog {
  public:
  AgentLog() {};
  void begin(const int log_level, Print *output);
};

#endif