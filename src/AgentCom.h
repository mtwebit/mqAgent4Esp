/*
 * Agent communication functions
 * 
 * Using https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html
 * Using https://github.com/knolleary/pubsubclient
 * Using https://github.com/arduino-libraries/NTPClient
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

/************************************************
 * Implementations
 ************************************************/

void AgentCom::setup(const char *mqttServer, const int mqttPort) {
  WiFi.persistent(false);
  Log.notice("[MQTT] Setting server address to %s", mqttServer);
  mqttClient.setClient(wifiClient);
  mqttClient.setServer(mqttServer, mqttPort);
  // mqttClient.setCallback(callback);
  Log.notice("[NTP] Setting up NTP client.");
  ntpClient.begin();
}

bool AgentCom::connect(const char *cName, const char *ssid, const char *password) {
  clientName = cName;
  Log.notice("[WIFI] Connecting to %s", ssid);
  if (WiFi.begin(ssid, password) != WL_CONNECTED) delay(1000);
  if (WiFi.status() == WL_CONNECTED) {
    Log.notice("[WIFI] Connected.");
    // TODO convert IPAddress to char * and log it "[WIFI] IP address: " + WiFi.localIP());
    if (!mqttClient.connected()) {
      Log.notice("[MQTT] Connecting as %s.", cName);
      if (!mqttClient.connect(clientName)) {   // this may block for a couple of seconds
        Log.notice("[MQTT] connection failed, rc=%i.", mqttClient.state());
        return false;
      }
      Log.notice("[MQTT] Connected.");
    }
    return true;
  }
  Log.error("[WIFI] failed to connect to %s.", ssid);
  return false;
}

bool AgentCom::reconnect() {
  while (!WiFi.status() != WL_CONNECTED) {
    if (!WiFi.reconnect()) {
      Log.fatal("[WIFI] Failed to reconnect.");
      return false;
    }
    if (!WiFi.isConnected()) delay(500);
  }
  Log.notice("[WIFI] Reconnected.");
  while (!mqttClient.connected()) {
    mqttClient.connect(clientName);
    if (!mqttClient.connected()) delay(500);
  }
  Log.notice("[MQTT] Reconnected.");
  if (!ntpClient.update()) {
    Log.error("[NTP] Failed to update the time.");
  }
  return true;
}

bool AgentCom::publish(const char *topic, char *payload) {
  Log.notice("[MQTT] Sending '%s' to topic '%s'.", payload, topic);
  return mqttClient.publish(topic, payload);
}

void AgentCom::shutdown() {
  // TODO mqtt disconnect
  Log.notice("[WIFI] Disconnecting.");
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}

bool AgentCom::isConnected() {
  return WiFi.isConnected() && mqttClient.connected();
}

/**
 * Send and receive messages
 */
void AgentCom::loop() {
  mqttClient.loop();
  if (millis() - timer < NTP_DELAY) return;
  timer = millis();
  Log.notice("[NTP] Updating time from the server.");
  if (!ntpClient.update()) {
    Log.error("[NTP] Failed to update the time.");
  }
}

#endif