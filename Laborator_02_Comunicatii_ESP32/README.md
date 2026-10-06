# Laboratorul 2 · Comunicații cu ESP32

Cinci aplicații practice, cu explicații în română, scheme, cod complet și probe de verificare.

## Documentație

- [Îndrumător PDF cu cuprins clicabil](documentatie/Laborator_02.pdf)
- [Document Word editabil](documentatie/Laborator_02.docx)
- [Scheme explicative](imagini/)

## Sarcini

| Task | Obiectiv | Proiect |
|---|---|---|
| 1 | Conectare Wi-Fi, IP, gateway, DNS și RSSI | [T1_WiFi](cod/T1_WiFi/) |
| 2 | Server HTTP pentru LED-ul de pe GPIO 2 | [T2_Server_LED](cod/T2_Server_LED/) |
| 3 | Bluetooth Classic SPP și comenzi 1 / 0 / ? | [T3_Bluetooth](cod/T3_Bluetooth/) |
| 4 | Notificare WhatsApp prin CallMeBot | [T4_WhatsApp](cod/T4_WhatsApp/) |
| 5 | E-mail text, HTML și atașamente TXT + PNG | [T5_Email](cod/T5_Email/) |

## Pregătire

- ESP32 **clasic**, de exemplu ESP32-WROOM-32 / DOIT ESP32 DEVKIT V1; cablu USB de date.
- Arduino IDE 2.x și platforma **esp32 by Espressif Systems**. Serial Monitor: **115200 baud**.
- Pentru taskul 3: telefon Android și terminal Bluetooth SPP. Nu folosiți ESP32-S2/S3/C3 pentru acest task.
- Pentru taskul 5: biblioteca **ReadyMail de Mobizt**; API-ul utilizat a fost verificat în versiunea **0.4.2**. Pentru atașamente: IDE 2.2.1+ și [LittleFS Uploader](https://github.com/earlephilhower/arduino-littlefs-upload).

## Configurare și rulare

1. Deschideți `.ino` din directorul taskului. Fiecare task este un program separat; nu combinați sketch-urile.
2. În T1, T2, T4 și T5, copiați `secrets.example.h` ca **`secrets.h`**, lângă `.ino`.
3. Completați în `secrets.h` SSID-ul și parola comunicate de cadrul didactic. Pentru T4 completați telefonul și cheia API; pentru T5 adresele de e-mail și parola de aplicație.
4. În `lab_config.h`, înlocuiți `G01` cu grupa. Pentru Bluetooth modificați `NUME_BT` în sketch.
5. Selectați placa și portul, apăsați Verify, apoi Upload. Consultați PDF-ul pentru explicații și probe.

**Adaptarea taskului 2:** `const int output26 = 2;` comandă LED-ul existent pe placă. Canalul GPIO 27 a fost eliminat. Nu sunt necesare LED-uri sau rezistoare externe. Verificați polaritatea LED-ului modelului utilizat.

## Taskul 4: disponibilitatea CallMeBot

La verificarea din **6 octombrie 2026**, [pagina oficială](https://www.callmebot.com/blog/free-api-whatsapp-messages/) anunța indisponibilitatea activărilor noi. Verificați situația actuală înainte de laborator.

- `SIMULARE = true` este valoarea inițială: se verifică local construirea cererii, fără trimitere WhatsApp.
- Cu o cheie validă existentă, completați `secrets.h`, setați `SIMULARE = false` și trimiteți **W** din Serial Monitor.
- Primirea efectivă pe telefon confirmă livrarea; simularea nu este o notificare trimisă.

## Taskul 5: e-mail și atașamente

- Completați parola de **aplicație**, nu parola obișnuită a contului.
- `MOD_EMAIL = 0`: text; `1`: HTML; `2`: HTML cu atașamente.
- Pentru modul 2 încărcați separat directorul [data](cod/T5_Email/data/) în LittleFS. Închideți Serial Monitor, apoi folosiți Ctrl+Shift+P → Upload LittleFS to Pico/ESP8266/ESP32.
- Trimiteți **E** pentru o singură expediere. Verificați Inbox, Spam și ambele atașamente.

## Limite și validare

Codul a fost inspectat și confruntat cu API-urile, dar **nu a fost compilat sau testat pe placă** în mediul în care a fost pregătit. Efectuați probele din îndrumător și notați versiunile software.

T4 și T5 includ modul didactic `TLS_DEMO_FARA_VALIDARE = true`: transportul este criptat, dar certificatul serverului nu este verificat. Pentru utilizare reală, configurați certificatul CA PEM corect și setați opțiunea `false`.

## Surse

- [Server web ESP32](https://randomnerdtutorials.com/esp32-web-server-arduino-ide/)
- [Bluetooth Classic](https://randomnerdtutorials.com/esp32-bluetooth-classic-arduino-ide/)
- [WhatsApp](https://randomnerdtutorials.com/esp32-send-messages-whatsapp/)
- [E-mail SMTP](https://randomnerdtutorials.com/esp32-send-email-smtp-server-arduino-ide/)
- [ReadyMail](https://github.com/mobizt/ReadyMail) — Suwatchai K. (Mobizt); [licența consultată](cod/LICENSE_ReadyMail.txt).

Explicațiile și schemele sunt proprii; PDF-ul include bibliografia completă. Pentru ReadyMail se păstrează atribuirea și condițiile licenței autorului. Nu se atribuie o licență generală întregului repository prin această încărcare.
