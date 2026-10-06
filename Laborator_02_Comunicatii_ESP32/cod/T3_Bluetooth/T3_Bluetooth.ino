#include <Arduino.h>
#include "BluetoothSerial.h"
#if !defined(CONFIG_BT_SPP_ENABLED)
#error "Acest task necesita ESP32 clasic cu Bluetooth SPP."
#endif

BluetoothSerial BT;
const char* NUME_BT = "ESP32_L2_G01";
const int LED_PIN = 2;
const bool LED_ACTIV_HIGH = true;
bool ledPornit = false;

void comanda(char c) {
  if (c != '0' && c != '1' && c != '?') return;
  if (c == '0' || c == '1') {
    ledPornit = (c == '1');
    digitalWrite(LED_PIN, (ledPornit == LED_ACTIV_HIGH) ? HIGH : LOW);
  }
  String raspuns = String("LED=") + (ledPornit ? "ON" : "OFF");
  BT.println(raspuns);
  Serial.println(raspuns);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LED_ACTIV_HIGH ? LOW : HIGH);
  if (!BT.begin(NUME_BT)) {
    Serial.println("Pornirea Bluetooth a esuat.");
    return;
  }
  Serial.print("Asociati telefonul cu: "); Serial.println(NUME_BT);
  Serial.println("Comenzi telefon: 1=ON, 0=OFF, ?=stare.");
}

void loop() {
  // Punte USB -> Bluetooth: fiecare octet este transmis telefonului.
  while (Serial.available()) BT.write(Serial.read());
  // Bluetooth -> USB; in plus interpretam cele trei comenzi.
  while (BT.available()) {
    char c = (char)BT.read();
    Serial.write(c);
    comanda(c);
  }
  delay(5);
}
