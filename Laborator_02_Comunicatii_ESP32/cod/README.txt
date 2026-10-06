Laborator 2 ESP32, 6 octombrie 2026
Deschideti fiecare .ino din propriul folder. Nu reuniti sketch-urile.
T1, T2, T4 si T5 folosesc lab_config.h, inclus in fiecare folder.
T3 necesita ESP32 clasic, nu S2/S3/C3.
T4 porneste in SIMULARE=true; nu trimite WhatsApp pana la configurare.
T5 necesita ReadyMail 0.4.2 si o parola de aplicatie valida.
TLS_DEMO_FARA_VALIDARE=true reproduce modul didactic simplificat: traficul este criptat, dar certificatul serverului nu este verificat. Pentru utilizare reala, instalati CA PEM corecta si folositi false.
Cod verificat prin inspectie si comparare cu API-urile; necompilat si netestat pe placa in acest mediu.
Nu publicati parolele sau cheile completate.

Credit ReadyMail: Suwatchai K. (Mobizt), https://github.com/mobizt/ReadyMail
Licenta consultata: CC BY-NC 4.0, https://creativecommons.org/licenses/by-nc/4.0/
Adaptari didactice: configurarea retelei laboratorului, trimitere manuala, timeout Wi-Fi/NTP, selectarea modului si verificarea fisierelor.

GitHub: copiati secrets.example.h ca secrets.h in fiecare proiect Wi-Fi si completati datele locale.
Licenta ReadyMail este in directorul cod, un nivel mai sus decat sketch-urile.
