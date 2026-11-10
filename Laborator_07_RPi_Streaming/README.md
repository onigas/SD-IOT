# Laboratorul 7 — Streaming video cu Raspberry Pi și mjpg-streamer

## Obiectiv

Transmiterea imaginilor de la o cameră USB compatibilă UVC prin HTTP, folosind `mjpg-streamer` pe Raspberry Pi.

> **Notă:** Această pagină documentează instalarea și verificarea `mjpg-streamer` efectuate pe sistemul `silab01`. Nu înlocuiește celelalte materiale ale laboratorului 7.

## 1. Diagnosticarea erorii

La executarea comenzii:

```bash
mjpg_streamer -i "input_uvc.so -d $CAMERA -r 640x480" -o "output_http.so -p 8080"
```

poate apărea:

```text
bash: mjpg_streamer: command not found
```

Verificați dacă executabilul se află în `PATH` sau în directoarele uzuale:

```bash
which mjpg_streamer
find ~/lab7 /usr/local/bin /usr/bin -name 'mjpg_streamer' 2>/dev/null
```

Dacă nu se găsește, instalați-l.

## 2. Instalarea din surse

```bash
sudo apt update
sudo apt install -y git cmake build-essential libjpeg-dev
cd ~/lab7
git clone https://github.com/jacksonliam/mjpg-streamer.git
cd mjpg-streamer/mjpg-streamer-experimental
make
sudo make install
```

**Verificare efectuată pe `silab01`:**

```bash
which mjpg_streamer
```

Rezultatul confirmat:

```text
/usr/local/bin/mjpg_streamer
```

Dacă `git clone` a fost efectuat anterior, nu repetați clonarea: intrați direct în directorul existent.

## 3. Verificarea camerei

Pentru o cameră USB:

```bash
ls /dev/video*
echo "$CAMERA"
```

Stabiliți care este dispozitivul de captură video (de exemplu `/dev/video0`). Dacă dispozitivele sunt multiple, inspectați-le cu `v4l2-ctl --list-devices` (pachetul `v4l-utils`).

## 4. Lansarea serverului MJPEG

Exemplu pentru webcam USB la `/dev/video0`:

```bash
mjpg_streamer -i "input_uvc.so -d /dev/video0 -r 640x480" -o "output_http.so -p 8080"
```

Alternativ, folosind variabila de mediu din comanda inițială:

```bash
export CAMERA=/dev/video0
mjpg_streamer -i "input_uvc.so -d $CAMERA -r 640x480" -o "output_http.so -p 8080"
```

Accesați fluxul dintr-un browser aflat în aceeași rețea:

```text
http://IP_RASPBERRY_PI:8080/?action=stream
```

Înlocuiți `IP_RASPBERRY_PI` cu adresa reală a dispozitivului, de exemplu cea afișată de `hostname -I`.

**Atenție la tipul camerei:** `input_uvc.so` utilizează captură V4L2, în special pentru webcam-uri USB UVC. O cameră CSI conectată la Raspberry Pi 5 și gestionată prin `libcamera`/`rpicam` poate necesita altă metodă de streaming; această comandă nu este o soluție universală pentru CSI.

## 5. Verificarea funcționării

1. Confirmați că `mjpg_streamer` pornește fără eroare și rămâne activ.
2. Deschideți adresa HTTP de mai sus pe un alt dispozitiv din rețea.
3. Verificați actualizarea imaginilor și rezoluția 640×480.

**Sursa software:** [jacksonliam/mjpg-streamer](https://github.com/jacksonliam/mjpg-streamer).
