#pragma once
#include <WiFi.h>
// Completati local. Nu publicati parola in repository.
const char* WIFI_SSID = "NUMELE_RETELEI";
const char* WIFI_PASS = "PAROLA_RETELEI";
bool conectareWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Conectare Wi-Fi");
  const uint32_t start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 20000) {
    delay(250); Serial.print('.');
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nWi-Fi indisponibil. Verificati SSID/parola; RESET.");
    return false;
  }
  Serial.print("\nDeschideti http://"); Serial.println(WiFi.localIP());
  return true;
}
