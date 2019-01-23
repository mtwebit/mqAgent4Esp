/*
 * Agent communication functions
 * 
 * Using https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html
 * Using https://github.com/knolleary/pubsubclient
 * 
 * Copyright 2019 Tamas Meszaros <mt+git@webit.hu>
 * This file licensed under Mozilla Public License v2.0 http://mozilla.org/MPL/2.0/
 */

#ifndef AGENT_COM_H
#define AGENT_COM_H

// Wifi
#include <ESP8266WiFi.h>
#define FPM_SLEEP_MAX_TIME 0xFFFFFFF

// MQTT
#include <PubSubClient.h>

// Logging
#include <AgentLog.h>

// Time
#define NTP_DELAY 1080000 // 5 minutes
#include <NTPClient.h>
#include <WiFiUdp.h>

class AgentCom {
  private:
  WiFiClient wifiClient;
  PubSubClient mqttClient;  // See https://platformio.org/lib/show/89/PubSubClient
  const char *clientName;
  WiFiUDP ntpUDP;
  long timer = NTP_DELAY;

  public:
  NTPClient ntpClient = NTPClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);
  AgentCom() { }
  void setup(const char *mqttServer, const int mqttPort);
  bool connect(const char *clientName, const char *ssid, const char *password);
  bool reconnect();
  bool publish(const char *topic, char *payload);
  void shutdown();
  bool isConnected();
  void loop();
};

#endif