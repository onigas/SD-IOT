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

### Deschiderea proiectelor Arduino

Deschideți fiecare proiect în Arduino IDE din directorul său.

- **Laboratorul 2** — copiați `secrets.example.h` ca `secrets.h` și completați configurația locală.
- **Laboratorul 3** — pentru T2 și T5, completați `lab_config.h` cu datele rețelei. Pentru T5, încărcați și conținutul directorului `data/` în LittleFS.

Păstrați parolele Wi-Fi, tokenurile și cheile API în configurația locală. Nu le publicați pe GitHub.

<a id="raspberry-pi"></a>

## Partea a II-a — Sisteme cu Raspberry Pi

| Laborator | Temă | Activități |
| --- | --- | --- |
| **L7** | [Raspberry Pi 5 și camera USB](Laborator_07_Raspberry_Pi_Camera/README.md) | Identificarea camerei, captură foto, timelapse cu cron și streaming video |
| **L8** | [PIR și camera USB](Laborator_08_PIR_Camera_USB/README.md) | Testarea HC-SR501, fotografie la detecție și afișarea detecțiilor în browser |
| **L9** | Achiziția și înregistrarea datelor de la senzori | Măsurători cu DHT11/DHT22, BME280 și GY-87 |
| **L10** | Prelucrarea imaginilor cu OpenCV | Achiziție de la o cameră USB, detectarea muchiilor și identificarea obiectelor după culoare |
| **L11** | Detecție în flux video cu OpenCV | Detectarea mișcării prin comparație cu un cadru de referință, detectarea fețelor și afișarea rezultatelor în browser |

<a id="proiect"></a>

## Partea a III-a — Proiect de sistem dedicat pentru IoT

### Teme de proiect propuse

**Lista temelor este în curs de actualizare.**

#### A. TinyML și inteligență artificială pentru IoT

| Nr. | Tema de proiect | Echipament principal | Demonstrație finală |
| --- | --- | --- | --- |
| 1 | **Recunoașterea gesturilor cu ESP32 și GY-87** | ESP32 + GY-87 | Echipa înregistrează trei gesturi și exemple de mișcare obișnuită, apoi antrenează un model care rulează pe ESP32. Fiecare gest transmite o comandă distinctă prin MQTT. Un coleg care nu a participat la antrenare execută gesturile într-o ordine aleasă de el. Se verifică atât comenzile recunoscute, cât și declanșările accidentale dintre gesturi. |
| 2 | Detectarea anomaliilor de vibrație cu TinyML | ESP32 + GY-87 | Echipa înregistrează vibrațiile unui dispozitiv în două regimuri de funcționare. Pe o înregistrare nouă, compară alertele unui model rulat pe ESP32 cu cele obținute printr-un prag simplu. |
| 3 | Clasificarea obiectelor cu Teachable Machine și publicarea rezultatului prin MQTT | Cameră USB + Raspberry Pi sau PC | Echipa antrenează un clasificator pe imagini proprii pentru două categorii de obiecte și clasa „cadru gol”. Testează cu alte exemplare și în altă lumină. Un obiect ținut în cadru generează un singur eveniment MQTT. Dacă este scos și reintrodus, generează al doilea eveniment. [Teachable Machine](https://teachablemachine.withgoogle.com/) |
| 4 | Recunoaștere facială locală cu Raspberry Pi | Raspberry Pi + cameră USB | Sistemul recunoaște voluntarii înscriși și afișează „necunoscut” pentru o persoană neînscrisă. Echipa testează cu imagini diferite de cele de referință și arată cum influențează iluminarea și pragul de potrivire rezultatul. Imaginile voluntarilor se păstrează local pe durata demonstrației. |
| 5 | Asistent AI pentru interogarea datelor unei stații IoT | ESP32 + senzor de mediu + Raspberry Pi sau PC | Stația înregistrează măsurători, iar utilizatorul întreabă, de exemplu, „Când a depășit ultima dată umiditatea 70% și pentru cât timp?”. Aplicația determină intervalul din jurnal și formulează răspunsul printr-un serviciu AI. Echipa verifică rezultatul în datele originale și testează o perioadă pentru care lipsesc măsurători. |

#### B. Sisteme cu senzori și comunicații IoT

| Nr. | Tema de proiect | Echipament principal | Demonstrație finală |
| --- | --- | --- | --- |
| 6 | Monitorizarea temperaturii și umidității cu alerte la depășiri persistente | ESP32 + DHT11/DHT22 sau BME280 | Stația publică măsurătorile prin MQTT și înregistrează începutul, durata și sfârșitul unei abateri. Echipa provoacă o depășire scurtă și una persistentă pentru a verifica regula de alertare. MicroPython este o opțiune de implementare. |
| 7 | Rețea de senzori pentru mai multe încăperi, cu gateway ESP-NOW–MQTT | Două noduri ESP32 cu senzori + un ESP32 gateway | Gateway-ul colectează măsurătorile și le publică prin MQTT. Echipa deconectează pe rând câte un nod. Interfața identifică sursa care a încetat să transmită și continuă afișarea datelor de la celălalt nod. |
| 8 | Înregistrator de date cu recuperarea măsurătorilor după întreruperea Wi-Fi | ESP32 + un senzor | Echipa întrerupe rețeaua în timpul achiziției. Placa salvează local măsurătorile și le retransmite la reconectare. Numerele de secvență permit verificarea eventualelor goluri sau duplicate. |
| 9 | Monitorizarea manipulării unui colet | ESP32 + GY-87 | Dispozitivul jurnalizează răsturnările, înclinările prelungite și șocurile. O altă echipă manipulează coletul, iar autorii reconstruiesc ordinea manevrelor din jurnal și o compară cu ceea ce s-a întâmplat efectiv. |

#### C. Detectarea mișcării și a persoanelor. Aplicații cu camere video

| Nr. | Tema de proiect | Echipament principal | Demonstrație finală |
| --- | --- | --- | --- |
| 10 | Detectarea mișcării cu PIR și confirmarea vizuală a prezenței unei persoane | Raspberry Pi + HC-SR501 + cameră USB | Sistemul păstrează o scurtă secvență video care începe înaintea declanșării și continuă după aceasta. Indică dacă în imagini apare o persoană. Echipa testează o trecere prin zona comună senzorului și camerei, apoi o trecere detectată de PIR în afara câmpului camerei. |
| 11 | Numărarea trecerilor pe sens cu doi senzori PIR | ESP32 sau Raspberry Pi + doi senzori PIR | Sistemul deduce sensul din ordinea activărilor. Echipa stabilește poziția senzorilor și testează o traversare completă, o întoarcere la jumătatea traseului și două treceri apropiate. |
| 12 | Numărarea persoanelor pe sens de deplasare cu o cameră video | Raspberry Pi + cameră USB | Sistemul urmărește persoanele între cadre și numără traversările unei linii virtuale. Echipa compară rezultatul cu numărătoarea manuală, inclusiv când o persoană se oprește lângă linie sau se întoarce înainte de a o traversa. |
| 13 | Transmisie video cu estomparea automată a fețelor | Raspberry Pi + cameră USB | Fluxul procesat se vede în browserul unui al doilea dispozitiv. Echipa verifică estomparea cu două persoane în cadru și observă ce se întâmplă când una își întoarce capul și detecția devine intermitentă. |
| 14 | Citirea automată a unui afișaj numeric prin OCR | Raspberry Pi + cameră USB + aparat cu afișaj | Sistemul citește periodic un afișaj real și salvează valoarea împreună cu ora. Echipa compară rezultatele cu citiri manuale și demonstrează cum marchează o imagine neclară sau o valoare imposibilă. |
| 15 | Acționarea unei bariere de acces prin recunoașterea numerelor de înmatriculare | Raspberry Pi + cameră USB + ieșire de comandă pentru controlerul barierei | Sistemul citește numărul de înmatriculare, îl compară cu lista de acces și emite o singură comandă de deschidere pentru un vehicul autorizat. Echipa testează un număr autorizat, unul neautorizat și unul ilizibil. La postul de laborator, comanda poate fi verificată printr-un LED sau la ieșirea unui releu. |
| 16 | Citirea asistată a indexului unui contor, cu redare vocală pe telefon pentru persoane cu deficiențe de vedere | Raspberry Pi + cameră USB orientată spre un contor + telefon | Utilizatorul pornește citirea de pe telefon printr-un buton etichetat pentru cititorul de ecran. Camera fotografiază afișajul, aplicația extrage indexul prin OCR, iar telefonul îl citește cu voce tare și permite repetarea lui. Echipa testează utilizarea fără consultarea vizuală a ecranului, inclusiv o captură neclară pentru care telefonul cere o nouă citire. |

### Realizarea proiectului

- Cerințe de realizare și documentare a sistemului.
- Demonstrarea și prezentarea proiectului.
