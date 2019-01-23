/*
 * Header file for sensor drivers
 * 
 * Copyright 2019 Tamas Meszaros <mt+git@webit.hu>
 * This file licensed under Mozilla Public License v2.0 http://mozilla.org/MPL/2.0/
 */

#ifndef AGENT_SENSORS_H
#define AGENT_SENSORS_H

//#ifdef SENSOR_BME280
#include <Wire.h>
#include <SparkFunBME280.h>
//#endif

// #ifdef SENSOR_DHT
#include <DHT.h>
// Data wire is plugged into pin D1 on the ESP8266 12-E - GPIO 1
#ifndef DHT11_PIN
#define DHT11_PIN D1
#endif
// Sensor type
#ifndef DHTTYPE
#define DHTTYPE DHT11
#endif
// #endif

#ifndef SENSOR_MOTION_PIN
#define SENSOR_MOTION_PIN D2
#endif

#ifndef SENSOR_MOISTURE_PIN
#define SENSOR_MOISTURE_PIN A0
#endif

class AgentSensors {
  private:
  BME280 bmeSensor;
  DHT dhtSensor;
  long timer = 0;
  bool motion;
  
  public:
  AgentSensors() {};
  void setup();
  void loop();
  const char *getStatus();
  float getTemperature();
  float getHumidity();
  float getPressure();
  bool getMotion();
  bool wasMotion() { return motion; }
  void clearMotion() { motion = false; };
  float getMoisture();
};

#endif