# Laboratorul 7 — Raspberry Pi 5 și camera USB

[Înapoi la Partea a II-a — Sisteme cu Raspberry Pi](../README.md#raspberry-pi)

La sfârșitul lucrării veți putea identifica o cameră USB în Linux, realiza o fotografie, programa o captură pe minut și urmări imaginea în direct de pe alt calculator.

**Echipament.** Raspberry Pi 5 cu alimentator USB-C, cameră web USB și PC conectat la rețeaua laboratorului. Placa are sistemul, Wi-Fi și accesul la distanță configurate. Folosiți datele de acces ale postului vostru.

## 1. Conectarea la Raspberry Pi

1. Conectați camera la un port USB al plăcii. Alimentați placa prin USB-C și așteptați conectarea la Wi-Fi.
2. Pe PC, deschideți clientul VNC al laboratorului. Folosiți conexiunea salvată pentru postul vostru sau introduceți adresa IP locală a plăcii. Dacă aveți nevoie de adresă, selectați placa în Raspberry Pi Connect, deschideți **Screen sharing** și țineți cursorul deasupra pictogramei Wi-Fi de pe desktopul plăcii. Notați adresa IPv4 și pentru proba de streaming. [Raspberry Pi Connect](https://connect.raspberrypi.com/sign-in)
3. Autentificați-vă pe placă. Dacă lucrați deja prin **Screen sharing**, puteți continua în același desktop. Deschideți un terminal **pe Raspberry Pi** și pregătiți directorul de lucru.

```bash
hostname
mkdir -p ~/lab7
cd ~/lab7
```

Verificați că numele afișat de `hostname` corespunde plăcii voastre. Introduceți comenzile următoare în acest terminal. La ultima activitate veți folosi și browserul de pe PC.

## 2. Identificarea camerei și prima fotografie

**Identificați camera.** Prima comandă afișează dispozitivele USB. A doua grupează dispozitivele video după numele camerei.

```bash
lsusb
v4l2-ctl --list-devices
```

Sub numele camerei pot apărea mai multe intrări. Verificați care dintre ele oferă formate de captură. În exemplu folosim `/dev/video0`. Dacă pe placa voastră intrarea corectă este alta, adaptați prima linie.

```bash
CAMERA=/dev/video0
v4l2-ctl -d "$CAMERA" --list-formats-ext
```

Notați o combinație disponibilă de format și rezoluție. Comenzile de mai jos folosesc `640x480`. Dacă această rezoluție lipsește, folosiți una din listă.

**Verificați dacă `fswebcam` este instalat.** În terminalul de pe Raspberry Pi, rulați `command -v fswebcam`. Dacă nu se afișează nicio cale sau dacă la captură apare mesajul `fswebcam: command not found`, instalați utilitarul cu următoarele comenzi:

```bash
sudo apt update
sudo apt install fswebcam
```

**Faceți o fotografie.** Îndreptați camera spre un obiect și rulați:

```bash
fswebcam -d "$CAMERA" -r 640x480 test.jpg
```

Deschideți `test.jpg` din folderul personal `lab7`. Verificați dacă se văd obiectul și detaliile sale.

**Comparați două capturi.** Păstrați obiectul și iluminarea fixe. Dacă lista camerei include `YUYV` la rezoluția aleasă, rulați comenzile de mai jos. Pentru alte camere, eliminați `-p YUYV` din ambele comenzi.

```bash
fswebcam -d "$CAMERA" -r 640x480 -p YUYV simpla.jpg
fswebcam -d "$CAMERA" -r 640x480 -p YUYV -D 2 -S 20 -F 2 reglata.jpg
```

A doua comandă așteaptă două secunde, ignoră primele 20 de cadre și combină două cadre pentru imaginea finală. Deschideți ambele fotografii și notați dacă observați vreo diferență de luminozitate sau zgomot.

## 3. Timelapse — o fotografie pe minut

**Pregătiți scriptul.** În terminal, deschideți `nano ~/lab7/capture.sh` și introduceți codul de mai jos. În comanda de captură folosiți dispozitivul și rezoluția cu care ați obținut fotografia la secțiunea 2. Scriptul va rula independent de terminal.

```bash
#!/bin/bash
set -e
folder="$HOME/lab7/webcampics"
mkdir -p "$folder"
data=$(date +'%Y-%m-%d_%H-%M-%S')
fswebcam -d /dev/video0 -r 640x480 "$folder/$data.jpg"
```

Salvați cu **Ctrl+O**, confirmați cu **Enter**, apoi ieșiți cu **Ctrl+X**. Dați scriptului permisiune de execuție și testați-l manual:

```bash
chmod +x ~/lab7/capture.sh
~/lab7/capture.sh
```

Verificați în `~/lab7/webcampics` că imaginea are în nume data și ora capturii.

**Programați o fotografie pe minut.**

1. **În terminalul de pe Raspberry Pi**, tastați comanda de mai jos și apăsați **Enter**. Aceasta deschide lista de sarcini programate pentru utilizatorul vostru.

   ```bash
   crontab -e
   ```

2. Dacă apare o listă de editoare, tastați numărul afișat lângă **nano** și apăsați **Enter**.
3. **În editorul care s-a deschis**, mergeți cu săgeata în jos până la sfârșitul fișierului. Pe o linie nouă, copiați integral textul următor, inclusiv cele cinci asteriscuri:

   ```cron
   * * * * * "$HOME/lab7/capture.sh" >> "$HOME/lab7/capture.log" 2>&1
   ```

   Cele cinci asteriscuri programează pornirea scriptului la fiecare minut. Scriptul salvează fotografiile în `~/lab7/webcampics/`, iar mesajele și eventualele erori se adaugă în `~/lab7/capture.log`.
4. **În nano**, apăsați **Ctrl+O** (litera O) pentru salvare, **Enter** pentru confirmarea numelui fișierului și **Ctrl+X** pentru ieșire.
5. **Înapoi în terminalul de pe Raspberry Pi**, tastați `crontab -l` (litera **l**, de la *list*) și apăsați **Enter**. Comanda afișează sarcinile programate pentru utilizatorul vostru. Dacă vedeți linia cu cele cinci asteriscuri și `capture.sh`, programarea este salvată.

**Ce se întâmplă după salvare?** După ce ieșiți din editor, `cron` pornește automat scriptul la începutul fiecărui minut. De exemplu, dacă reveniți în terminal la 10:34:25, primele două porniri sunt programate la 10:35 și 10:36. Fotografia apare după terminarea capturii. Mesajele sunt scrise în `~/lab7/capture.log`, de aceea terminalul poate rămâne fără niciun mesaj nou.

**Câte fotografii se fac?** Fiecare rulare reușită salvează o fotografie. Captura continuă o dată pe minut, inclusiv după închiderea terminalului, cât timp placa este pornită, serviciul `cron` este activ și programarea există. Pentru această probă, urmăriți **trei fotografii automate noi**. Dacă ați pornit cu o singură fotografie de la testul manual, după cele trei capturi veți avea patru fotografii în folder.

**Verificați rezultatul.** Lăsați camera fixă și deplasați obiectul între capturi. După puțin peste un minut, rulați în terminalul de pe Raspberry Pi:

```bash
ls -lt ~/lab7/webcampics
```

Fotografia cea mai recentă apare prima. Verificați ora din numele ei și repetați comanda după următoarea captură. Deschideți cele trei fotografii noi din folderul `lab7/webcampics` și comparați poziția obiectului. Dacă programarea apare în `crontab -l`, dar fotografiile lipsesc, rulați `tail ~/lab7/capture.log` pentru ultimele mesaje ale scriptului și `systemctl is-active cron` pentru starea serviciului.

**Opriți captura periodică înainte de streaming.** În terminalul de pe Raspberry Pi, tastați din nou `crontab -e` și apăsați **Enter**. În editor, așezați cursorul pe linia adăugată la pasul 3. În nano, **Ctrl+K** șterge linia curentă. Salvați cu **Ctrl+O**, confirmați cu **Enter** și ieșiți cu **Ctrl+X**. Înapoi în terminal, verificați cu `crontab -l` că programarea a fost eliminată. Dacă o captură este deja în curs, așteptați să se încheie.

## 4. Streaming video de pe placă spre PC

**4.1. Verificarea instalării mjpg-streamer.** În terminalul Raspberry Pi executați:

```bash
which mjpg_streamer
```

Dacă nu se afișează nimic sau comanda de streaming produce `bash: mjpg_streamer: command not found`, instalați programul din surse:

```bash
sudo apt update
sudo apt install -y git cmake build-essential libjpeg-dev
cd ~/lab7
git clone https://github.com/jacksonliam/mjpg-streamer.git
cd mjpg-streamer/mjpg-streamer-experimental
make
sudo make install
```

Dacă directorul `mjpg-streamer` există deja, omiteți clonarea și intrați în `~/lab7/mjpg-streamer/mjpg-streamer-experimental`.

Verificați din nou:

```bash
which mjpg_streamer
```

Pe placa `silab01`, instalarea a fost confirmată prin rezultatul `/usr/local/bin/mjpg_streamer`.

**4.2. Pornirea streamingului.** Consultați formatele disponibile de la secțiunea 2. Dacă ați deschis un terminal nou, reinițializați variabila `CAMERA` cu dispozitivul verificat:

```bash
CAMERA=/dev/video0
mjpg_streamer -i "input_uvc.so -d $CAMERA -r 640x480" -o "output_http.so -p 8080"
```

Înlocuiți `/dev/video0` dacă dispozitivul camerei are alt nume. Pentru o cameră care oferă `MJPG` la 640×480, comanda este potrivită. Dacă oferă doar `YUYV`, adăugați `-y` în opțiunile `input_uvc.so`. Lăsați terminalul cu serverul pornit.

**4.3. Vizualizarea imaginii în browser.** Deschideți alt terminal pe Raspberry Pi și identificați adresa sa IP:

```bash
hostname -I
```

Pe PC-ul din aceeași rețea, deschideți Chrome sau Edge și accesați:

```text
http://ADRESA_IP:8080/?action=stream
```

Înlocuiți `ADRESA_IP` cu IP-ul real al plăcii. Mișcați obiectul din fața camerei și verificați imaginea în timp real. La final opriți streamingul cu **Ctrl+C** în terminalul serverului.

**Notă:** Acest experiment utilizează o cameră web USB UVC și modulul `input_uvc.so`. Camerele CSI pentru Raspberry Pi 5 pot necesita o metodă diferită bazată pe `rpicam`/`libcamera`.

## Rezultate de prezentat

- Fotografia inițială și cele două capturi de comparație, cu o observație despre rezultat.
- Trei fotografii automate consecutive ale aceleiași scene, cu poziții diferite ale obiectului și orele capturilor vizibile în numele fișierelor.
- Demonstrarea fluxului live în browserul PC-ului.

## Materiale de referință

- [Introducere în Raspberry Pi — Random Nerd Tutorials](https://randomnerdtutorials.com/getting-started-with-raspberry-pi/)
- [Acces la distanță — documentația Raspberry Pi](https://www.raspberrypi.com/documentation/computers/remote-access.html)
- [Captură foto cu fswebcam](https://manpages.debian.org/trixie/fswebcam/fswebcam.1.en.html) și [identificarea dispozitivelor cu v4l2-ctl](https://manpages.debian.org/trixie/v4l-utils/v4l2-ctl.1.en.html)
- [Programarea sarcinilor cu cron](https://manpages.debian.org/trixie/cron/crontab.5.en.html)
- [mjpg-streamer — accesarea fluxului HTTP](https://github.com/jacksonliam/mjpg-streamer/blob/master/mjpg-streamer-experimental/plugins/output_http/README.md)
