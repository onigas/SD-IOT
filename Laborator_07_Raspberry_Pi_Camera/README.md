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

**Programați captura.** Rulați `crontab -e`. Adăugați la sfârșit linia următoare și salvați:

```cron
* * * * * "$HOME/lab7/capture.sh" >> "$HOME/lab7/capture.log" 2>&1
```

Cele cinci asteriscuri programează execuția în fiecare minut. Lăsați camera fixă și deplasați obiectul între capturi. Așteptați cel puțin trei fotografii automate, apoi parcurgeți-le în ordinea numelor. Verificați poziția obiectului și intervalul dintre marcajele de timp. Dacă fotografiile lipsesc, verificați programarea cu `crontab -l`, starea serviciului cu `systemctl is-active cron` și mesajele din `~/lab7/capture.log`.

**Eliberați camera înainte de streaming.** Redeschideți `crontab -e`, ștergeți doar linia pe care ați adăugat-o și salvați. Verificați cu `crontab -l` și lăsați orice captură în curs să se încheie.

## 4. Streaming video de pe placă spre PC

**Pe Raspberry Pi**, consultați lista formatelor de la secțiunea 2. Pentru o cameră care oferă `MJPG` la rezoluția aleasă, porniți serverul astfel, din același terminal în care ați definit `CAMERA`:

```bash
mjpg_streamer -i "input_uvc.so -d $CAMERA -r 640x480" -o "output_http.so -p 8080"
```

Dacă la rezoluția aleasă camera oferă doar `YUYV`, adăugați `-y` în opțiunile de intrare, imediat după `input_uvc.so`. Lăsați serverul pornit. Modulul de intrare citește cadrele camerei, iar modulul HTTP le transmite pe portul 8080.

**Pe PC**, deschideți un browser și introduceți `http://ADRESA_IP:8080/?action=stream`. Înlocuiți `ADRESA_IP` cu adresa locală a plăcii. Mișcați obiectul și verificați dacă imaginea din browser urmărește mișcarea. La final apăsați **Ctrl+C** în terminalul în care rulează serverul.

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

[Continuați cu partea a doua — Senzorul PIR și captura foto cu camera USB](PIR_si_camera_USB.md)
