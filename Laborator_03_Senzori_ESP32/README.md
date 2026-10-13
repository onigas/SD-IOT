# Laboratorul 3 — Senzori și interfețe web cu ESP32

**DHT11 · PIR HC-SR501 · MPU6050 pe GY-87 · LittleFS · vizualizare 3D**

## Materiale

- **[Îndrumător complet — citire directă pe GitHub](documentatie/Laborator_03.md)**: cele cinci taskuri, cuprins clicabil, conexiuni, cod complet, depanare și fișe de rezultate.
- **[PDF corectat, 24 de pagini](documentatie/Laborator_03.pdf)** — versiunea furnizată de autor, cu cele două figuri corectate.
- [Proiectele Arduino](cod/) și [cele 8 figuri extrase din PDF-ul corectat](imagini/).

## Cele cinci taskuri

| Task | Activitate | Îndrumător | Proiect |
| --- | --- | --- | --- |
| 1 | DHT11: temperatură și umiditate în terminal | [Pași și explicații](documentatie/Laborator_03.md#sectiunea-2) | [T1_DHT11](cod/T1_DHT11/) |
| 2 | DHT11: server web | [Pași și explicații](documentatie/Laborator_03.md#sectiunea-3) | [T2_DHT_Web](cod/T2_DHT_Web/) |
| 3 | PIR: întreruperi și temporizare | [Pași și explicații](documentatie/Laborator_03.md#sectiunea-4) | [T3_PIR](cod/T3_PIR/) |
| 4 | MPU6050 pe GY-87: accelerație și rotație | [Pași și explicații](documentatie/Laborator_03.md#sectiunea-5) | [T4_GY87](cod/T4_GY87/) |
| 5 | MPU6050: server web și model 3D | [Pași și explicații](documentatie/Laborator_03.md#sectiunea-6) | [T5_MPU_Web](cod/T5_MPU_Web/) |

## Pornire

1. Descărcați repository-ul prin **Code → Download ZIP**, sau clonați-l:

   ```bash
   git clone https://github.com/onigas/SD-IOT.git
   ```

2. Citiți [pregătirea laboratorului](documentatie/Laborator_03.md#sectiunea-1) și instalați bibliotecile indicate.
3. Deschideți separat fiecare fișier `.ino` din folderul său. Nu combinați sketch-urile.
4. Pentru T2 și T5 completați local `lab_config.h` cu SSID-ul și parola rețelei de 2,4 GHz. Nu publicați fișiere cu parole reale.
5. Pentru T5 încărcați **separat** sketch-ul și folderul `data/` în LittleFS. Procedura pentru IDE 2 și IDE 1.8 este în îndrumător. Biblioteca 3D este inclusă local.

| Task | Conexiuni principale | Serial Monitor |
| --- | --- | --- |
| T1 | DHT11 DATA → GPIO 4 | **9600 baud** |
| T2 | Același montaj DHT11 | **115200 baud** |
| T3 | PIR OUT → GPIO 4; LED integrat GPIO 2 | **115200 baud** |
| T4–T5 | SDA → GPIO 21; SCL → GPIO 22 | **115200 baud** |

Deconectați DHT11 înainte de a conecta PIR la GPIO 4. Alimentarea GY-87 se verifică pentru revizia concretă a modulului, conform îndrumătorului.

## Organizare

- `documentatie/Laborator_03.md` — ghid complet, editabil și afișat direct pe GitHub.
- `documentatie/Laborator_03.pdf` — PDF-ul corectat, păstrat nemodificat.
- `cod/T1_DHT11` … `cod/T5_MPU_Web` — proiecte independente.
- `cod/T5_MPU_Web/data` — HTML, CSS, JavaScript, three.js și licența sa.
- `imagini` — toate cele 8 figuri din versiunea corectată.
- `ATRIBUIRI.txt` — surse și atribuiri; bibliografia detaliată este la finalul ghidului.

Codul a fost revizuit static; nu este prezentat ca fiind compilat sau testat pe echipament în această sesiune.

[Înapoi la lista laboratoarelor](../README.md)
