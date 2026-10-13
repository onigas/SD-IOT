# Sisteme dedicate pentru IoT

Materiale de laborator pentru ESP32, Raspberry Pi și aplicații Internet of Things (IoT).

## Laboratoare disponibile

| Laborator | Temă | Îndrumător | Cod sursă |
| --- | --- | --- | --- |
| **2** | Comunicații cu ESP32: Wi-Fi, HTTP, Bluetooth, WhatsApp și e-mail | [Laboratorul 2](Laborator_02_Comunicatii_ESP32/README.md) | [Proiecte Arduino](Laborator_02_Comunicatii_ESP32/cod/) |
| **3** | Senzori și interfețe web cu ESP32: DHT11, PIR, MPU6050/GY-87 și vizualizare 3D | [Îndrumător online](Laborator_03_Senzori_ESP32/documentatie/Laborator_03.md) · [PDF](Laborator_03_Senzori_ESP32/documentatie/Laborator_03.pdf) | [Cele 5 proiecte Arduino](Laborator_03_Senzori_ESP32/cod/) |

### Laboratorul 3 — Senzori și interfețe web cu ESP32

Exercițiile sunt organizate în cinci proiecte independente:

1. [T1 — DHT11: măsurarea temperaturii și umidității](Laborator_03_Senzori_ESP32/cod/T1_DHT11/)
2. [T2 — DHT11: afișarea măsurătorilor printr-un server web](Laborator_03_Senzori_ESP32/cod/T2_DHT_Web/)
3. [T3 — Senzor PIR: detectarea mișcării](Laborator_03_Senzori_ESP32/cod/T3_PIR/)
4. [T4 — GY-87 / MPU6050: măsurarea accelerației și rotației](Laborator_03_Senzori_ESP32/cod/T4_GY87/)
5. [T5 — MPU6050: server web și vizualizare 3D](Laborator_03_Senzori_ESP32/cod/T5_MPU_Web/)

**[Deschideți pagina completă a Laboratorului 3](Laborator_03_Senzori_ESP32/README.md)** pentru cerințe, conexiuni, instalarea bibliotecilor și utilizarea LittleFS.

## Utilizare

1. Descărcați repository-ul prin **Code → Download ZIP** sau clonați-l:

   ```bash
   git clone https://github.com/onigas/SD-IOT.git
   ```

2. Deschideți README-ul laboratorului ales și urmați instrucțiunile de instalare și conectare.
3. Deschideți fiecare proiect Arduino separat, din propriul director:
   - **Laboratorul 2:** copiați `secrets.example.h` ca `secrets.h` și completați configurația locală.
   - **Laboratorul 3:** pentru T2 și T5 completați local `lab_config.h` cu datele rețelei; pentru T5 încărcați separat și directorul `data/` în LittleFS.

**Securitate:** nu publicați parole Wi-Fi, tokenuri sau chei API în repository.

Materialele fiecărui laborator sunt organizate în directoare separate, cu documentație, resurse și cod sursă.
