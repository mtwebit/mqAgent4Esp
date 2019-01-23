/*
 * Sensor drivers
 * 
 * Devices:
 *   BME280 https://learn.sparkfun.com/tutorials/sparkfun-bme280-breakout-hookup-guide
 *   DHT11, DHT22
 *   DS18B20 https://randomnerdtutorials.com/esp8266-ds18b20-temperature-sensor-web-server-with-arduino-ide/
 *   HC-SR501 https://www.instructables.com/id/Interface-PIR-Sensor-With-NodeMCU/
 *   Moisture http://www.electronicwings.com/sensors-modules/soil-moisture-sensor
 * 
 * TODO: avoid false positives on HC-SR501, use 3,3V mini PIRs instead
 * TODO dew point etc https://github.com/dpavlin/Arduino-projects/blob/master/DHT11_DS18B20_temperature/DHT11_DS18B20_temperature.ino
 * 
 * Copyright 2019 Tamas Meszaros <mt+git@webit.hu>
 * This file licensed under Mozilla Public License v2.0 http://mozilla.org/MPL/2.0/
 */

#include <AgentSensors.h>

// math helpers
#include <AgentUtils.h>

// Logging
#include <AgentLog.h>

void AgentSensors::setup() {
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
  pinMode(SENSOR_MOTION_PIN, INPUT);
  motion = (digitalRead(SENSOR_MOTION_PIN) == HIGH);

  dhtSensor.setup(DHT11_PIN);
  delay(dhtSensor.getMinimumSamplingPeriod());
  Log.notice("[SENSORS] DHT sensor status: %s", dhtSensor.getStatusString());
}

void AgentSensors::loop() {
}

const char * AgentSensors::getStatus() {
  return (const char *) "?";
  return dhtSensor.getStatusString();
}

float AgentSensors::getTemperature() {
  return ROUND_FLOAT(bmeSensor.readTempC(), 10);  // one decimal digit
  delay(dhtSensor.getMinimumSamplingPeriod());
  return dhtSensor.getTemperature();
}

float AgentSensors::getHumidity() {
  return ROUND_FLOAT(bmeSensor.readFloatHumidity(), 1); // no decimal digit
  delay(dhtSensor.getMinimumSamplingPeriod());
  return dhtSensor.getHumidity();
}

float AgentSensors::getPressure() {
  return ROUND_FLOAT(bmeSensor.readFloatPressure(), 1); // no decimal digit
}

bool AgentSensors::getMotion() {
  motion = (digitalRead(SENSOR_MOTION_PIN) == HIGH);
  return motion;
}

float AgentSensors::getMoisture() {
  return int(10*(100.00 - 100.00 * analogRead(SENSOR_MOISTURE_PIN)/1023.00))/10;   // one decimal digit
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
