#include "lab_config.h"
#include <DHT.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
DHT dht(4, DHT11);
AsyncWebServer server(80);
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
float temperatura = 0, umiditate = 0;
bool valid = false;
uint32_t ultimaCitire = 0;
const char pagina[] PROGMEM = R"HTML(
<!doctype html><html lang="ro"><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>DHT11 - Laborator 3</title>
<style>body{font:22px Arial;margin:3rem;line-height:1.6}</style>
<h1>ESP32 + DHT11</h1><p id="valori">Astept citirea...</p>
<script>
async function actualizeaza(){
 const e=document.getElementById('valori');
 try {
  const r=await fetch('/date',{cache:'no-store'});
  if(!r.ok) throw new Error('Senzor indisponibil');
  const d=await r.json();
  e.textContent=`Temperatura: ${d.t} °C; umiditate: ${d.h} %`;
 } catch(err){e.textContent=err.message;}
}
actualizeaza();setInterval(actualizeaza,2000);
</script></html>)HTML";
void setup() {
  Serial.begin(115200); delay(1000);
  Serial.println("Task 2 - DHT11 web"); dht.begin();
  if (!conectareWiFi()) return;
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* r) {
    r->send(200, "text/html; charset=utf-8", pagina);
  });
  server.on("/date", HTTP_GET, [](AsyncWebServerRequest* r) {
    portENTER_CRITICAL(&mux);
    float t = temperatura, h = umiditate; bool ok = valid;
    portEXIT_CRITICAL(&mux);
    if (!ok) { r->send(503, "text/plain", "Senzor indisponibil"); return; }
    String json = "{\"t\":" + String(t,1) + ",\"h\":" + String(h,1) + "}";
    r->send(200, "application/json", json);
  });
  server.begin();
}
void loop() {
  if (millis() - ultimaCitire < 2000) return;
  ultimaCitire = millis();
  float t = dht.readTemperature(), h = dht.readHumidity();
  bool ok = !isnan(t) && !isnan(h);
  portENTER_CRITICAL(&mux);
  valid = ok;
  if (ok) { temperatura = t; umiditate = h; }
  portEXIT_CRITICAL(&mux);
  if (ok) Serial.printf("T=%.1f C; RH=%.1f %%\n",t,h);
  else Serial.println("Eroare DHT11");
}
