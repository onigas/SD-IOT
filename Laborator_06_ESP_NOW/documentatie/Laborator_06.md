# Laboratorul 6 — ESP-NOW: comunicații directe între noduri ESP32

**Sisteme dedicate pentru IoT · Master, anul I**

> **Scop:** realizarea unei rețele locale de senzori distribuiți, în care mai multe plăci ESP32 schimbă date prin ESP-NOW, iar un nod gateway poate publica valorile prin Wi-Fi într-o interfață web.

## Cuprins

1. [Context și motivație](#1-context-și-motivație)
2. [Obiective de învățare](#2-obiective-de-învățare)
3. [Principiul de funcționare ESP-NOW](#3-principiul-de-funcționare-esp-now)
4. [Topologii de comunicație](#4-topologii-de-comunicație)
5. [ESP-NOW versus ESP-MESH](#5-esp-now-versus-esp-mesh)
6. [ESP-NOW împreună cu Wi-Fi: gateway și server web](#6-esp-now-împreună-cu-wi-fi-gateway-și-server-web)
7. [Datele senzorului GY-87](#7-datele-senzorului-gy-87)
8. [Fiabilitate și limitări](#8-fiabilitate-și-limitări)
9. [Structura celor patru experimente](#9-structura-celor-patru-experimente)
10. [Pregătire și întrebări introductive](#10-pregătire-și-întrebări-introductive)
11. [Referințe](#11-referințe)

## 1. Context și motivație

Într-un sistem IoT convențional, fiecare dispozitiv se conectează la un access point Wi-Fi și transmite date prin protocoale IP (de exemplu HTTP sau MQTT). Această soluție este potrivită când există infrastructură de rețea și se urmărește integrarea cu aplicații web, cloud sau servere locale.

Există însă aplicații în care *nodurile de senzori* trebuie să comunice direct între ele, fără să se asocieze la router: senzori distribuiți într-o clădire, comenzi locale pentru actuatoare, echipamente mobile sau instalații temporare.

**ESP-NOW** rezolvă această problemă prin schimbul de mesaje direct între interfețele Wi-Fi ale dispozitivelor compatibile. Routerul nu este necesar pentru comunicația ESP-NOW dintre noduri. Routerul Wi-Fi disponibil în laborator va fi utilizat pentru conectarea gateway-ului la rețeaua locală și afișarea datelor într-un browser.

**Legătura cu laboratorul anterior:** ESP-MESH urmărea interconectarea nodurilor și extinderea acoperirii prin *multi-hop*. ESP-NOW urmărește în primul rând schimbul de mesaje directe, cu configurare relativ simplă. Aceste tehnologii nu sunt sinonime.

## 2. Obiective de învățare

La finalul laboratorului, studentul trebuie să poată:

- explica diferențele dintre comunicația Wi-Fi/IP, ESP-NOW și ESP-MESH;
- identifica o placă ESP32 prin adresa MAC și configura schimbul de mesaje unicast sau broadcast;
- implementa legături one-to-one, one-to-many și many-to-one;
- transmite date achiziționate de la senzorul **GY-87** între grupe;
- înțelege rolul canalului Wi-Fi și condițiile de coexistență ESP-NOW + Wi-Fi;
- explica de ce confirmarea transmisiei nu garantează procesarea mesajului de aplicația destinatară;
- descrie arhitectura unui gateway ESP-NOW → Wi-Fi → dashboard web.

## 3. Principiul de funcționare ESP-NOW

ESP-NOW este un protocol *connectionless*, dezvoltat de Espressif, bazat pe cadre IEEE 802.11 de tip **vendor-specific action frame**. Datele aplicației sunt transportate prin radio Wi-Fi, fără asocierea prealabilă a nodurilor la un access point și fără utilizarea adreselor IP, TCP sau UDP în legătura ESP-NOW propriu-zisă.

### 3.1. Identificarea nodurilor

Destinația este identificată printr-o **adresă MAC**, nu printr-o adresă IP. În funcție de aplicație, mesajul poate fi transmis:

- **unicast** — către un dispozitiv cu o adresă MAC specificată; expeditorul înregistrează de regulă destinatarul ca *peer*;
- **broadcast** — către adresa `FF:FF:FF:FF:FF:FF`, pentru recepție de către nodurile compatibile aflate pe același canal.

Adresele MAC ale interfețelor STA și SoftAP pot fi diferite. La configurarea fiecărui nod, identificați interfața utilizată și adresa MAC corespunzătoare.

### 3.2. Canal radio comun

ESP-NOW utilizează radioul Wi-Fi de 2,4 GHz al plăcilor ESP32 clasice. **Emițătorul și receptorul trebuie să opereze pe același canal radio.** Faptul că două noduri se află în aceeași încăpere nu este suficient dacă sunt configurate pe canale diferite.

În mod obișnuit, un ESP32 conectat în regim STA la router utilizează canalul punctului de acces. Ca urmare, când ESP-NOW și conexiunea Wi-Fi funcționează simultan pe același ESP32, celelalte noduri ESP-NOW trebuie aliniate la canalul gateway-ului.

### 3.3. Mesaje și dimensiunea datelor

Aplicația trimite un bloc de octeți; acesta poate reprezenta un text, o structură C/C++ sau un mesaj serializat. Pentru interoperabilitate, emițătorul și receptorul trebuie să convină asupra câmpurilor, dimensiunilor, ordinii și unităților.

ESP-NOW v1.0 permite maximum **250 octeți** de date utile per mesaj; ESP-NOW v2.0 poate permite până la **1470 octeți**, în funcție de versiunea framework-ului și de compatibilitatea dispozitivelor. Pentru laborator vom utiliza mesaje **mai mici de 250 octeți**.

### 3.4. Un schimb tipic de date

~~~text
ESP32 A (senzor)                         ESP32 B (receptor)
  citește GY-87                              așteaptă mesaje
  construiește pachetul
  group_id, sequence, ax, ay, az, ...
              ── ESP-NOW (MAC, canal) ──►   validează lungimea
                                             identifică grupa
                                             afișează valorile
~~~

## 4. Topologii de comunicație

| Topologie | Organizare | Exemplu de utilizare |
| --- | --- | --- |
| **One-to-One** | A → B | Un senzor transmite unui receptor |
| **Two-Way** | A ↔ B | Două noduri schimbă măsurători/comenzi |
| **One-to-Many** | A → B, C, D | O comandă către mai multe noduri |
| **Many-to-One** | A, B, C → R | Colectarea centralizată a măsurătorilor |
| **Many-to-Many** | Mai multe perechi comunică direct | Schimb de date între grupele laboratorului |

**Distincție importantă:** existența mai multor conexiuni sau chiar a unei configurații many-to-many **nu transformă automat rețeaua într-un mesh**. ESP-NOW nu descoperă și nu rutează implicit pachete pe trasee multi-hop.

## 5. ESP-NOW versus ESP-MESH

Într-un laborator anterior am discutat **ESP-MESH**, protocolul Espressif pentru rețele Wi-Fi organizate pe nivele, cu relații părinte–copil, nod *root* și posibilitatea de a transmite prin noduri intermediare.

### 5.1. Diferența ilustrată

~~~text
ESP-NOW — comunicare directă

  Nod A  ───────────────────►  Nod B

Dacă B este în afara razei radio directe a lui A,
ESP-NOW nu stabilește automat o rută prin C.


ESP-MESH — comunicație multi-hop

  Nod A  ─────►  Nod C  ─────►  Nod B
                 (intermediar)

Într-o rețea mesh configurată, datele pot traversa
mai multe legături; rețeaua poate reorganiza traseele.
~~~

### 5.2. Comparație

| Caracteristică | ESP-NOW | ESP-MESH |
| --- | --- | --- |
| Concept de bază | Schimb direct de mesaje între dispozitive | Rețea Wi-Fi structurată, cu noduri intermediare |
| Adresare pentru mesajele dintre noduri | MAC | Identitatea nodurilor și mecanisme de rutare mesh |
| Asociere la un AP pentru fiecare senzor | Nu este necesară | Nodurile stabilesc legături Wi-Fi cu părinții lor |
| Transmitere multi-hop | **Nu automat**; cere logică suplimentară în aplicație | **Da**, face parte din arhitectura rețelei |
| Reorganizare după căderea unui nod intermediar | Trebuie implementată la nivelul aplicației, dacă este necesară | Există mecanisme de *self-healing* |
| Complexitate | Relativ redusă pentru mesaje directe | Mai ridicată, pentru organizarea și întreținerea rețelei |
| Aplicație tipică | Senzori/comenzi locale, telemetrie, gateway | Acoperire extinsă, rețele cu noduri la distanță |

**Nu confundați:** ESP-MESH (Wi-Fi Mesh din ESP-IDF) cu ESP-Mesh-Lite sau cu soluții mesh dezvoltate *peste* ESP-NOW. Acestea au implementări și capabilități diferite.

**Întrebare pentru discuție:** Dacă A nu poate comunica direct cu B, dar ambele pot comunica cu C, ce logică ar trebui adăugată în aplicația ESP-NOW pentru ca C să devină un releu? Cum s-ar evita buclele și duplicarea mesajelor?

## 6. ESP-NOW împreună cu Wi-Fi: gateway și server web

Un ESP32 poate îndeplini simultan rolurile de **receptor ESP-NOW** și **client Wi-Fi al routerului**, cu condiția configurării corecte a canalului și a interfețelor. Gateway-ul va primi măsurătorile de la grupe și le va publica unui browser din rețeaua locală.

~~~mermaid
flowchart LR
    A["ESP32 Grupa 1<br/>GY-87"] -->|"ESP-NOW"| G["ESP32 Gateway"]
    B["ESP32 Grupa 2<br/>GY-87"] -->|"ESP-NOW"| G
    C["ESP32 Grupa 3<br/>GY-87"] -->|"ESP-NOW"| G
    G -->|"Wi-Fi / IP"| R["Router 2,4 GHz"]
    R -->|"HTTP + SSE"| L["Laptop<br/>Dashboard web"]
~~~

Prin urmare, **routerul nu transportă pachetele ESP-NOW dintre senzori și gateway**. El este folosit pentru accesul la serverul web găzduit de gateway. Pagina poate afișa valori de la mai multe grupe, actualizate prin *Server-Sent Events (SSE)*.

În tutorialul de referință, gateway-ul folosește modul `WIFI_AP_STA`; canalul său Wi-Fi este cel al routerului. Nodurile emițătoare trebuie să transmită pe **același canal**. Un eventual schimb automat de canal trebuie tratat explicit; simpla configurare ESP-NOW nu îl rezolvă.

**Exemplu:** dacă routerul operează pe canalul 6 în banda de 2,4 GHz, toate plăcile care comunică prin ESP-NOW cu gateway-ul trebuie să folosească acest canal. În laborator nu vom presupune numărul canalului: îl vom **citi efectiv** la gateway.

## 7. Datele senzorului GY-87

În exemplele practice vom utiliza **GY-87** în locul senzorilor BME280/DHT22 utilizați în unele tutoriale de referință. GY-87 este un modul multisenzor cu interfață **I²C**, care include în mod obișnuit:

- **MPU6050** — accelerometru și giroscop, câte trei axe;
- **BMP180** — presiune atmosferică și temperatură;
- un **magnetometru** — tipul concret depinde de revizia modulului.

GY-87 **nu furnizează umiditate**; structurile de date din tutoriale trebuie adaptate. Pentru început sunt suficiente accelerațiile `ax`, `ay`, `az`, eventual vitezele unghiulare și presiunea.

Pentru placa ESP32 clasică, conexiunile I²C utilizate frecvent sunt **SDA = GPIO 21**, **SCL = GPIO 22**. Verificați alimentarea, nivelurile logice și revizia modulului înainte de conectare. Puteți reutiliza codul de achiziție GY-87 din [Laboratorul 3](../../Laborator_03_Senzori_ESP32/README.md).

### Formatul comun al mesajului

Toate grupele vor utiliza același format de date, astfel încât un receptor să poată identifica emițătorul și să afișeze măsurătorile. Conceptual, un mesaj conține:

| Câmp | Rol |
| --- | --- |
| `group_id` | Identificatorul grupei |
| `sequence` | Număr secvențial, pentru identificarea mesajelor pierdute sau duplicate |
| `timestamp_ms` | Timp local al nodului emițător; **nu** reprezintă un ceas sincronizat între noduri |
| `ax, ay, az` | Accelerație pe trei axe, într-o unitate stabilită |
| `gx, gy, gz` | Viteze unghiulare, într-o unitate stabilită |
| `pressure` | Presiune atmosferică, într-o unitate stabilită |

În implementare, definiți explicit structura C/C++, verificați lungimea pachetului recepționat și inițializați corect senzorii. Pentru măsurarea latenței între două dispozitive, valorile `millis()` nu sunt direct comparabile fără sincronizarea ceasurilor.

## 8. Fiabilitate și limitări

ESP-NOW simplifică transferul de mesaje, dar **nu oferă, în mod implicit, toate serviciile unei rețele IP sau ale unui protocol de aplicație fiabil**.

- **Canal radio comun:** nepotrivirea canalelor poate împiedica recepția.
- **Coliziuni și trafic simultan:** mai multe noduri care emit frecvent pot conduce la pierderi sau variații de întârziere.
- **Confirmări:** un rezultat pozitiv al callback-ului de transmisie indică rezultatul la nivel MAC, nu dovedește că aplicația destinatară a prelucrat mesajul. Pentru aceasta se poate implementa un **ACK de aplicație**.
- **Numerotare:** `sequence` permite depistarea golurilor și a mesajelor duplicate.
- **Securitate:** ESP-NOW permite criptarea traficului unicast pe bază de chei configurate; **broadcast-ul nu este criptat prin mecanismul standard de criptare ESP-NOW**. Evitați datele sensibile în mesaje neprotejate.
- **Raza de acțiune:** depinde de antenă, obstacole, orientare și interferențe; nu presupuneți o distanță universal garantată.

Un indicator util al recepției este *Packet Delivery Ratio* (PDR):

`PDR = 100 × N_pachete_primite / N_pachete_transmise [%]`

Pentru un rezultat corect, valorile trebuie măsurate pe aceeași fereastră temporală, cu identificarea emițătorului și tratarea duplicatelor.

## 9. Structura celor patru experimente

| Experiment | Temă | Obiectiv principal |
| --- | --- | --- |
| **T1** | ESP-NOW One-to-One / Two-Way | Identificare MAC, inițializare ESP-NOW, schimb bidirecțional de mesaje și date GY-87 |
| **T2** | ESP-NOW One-to-Many | Un emițător transmite către mai mulți destinatari |
| **T3** | ESP-NOW Many-to-One între grupe | Un receptor colectează simultan date de la mai multe grupe, identificând nodurile |
| **T4** | ESP-NOW + Wi-Fi Web Server | Un gateway afișează în browser date GY-87 de la mai multe grupe, prin routerul laboratorului |

**Rezultatul urmărit:** studenții să obțină o rețea funcțională de senzori, să poată distinge comunicațiile directe de cele rutate și să explice limitările și avantajele unui gateway IoT.

## 10. Pregătire și întrebări introductive

**Echipamente:** plăci ESP32 cu radio Wi-Fi de 2,4 GHz, module GY-87, cabluri USB de date, calculatoare cu Arduino IDE 2.x; pentru T4, router Wi-Fi de 2,4 GHz și un laptop cu browser.

**Mediu software:** platforma `esp32 by Espressif Systems` și bibliotecile pentru senzor. Exemplele online pot utiliza semnături vechi ale callback-urilor ESP-NOW; pentru **Arduino ESP32 Core 3.x** trebuie verificate și adaptate declarațiile de recepție și transmisie. Nu utilizați conversii forțate de tip pentru a ascunde incompatibilitățile. Notați versiunea Arduino ESP32 Core și a bibliotecilor utilizate la compilare.

**Întrebări pentru discuție:**

1. De ce ESP-NOW poate funcționa fără router, deși utilizează același circuit radio Wi-Fi?
2. Ce înseamnă o adresă MAC în acest context și de ce nu avem nevoie de IP pentru un mesaj ESP-NOW?
3. De ce o topologie ESP-NOW many-to-many nu este, automat, ESP-MESH?
4. În ce situație ar fi mai potrivit ESP-MESH decât ESP-NOW?
5. De ce trebuie să ținem cont de canalul routerului la gateway-ul ESP-NOW + Wi-Fi?
6. Cum putem verifica dacă o măsurătoare a ajuns efectiv în aplicația de pe receptor?

## 11. Referințe

### Documentație oficială

- [Espressif — ESP-NOW (ESP-IDF Programming Guide)](https://docs.espressif.com/projects/esp-idf/en/v5.5/esp32/api-reference/network/esp_now.html)
- [Espressif — ESP-MESH (Programming Guide)](https://docs.espressif.com/projects/esp-idf/en/release-v3.2/api-guides/mesh.html)
- [Espressif — ESP-NOW FAQ](https://github.com/espressif/esp-faq/blob/master/docs/en/application-solution/esp-now.rst)
- [Espressif Developer Portal — Using ESP-NOW in Arduino](https://developer.espressif.com/blog/2024/08/arduino-esp-now-lib/)

### Tutoriale de lucru

- [Random Nerd Tutorials — Getting Started with ESP-NOW](https://randomnerdtutorials.com/esp-now-esp32-arduino-ide/)
- [Random Nerd Tutorials — ESP-NOW Two-Way](https://randomnerdtutorials.com/esp-now-two-way-communication-esp32/)
- [Random Nerd Tutorials — ESP-NOW One-to-Many](https://randomnerdtutorials.com/esp-now-one-to-many-esp32-esp8266/)
- [Random Nerd Tutorials — ESP-NOW Many-to-One](https://randomnerdtutorials.com/esp-now-many-to-one-esp32/)
- [Random Nerd Tutorials — ESP-NOW + Wi-Fi Web Server](https://randomnerdtutorials.com/esp32-esp-now-wi-fi-web-server/)

