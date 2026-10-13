#include <DHT.h>
constexpr uint8_t DHT_PIN = 4;
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);
void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("Task 1 - DHT11 pe GPIO 4");
  dht.begin();
}
void loop() {
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Eroare DHT: verificati firele si tipul DHT11.");
    return;
  }
  Serial.printf("T = %.1f C; RH = %.1f %%\n", t, h);
}
