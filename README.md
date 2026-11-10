# Sisteme dedicate pentru IoT

Materiale de laborator pentru ESP32, Raspberry Pi și aplicații Internet of Things (IoT).

## Cuprins

- [Partea I — Sisteme cu ESP32](#esp32)
- [Partea a II-a — Sisteme cu Raspberry Pi](#raspberry-pi)
- [Partea a III-a — Proiect de sistem dedicat pentru IoT](#proiect)

<a id="esp32"></a>

## Partea I — Sisteme cu ESP32

<a id="laboratoare-disponibile"></a>

### Laboratoare și materiale

| Laborator | Temă | Îndrumător | Cod sursă |
| --- | --- | --- | --- |
| **1** | Introducere în ESP32. Configurarea plăcii, GPIO și scanarea rețelelor Wi-Fi | — | — |
| **2** | Comunicații cu ESP32: Wi-Fi, HTTP, Bluetooth, WhatsApp și e-mail | [Laboratorul 2](Laborator_02_Comunicatii_ESP32/README.md) | [Proiecte Arduino](Laborator_02_Comunicatii_ESP32/cod/) |
| **3** | Senzori și interfețe web cu ESP32: DHT11, PIR, MPU6050/GY-87 și vizualizare 3D | [Îndrumător online](Laborator_03_Senzori_ESP32/documentatie/Laborator_03.md) · [PDF](Laborator_03_Senzori_ESP32/documentatie/Laborator_03.pdf) | [Cele 5 proiecte Arduino](Laborator_03_Senzori_ESP32/cod/) |
| **4** | Interfețe web și rețele ESP32. Grafice, punct de acces și ESP-MESH | — | — |
| **5** | MQTT pe ESP32. Publicarea datelor de la senzori printr-un broker public sau local | — | — |
| **6** | ESP-NOW: comunicații directe între ESP32, comparație cu ESP-MESH și gateway Wi-Fi | [Introducere teoretică](Laborator_06_ESP_NOW/documentatie/Laborator_06.md) | [Prezentarea laboratorului](Laborator_06_ESP_NOW/README.md) |

### Laboratorul 3 — Senzori și interfețe web cu ESP32

Exercițiile sunt organizate în cinci proiecte independente:

1. [T1 — DHT11: măsurarea temperaturii și umidității](Laborator_03_Senzori_ESP32/cod/T1_DHT11/)
2. [T2 — DHT11: afișarea măsurătorilor printr-un server web](Laborator_03_Senzori_ESP32/cod/T2_DHT_Web/)
3. [T3 — Senzor PIR: detectarea mișcării](Laborator_03_Senzori_ESP32/cod/T3_PIR/)
4. [T4 — GY-87 / MPU6050: măsurarea accelerației și rotației](Laborator_03_Senzori_ESP32/cod/T4_GY87/)
5. [T5 — MPU6050: server web și vizualizare 3D](Laborator_03_Senzori_ESP32/cod/T5_MPU_Web/)

**[Deschideți pagina completă a Laboratorului 3](Laborator_03_Senzori_ESP32/README.md)** pentru cerințe, conexiuni, instalarea bibliotecilor și utilizarea LittleFS.

### Laboratorul 6 — ESP-NOW

Laboratorul abordează protocolul ESP-NOW, diferențele față de ESP-MESH, topologiile de comunicație și transmiterea datelor de la senzorul GY-87. Aplicațiile includ comunicații Two-Way, One-to-Many, Many-to-One și un gateway ESP-NOW + Wi-Fi Web Server.

**[Deschideți introducerea Laboratorului 6](Laborator_06_ESP_NOW/documentatie/Laborator_06.md)**.

<a id="raspberry-pi"></a>

## Partea a II-a — Sisteme cu Raspberry Pi

### Lucrări din îndrumătorul Raspberry Pi

| Laborator | Temă | Activități | Număr în îndrumătorul inițial |
| --- | --- | --- | --- |
| **L7** | [Raspberry Pi 5 și camera USB](Laborator_07_Raspberry_Pi_Camera/README.md) | Identificarea camerei, captură foto, timelapse cu cron și streaming video | 8 |
| **L8** | [PIR și camera USB](Laborator_07_Raspberry_Pi_Camera/PIR_si_camera_USB.md) | Testarea HC-SR501, fotografie la detecție și afișarea detecțiilor în browser | 9 |
| **L9** | Achiziția și înregistrarea datelor de la senzori | Măsurători cu DHT11/DHT22, BME280 și GY-87 | 11 |

### Lucrări OpenCV

| Laborator | Temă | Activități |
| --- | --- | --- |
| **L10** | Prelucrarea imaginilor cu OpenCV | Achiziție de la o cameră USB, detectarea muchiilor și identificarea obiectelor după culoare |
| **L11** | Detecție în flux video cu OpenCV | Detectarea mișcării prin comparație cu un cadru de referință, detectarea fețelor și afișarea rezultatelor în browser |

<a id="proiect"></a>

## Partea a III-a — Proiect de sistem dedicat pentru IoT

### Teme de proiect propuse

**Lista temelor este în curs de actualizare.**

| Nr. | Temă | Material de pornire |
| --- | --- | --- |
| 1 | Aplicații TinyML pe ESP32 | [Introducere în TinyML pe ESP32](https://www.teachmemicro.com/tinyml-with-esp32-tutorial/) |
| 2 | Detectarea anomaliilor cu TinyML pe Raspberry Pi și ESP32 | [Colectarea datelor](https://www.digikey.cz/en/maker/projects/edge-ai-anomaly-detection-part-1-data-collection/7bb112f76ef644edaedc5e08dba5faae) · [Antrenarea modelului](https://www.digikey.cz/en/maker/projects/edge-ai-anomaly-detection-part-2-feature-extraction-and-model-training/70927a6e439b49bea7305953a3c9bfff) · [Raspberry Pi](https://www.digikey.cz/en/maker/projects/edge-ai-anomaly-detection-part-3-machine-learning-on-raspberry-pi/af9dd958b23d4ea1b40bc3cc060ef8c9) · [ESP32](https://www.digikey.cz/en/maker/projects/edge-ai-anomaly-detection-part-4-machine-learning-on-esp32-via-arduino/afacfc3dbaf24c6c94a55c4afae1afb2) |
| 3 | Senzori, server web și MQTT cu MicroPython pe ESP32 | [Introducere](https://randomnerdtutorials.com/getting-started-micropython-esp32-esp8266/) · [DHT](https://randomnerdtutorials.com/micropython-esp32-esp8266-dht11-dht22-web-server/) · [BME280](https://randomnerdtutorials.com/micropython-bme280-esp32-esp8266/) · [MQTT](https://randomnerdtutorials.com/micropython-mqtt-publish-dht11-dht22-esp32-esp8266/) · [HC-SR04](https://randomnerdtutorials.com/micropython-hc-sr04-ultrasonic-esp32-esp8266/) |
| 4 | Detectarea obiectelor cu ESP32-CAM și Edge Impulse | [Tutorial ESP32-CAM și Edge Impulse](https://dronebotworkshop.com/esp32-object-detect/) |
| 5 | Streaming video și recunoaștere facială cu ESP32-CAM | [Exemplu pentru Arduino IDE](https://randomnerdtutorials.com/esp32-cam-video-streaming-face-recognition-arduino-ide/) |
| 6 | Detectarea obiectelor în imagini de la ESP32-CAM cu OpenCV și YOLOv3 | [Exemplu în Python](https://diyprojectslabs.com/esp32-cam-object-detection-using-opencv-python/) |
| 7 | Integrarea serviciilor OpenAI într-o aplicație Arduino pentru ESP32 | [Biblioteca OpenAI-ESP32](https://github.com/me-no-dev/OpenAI-ESP32) |
| 8 | Achiziție de imagini cu o cameră USB și OpenCV pe Raspberry Pi | [Configurarea camerei USB](https://randomnerdtutorials.com/set-up-usb-camera-opencv-raspberry-pi/) |
| 9 | Recunoașterea numerelor de înmatriculare | [Exemplu cu Raspberry Pi și Node-RED](https://randomnerdtutorials.com/car-plate-recognition-system-with-raspberry-pi-and-node-red/) |
| 10 | Clasificarea imaginilor și detectarea obiectelor pe Raspberry Pi | [Fișă de laborator (PDF)](https://irh.inf.unideb.hu/~sutoj/subjects/embedded/slides/lab/lab10.pdf) |
| 11 | Detectarea și recunoașterea textului în imagini | [Fișă de laborator (PDF)](https://irh.inf.unideb.hu/~sutoj/subjects/embedded/slides/lab/lab11.pdf) |

### Realizarea proiectului

- Cerințe de realizare și documentare a sistemului.
- Demonstrarea și prezentarea proiectului.

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
