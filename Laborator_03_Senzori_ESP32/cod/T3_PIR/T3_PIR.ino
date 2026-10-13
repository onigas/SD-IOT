#include <Arduino.h>
#include "driver/gpio.h"
constexpr uint8_t PIR_PIN = 4, LED_PIN = 2;
constexpr bool LED_ACTIV_HIGH = true;
constexpr uint32_t timeSeconds = 5;
// Testati pe rand: RISING, FALLING, CHANGE, ONLOW, ONHIGH.
constexpr int MOD = RISING;
constexpr bool NIVEL = (MOD == ONLOW || MOD == ONHIGH);
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
volatile bool eveniment = false;
bool ledAprins = false, rearmare = false;
uint32_t ultimulEveniment = 0, momentRearmare = 0;
void ARDUINO_ISR_ATTR detectie() {
  // La nivel, oprim sursa pana la rearmarea din loop().
  if (NIVEL) gpio_intr_disable((gpio_num_t)PIR_PIN);
  portENTER_CRITICAL_ISR(&mux);
  eveniment = true;
  portEXIT_CRITICAL_ISR(&mux);
}
void led(bool on) {
  digitalWrite(LED_PIN, (on == LED_ACTIV_HIGH) ? HIGH : LOW);
  ledAprins = on;
}
void setup() {
  Serial.begin(115200); delay(1000);
  pinMode(LED_PIN, OUTPUT); led(false);
  pinMode(PIR_PIN, INPUT);
  Serial.println("Task 3: astept stabilizarea PIR (60 s).");
  delay(60000);
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), detectie, MOD);
  Serial.println("PIR activ. Observati OUT si LED.");
}
void loop() {
  portENTER_CRITICAL(&mux);
  bool e = eveniment; eveniment = false;
  portEXIT_CRITICAL(&mux);
  const uint32_t acum = millis();
  if (e) {
    led(true); ultimulEveniment = acum;
    Serial.printf("Eveniment; OUT=%d; t=%lu ms\n",
      digitalRead(PIR_PIN), (unsigned long)acum);
    if (NIVEL) { rearmare = true; momentRearmare = acum; }
  }
  if (ledAprins && acum - ultimulEveniment >= timeSeconds * 1000UL) {
    led(false); Serial.println("Temporizare expirata: LED OFF");
  }
  if (rearmare && acum - momentRearmare >= 500) {
    rearmare = false; gpio_intr_enable((gpio_num_t)PIR_PIN);
  }
}
