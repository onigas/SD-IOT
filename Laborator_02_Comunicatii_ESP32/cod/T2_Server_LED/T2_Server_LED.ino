#include "lab_config.h"
#include <WebServer.h>

const int output26 = 2;  // Adaptarea ceruta: LED-ul de pe placa.
const bool LED_ACTIV_HIGH = true;
bool ledPornit = false;
WebServer server(80);

void seteazaLED(bool pornit) {
  ledPornit = pornit;
  digitalWrite(output26, (pornit == LED_ACTIV_HIGH) ? HIGH : LOW);
  Serial.printf("GPIO 2: %s\n", pornit ? "ON" : "OFF");
}

String pagina() {
  String h = "<!doctype html><html lang='ro'><meta charset='utf-8'>";
  h += "<meta name='viewport' content='width=device-width,initial-scale=1'>";
  h += "<title>ESP32 - laborator 2</title>";
  h += "<style>body{font:22px Arial;text-align:center;margin:40px}";
  h += "button{font-size:24px;padding:18px;margin:12px}</style>";
  h += "<h1>ESP32 - "; h += GRUPA; h += "</h1>";
  h += "<p>LED incorporat: GPIO 2</p><p>Stare: <b>";
  h += ledPornit ? "ON" : "OFF";
  h += "</b></p><form action='/2/on' method='post'>";
  h += "<button>PORNESTE</button></form>";
  h += "<form action='/2/off' method='post'>";
  h += "<button>OPRESTE</button></form></html>";
  return h;
}

void redirectioneaza() {
  server.sendHeader("Location", "/");
  server.send(303, "text/plain", "Revenire la pagina principala");
}

void setup() {
  Serial.begin(115200);
  pinMode(output26, OUTPUT);
  seteazaLED(false);
  if (!conectareWiFi()) return;
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Cache-Control", "no-store");
    server.send(200, "text/html; charset=utf-8", pagina());
  });
  server.on("/2/on", HTTP_POST, []() {
    seteazaLED(true); redirectioneaza();
  });
  server.on("/2/off", HTTP_POST, []() {
    seteazaLED(false); redirectioneaza();
  });
  server.onNotFound([]() { server.send(404, "text/plain", "Ruta inexistenta"); });
  server.begin();
  Serial.print("Deschideti http://"); Serial.println(WiFi.localIP());
}

void loop() {
  server.handleClient();
}
