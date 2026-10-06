#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <time.h>

#include "secrets.h"
const char* GRUPA = "G01";  // Fiecare echipa modifica.

bool conectareWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Conectare Wi-Fi");
  uint32_t t0 = millis();
  while (WiFi.status() != WL_CONNECTED && millis()-t0 < 20000) {
    delay(250);
    Serial.print('.');
  }
  Serial.println();
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Conectare esuata. Verificati reteaua si parola.");
    return false;
  }
  Serial.print("IP: "); Serial.println(WiFi.localIP());
  return true;
}

bool sincronizareOra() {
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  uint32_t t0 = millis();
  while (time(nullptr) < 1700000000 && millis()-t0 < 20000)
    delay(250);
  return time(nullptr) >= 1700000000;
}
