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


/************************************************
 * Implementations
 ************************************************/

// math helpers
#include <AgentUtils.h>

// Logging
#include <AgentLog.h>

void AgentSensors::setup() {
  #ifdef SENSOR_BME280
  bmeSensor.settings.commInterface = I2C_MODE;
  bmeSensor.settings.I2CAddress = 0x76;
  delay(5);  // BME280 requires 2ms to start up.
  Wire.begin(D3, D4); // Wire.begin(sda, scl)
  //runMode can be:
    //  0, Sleep mode
    //  1 or 2, Forced mode
    //  3, Normal mode
  bmeSensor.settings.runMode = 3;
  if (bmeSensor.beginI2C() == false) {
    Log.fatal("ERROR: The sensor did not respond. Please check wiring.");
    ESP.deepSleep(0);
  }
  #endif
  #ifdef SENSOR_MOTION
  pinMode(SENSOR_MOTION_PIN, INPUT);
  motion = (digitalRead(SENSOR_MOTION_PIN) == HIGH);
  #endif
  #ifdef SENSOR_DHT11
  dhtSensor.setup(DHT11_PIN);
  delay(dhtSensor.getMinimumSamplingPeriod());
  Log.notice("[SENSORS] DHT sensor status: %s", dhtSensor.getStatusString());
  #endif
}

void AgentSensors::loop() {
}

const char * AgentSensors::getStatus() {
  #ifdef SENSOR_DHT11
  return dhtSensor.getStatusString();
  #endif
  return (const char *) "?";
}

float AgentSensors::getTemperature() {
  #ifdef SENSOR_BME280
  return ROUND_FLOAT(bmeSensor.readTempC(), 10);  // one decimal digit
  #endif
  #ifdef SENSOR_DHT11
  delay(dhtSensor.getMinimumSamplingPeriod());
  return dhtSensor.getTemperature();
  #endif
}

float AgentSensors::getHumidity() {
  #ifdef SENSOR_BME280
  return ROUND_FLOAT(bmeSensor.readFloatHumidity(), 1); // no decimal digit
  #endif
  #ifdef SENSOR_DHT11
  delay(dhtSensor.getMinimumSamplingPeriod());
  return dhtSensor.getHumidity();
  #endif
}

float AgentSensors::getPressure() {
  #ifdef SENSOR_BME280
  return ROUND_FLOAT(bmeSensor.readFloatPressure(), 1); // no decimal digit
  #endif
}

bool AgentSensors::getMotion() {
  #ifdef SENSOR_MOTION
  motion = (digitalRead(SENSOR_MOTION_PIN) == HIGH);
  return motion;
  #endif
}

float AgentSensors::getMoisture() {
  #ifdef SENSOR_MOISTURE
  return int(10*(100.00 - 100.00 * analogRead(SENSOR_MOISTURE_PIN)/1023.00))/10;   // one decimal digit
  #endif
}

  // return bme.readFloatPressure();
  // return bme.readFloatAltitudeMeters();
  // return bme.readFloatAltitudeFeet();
  // return bme.readTempF();

/*
#elif SENSOR_DS18B20
// TODO https://randomnerdtutorials.com/esp8266-ds18b20-temperature-sensor-web-server-with-arduino-ide/
/*
#include <OneWire.h>
#include <DallasTemperature.h>

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(DHT11_PIN);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device addresses
DeviceAddress insideThermometer, outsideThermometer;

char temperatureCString[7];
char temperatureFString[7];

float sensors_get_temperature() {
  float tempC;
  float tempF;
  do {
    sensors.requestTemperatures(); 
    tempC = sensors.getTempCByIndex(0);
    dtostrf(tempC, 2, 2, temperatureCString);
    tempF = sensors.getTempFByIndex(0);
    dtostrf(tempF, 3, 2, temperatureFString);
    delay(100);
//    Serial.print("*");
  } while (tempC == 85.0 || tempC == (-127.0));
  return tempC;
}

void sensors_setup(void) {
  // Start up the library
  sensors.begin();
  Log.notice("[SENSORS] Found %i sensors", sensors.getDeviceCount());
}
*/

#endif