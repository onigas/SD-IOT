// Implementare didactica pe baza API-ului ReadyMail (Mobizt).
// Pentru licenta bibliotecii consultati LICENSE_ReadyMail.txt.
#include "lab_config.h"
#include <WiFiClientSecure.h>
#include <FS.h>
#include <LittleFS.h>
#define ENABLE_SMTP
#define ENABLE_DEBUG
#define ENABLE_FS
#include <ReadyMail.h>

const char* SMTP_HOST = "smtp.gmail.com";
const uint16_t SMTP_PORT = 465;
// 0=text simplu, 1=HTML, 2=HTML + TXT + PNG din LittleFS.
const int MOD_EMAIL = 0;
const bool TLS_DEMO_FARA_VALIDARE = true;
const char* CA_PEM = ""; // CA PEM pentru smtp.gmail.com in modul validat.
WiFiClientSecure tls;
SMTPClient smtp(tls);

void stareSMTP(SMTPStatus status) {
  Serial.println(status.text);
}

void fisierCitire(File& f, const char* nume,
                 readymail_file_operating_mode mod) {
  if (mod==readymail_file_mode_open_read) f=LittleFS.open(nume, "r");
}

void atasament(SMTPMessage& m, const char* nume,
               const char* mime, const char* cale) {
  Attachment a;
  a.filename=nume; a.name=nume; a.mime=mime;
  a.attach_file.callback=fisierCitire;
  a.attach_file.path=cale;
  m.attachments.add(a, attach_type_attachment);
}

void trimiteEmail() {
  if (String(PAROLA_APP)=="COMPLETEAZA_PAROLA_DE_APLICATIE") {
    Serial.println("Completati expeditorul, parola APP si destinatarul.");
    return;
  }
  if (WiFi.status()!=WL_CONNECTED) {
    Serial.println("Nu exista Wi-Fi."); return;
  }
  if (!sincronizareOra()) {
    Serial.println("NTP indisponibil. Verificati accesul la Internet."); return;
  }
  if (MOD_EMAIL==2) {
    // false: NU formata automat; trebuie pastrate fisierele incarcate.
    if (!LittleFS.begin(false)) {
      Serial.println("LittleFS indisponibil. Incarcati folderul data."); return;
    }
    if (!LittleFS.exists("/text_file.txt") || !LittleFS.exists("/image.png")) {
      Serial.println("Lipseste text_file.txt sau image.png din LittleFS."); return;
    }
  }
  if (TLS_DEMO_FARA_VALIDARE) tls.setInsecure();
  else {
    if (!strlen(CA_PEM)) { Serial.println("Lipseste CA PEM."); return; }
    tls.setCACert(CA_PEM);
  }
  smtp.connect(SMTP_HOST, SMTP_PORT, stareSMTP);
  if (!smtp.isConnected()) { tls.stop(); return; }
  smtp.authenticate(EXPEDITOR, PAROLA_APP, readymail_auth_password);
  if (!smtp.isAuthenticated()) { tls.stop(); return; }
  SMTPMessage m;
  m.headers.add(rfc822_from, String("ESP32 <")+EXPEDITOR+">");
  m.headers.add(rfc822_to, String("Student <")+DESTINATAR+">");
  m.headers.add(rfc822_subject, String("Laborator 2 - ")+GRUPA);
  String corp=String("Mesaj de laborator, grupa ")+GRUPA+". ESP32 functioneaza.";
  if (MOD_EMAIL==0) m.text.body(corp);
  else m.html.body(String("<html><body><h2>Raport ESP32</h2><p>")+
                   corp+"</p><p><b>Status: OK</b></p></body></html>");
  if (MOD_EMAIL==2) {
    atasament(m, "text_file.txt", "text/plain", "/text_file.txt");
    atasament(m, "image.png", "image/png", "/image.png");
  }
  m.timestamp=time(nullptr);
  bool ok=smtp.send(m);  // Mod sincron: asteapta rezultatul.
  Serial.println(ok ? "SMTP: trimitere reusita. Verificati Inbox/Spam."
                    : "SMTP: trimitere esuata. Consultati jurnalul.");
  tls.stop();
}

void setup() {
  Serial.begin(115200);
  conectareWiFi();
  Serial.println("Trimiteti E pentru UN email. Pauza minima: 60 s.");
}

void loop() {
  static uint32_t ultima=0;
  static bool incercat=false;
  if (Serial.available()) {
    char c=Serial.read();
    if (c!='E' && c!='e') return;
    if (incercat && millis()-ultima<60000) {
      Serial.println("Asteptati 60 s."); return;
    }
    ultima=millis(); incercat=true; trimiteEmail();
  }
}
