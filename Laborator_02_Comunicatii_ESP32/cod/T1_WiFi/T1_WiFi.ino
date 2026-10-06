#include "lab_config.h"

void setup() {
  Serial.begin(115200);
  delay(500);
  if (!conectareWiFi()) return;
  Serial.print("SSID: "); Serial.println(WiFi.SSID());
  Serial.print("MAC: "); Serial.println(WiFi.macAddress());
  Serial.print("Masca: "); Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
  Serial.print("DNS: "); Serial.println(WiFi.dnsIP());
  Serial.print("Canal: "); Serial.println(WiFi.channel());
}

void loop() {
  static uint32_t ultima = 0;
  if (millis()-ultima >= 3000) {
    ultima = millis();
    if (WiFi.status() == WL_CONNECTED)
      Serial.printf("RSSI: %d dBm\n", WiFi.RSSI());
    else
      Serial.println("Wi-Fi deconectat; apasati EN pentru retestare.");
  }
}
