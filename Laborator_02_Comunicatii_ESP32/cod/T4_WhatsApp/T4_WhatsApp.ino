#include "lab_config.h"
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

// true: exercitiu local, NU se trimite un mesaj WhatsApp.
const bool SIMULARE = true;
// Optiunea tutorialului, numai pentru demonstratie controlata.
const bool TLS_DEMO_FARA_VALIDARE = true;
const char* CA_PEM = "";  // Certificatul CA PEM, pentru validare TLS.

String codificaURL(const String& s) {
  const char hex[] = "0123456789ABCDEF";
  String out;
  for (size_t i=0; i<s.length(); i++) {
    uint8_t c = (uint8_t)s[i];
    if ((c>='A' && c<='Z') || (c>='a' && c<='z') ||
        (c>='0' && c<='9') || c=='-' || c=='_' || c=='.' || c=='~')
      out += (char)c;
    else {
      out += '%'; out += hex[c>>4]; out += hex[c & 15];
    }
  }
  return out;
}

void trimite() {
  String mesaj = String("Laborator 2 - ") + GRUPA + ": Salut de la ESP32!";
  if (SIMULARE) {
    Serial.println("SIMULARE: niciun mesaj nu a fost trimis.");
    Serial.println("Gazda: api.callmebot.com, resursa: /whatsapp.php");
    Serial.print("text="); Serial.println(codificaURL(mesaj));
    Serial.println("phone si apikey: ascunse. Configurati pentru test real.");
    return;
  }
  if (String(API_KEY)=="COMPLETEAZA_CHEIA" || String(TELEFON).indexOf('X')>=0) {
    Serial.println("Completati telefonul si cheia API."); return;
  }
  if (WiFi.status()!=WL_CONNECTED) {
    Serial.println("Nu exista conexiune Wi-Fi."); return;
  }
  WiFiClientSecure tls;
  if (TLS_DEMO_FARA_VALIDARE) tls.setInsecure();
  else {
    if (!strlen(CA_PEM) || !sincronizareOra()) {
      Serial.println("Validarea TLS necesita CA PEM si ora corecta."); return;
    }
    tls.setCACert(CA_PEM);
  }
  String url = "https://api.callmebot.com/whatsapp.php?phone=";
  url += codificaURL(TELEFON);
  url += "&text=" + codificaURL(mesaj);
  url += "&apikey=" + codificaURL(API_KEY);
  HTTPClient http;
  http.setTimeout(15000);
  if (!http.begin(tls, url)) {
    Serial.println("Initializare HTTP esuata."); return;
  }
  int cod = http.GET();
  Serial.printf("Rezultat HTTP: %d\n", cod);
  if (cod==200)
    Serial.println("Cerere acceptata HTTP; verificati primirea in WhatsApp.");
  else
    Serial.println("Cerere nereusita; verificati reteaua, cheia si serviciul.");
  http.end();
}

void setup() {
  Serial.begin(115200);
  if (!SIMULARE) conectareWiFi();
  Serial.println("Trimiteti W din Serial Monitor. Pauza minima: 60 s.");
}

void loop() {
  static bool trimis = false;
  static uint32_t ultima = 0;
  if (Serial.available()) {
    char c = Serial.read();
    if (c!='W' && c!='w') return;
    if (trimis && millis()-ultima < 60000) {
      Serial.println("Asteptati 60 s intre incercari."); return;
    }
    ultima=millis(); trimis=true; trimite();
  }
}
