# Sisteme dedicate pentru IoT

Materiale de laborator: ESP32, Raspberry Pi și proiecte IoT.

## Laboratoare disponibile

| Laborator | Temă | Materiale |
|---|---|---|
| 2 | Comunicații cu ESP32: Wi-Fi, HTTP, Bluetooth, WhatsApp și e-mail | [Îndrumător și cod](Laborator_02_Comunicatii_ESP32/README.md) |
| 3 | Senzori și interfețe web: DHT11, PIR, MPU6050/GY-87 și vizualizare 3D | [Îndrumător, cod și PDF corectat](Laborator_03_Senzori_ESP32/README.md) |

## Utilizare

1. Descărcați repository-ul prin **Code → Download ZIP** sau clonați-l:

```bash
git clone https://github.com/onigas/SD-IOT.git
```

2. Urmați README-ul laboratorului ales.
3. Deschideți fiecare proiect Arduino separat și urmați configurarea indicată în îndrumător.
   - **Laboratorul 2:** copiați `secrets.example.h` ca `secrets.h` și completați valorile locale.
   - **Laboratorul 3:** pentru T2 și T5 completați local `lab_config.h`; pentru T5 încărcați și folderul `data/` în LittleFS.

Parolele și cheile API nu se publică. Fișierele `secrets.h` sunt excluse prin `.gitignore`.

Fiecare laborator are propriul director, cu documentație și proiecte Arduino independente.
