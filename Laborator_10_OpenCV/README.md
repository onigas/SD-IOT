# Laboratorul 10 — Prelucrarea imaginilor cu OpenCV pe Raspberry Pi 5

[Înapoi la cuprinsul disciplinei](../README.md#raspberry-pi)

**Durată:** 3–4 ore. **Echipament:** Raspberry Pi 5, cameră USB, PC cu Raspberry Pi Connect sau VNC, trei obiecte distincte roșu/verde/albastru.  
**Material de bază:** programele Python furnizate în arhiva `RPi_OpenCV.zip`. Codurile din [`cod/`](cod/) păstrează implementările inițiale; exercițiile propun modificări realizate de studenți, fără înlocuirea codului original.

## Obiective

Studenții vor putea să acceseze un dispozitiv video USB prin OpenCV; să captureze imagini și secvențe AVI; să compare operatorii Canny, Sobel și Laplacian; să localizeze obiecte după culoare în spațiul HSV; să evalueze separat latența procesării și timpul total de achiziție + procesare.

## Pregătirea laboratorului

Lucrăm în **desktopul Raspberry Pi**, accesat prin **Raspberry Pi Connect** sau **VNC**. Porniți un terminal în acea sesiune (ferestrele `cv2.imshow()` apar pe desktopul Raspberry Pi).

Conectați webcam-ul USB. Folosiți camera testată deja la L7:

```bash
v4l2-ctl --list-devices
v4l2-ctl -d /dev/video0 --list-formats-ext
```

Dacă dispozitivul camerei este altul decât `/dev/video0`, adaptați `VideoCapture(0)` din programele furnizate. Închideți aplicațiile care dețin camera (inclusiv `mjpg_streamer` din L7).

**Biblioteci:** verificați înainte de instalare:

```bash
python3 -c "import cv2, numpy; print('OpenCV', cv2.__version__)"
```

Dacă importurile nu reușesc, pe Raspberry Pi OS folosiți pachetele distribuției pentru executarea scripturilor cu interfață grafică:

```bash
sudo apt update
sudo apt install -y python3-opencv python3-numpy python3-pil python3-pandas python3-matplotlib python3-ipykernel jupyter-notebook
```

În cazul în care `jupyter-notebook` nu există în versiunea curentă a depozitului, configurați Jupyter separat în mediul Python utilizat în laborator. Nu amestecați un `cv2` instalat prin apt cu unul dintr-un mediu virtual fără a verifica interpretul folosit.

Pregătiți directorul de lucru descărcând repository-ul, dacă nu există deja local:

```bash
cd ~
git clone https://github.com/onigas/SD-IOT.git
cd SD-IOT/Laborator_10_OpenCV
```

Dacă repository-ul există deja, în directorul lui executați `git pull` și apoi intrați în `Laborator_10_OpenCV`.

## Experimentul 1 — Achiziție video de la camera USB (15–20 min)

**Fișier:** [`opencv_test_usb_camera.py`](cod/opencv_test_usb_camera.py)

```bash
python3 cod/opencv_test_usb_camera.py
```

Programul folosește `cv2.VideoCapture(0)`, citește succesiv cadre cu `read()` și le afișează cu `imshow()`. Opriți prin tasta **q**, apăsată în fereastra video.

**Cerințe:** identificați dimensiunile reale ale imaginii, probați două rezoluții acceptate de cameră și observați dacă modificarea rezoluției schimbă fluiditatea.

**De explicat:** diferența dintre camera USB, fluxul de cadre, obiectul `VideoCapture` și o imagine individuală `numpy.ndarray`.

## Experimentul 2 — Captură foto și înregistrare video (25–30 min)

**Fișier:** [`capture_save.py`](cod/capture_save.py)

```bash
python3 cod/capture_save.py
```

Comenzi interactive din fereastra de captură:

| Tastă | Funcție |
| --- | --- |
| **s** | Salvează fotografia `snapshot_YYYYMMDD_HHMMSS.jpg` |
| **v** | Pornește/oprește înregistrarea `video_YYYYMMDD_HHMMSS.avi` |
| **q** | Închide aplicația |

Fotografiile și videoclipurile se salvează în **directorul curent**, din care ați pornit programul.

**Cerințe:** salvați două fotografii cu obiectul în poziții diferite și înregistrați aproximativ 10 secunde video. Verificați în managerul de fișiere imaginile și videoclipul. Notați rezoluția și codec-ul configurat prin `VideoWriter_fourcc(*"XVID")`; verificați dacă videoclipul poate fi redat pe postul folosit.

**Întrebare:** de ce `VideoWriter` trebuie să cunoască rezoluția și de ce frecvența configurată în fișier (20 FPS) nu garantează că achiziția rulează efectiv la 20 FPS?

## Experimentul 3 — Detectarea muchiilor în flux video (40–50 min)

**Fișier:** [`opencv_edges_live.py`](cod/opencv_edges_live.py)

```bash
python3 cod/opencv_edges_live.py
```

Programul afișează alăturat cadrul original și imaginea procesată.

| Tastă | Algoritm |
| --- | --- |
| **0** | Imagine gri (referință) |
| **1** | Canny, după Gaussian Blur |
| **2** | Sobel: gradient orizontal + vertical, combinat prin magnitudine |
| **3** | Laplacian |
| **q** | Închidere |

### Noțiuni teoretice

Într-o imagine gri `I(x,y)`, operatorul Sobel estimează derivatele spațiale `Gx` și `Gy`; intensitatea muchiei este aproximată prin `sqrt(Gx²+Gy²)`. Laplacianul estimează variația de ordinul al doilea, iar Canny include filtrare, suprimarea non-maximelor și selecția muchiilor prin două praguri.

**Cerințe experimentale:**

1. Așezați în fața camerei un obiect cu contur clar și testați modurile 0–3 în aceeași scenă.
2. Comparați răspunsul la zgomot, textură fină, contur și variații de lumină.
3. Modificați în cod pragurile Canny, de exemplu de la `(50,150)` la `(100,200)`. Explicați diferențele.
4. Comparați execuția la 640×480 și 1280×720, dacă rezoluțiile sunt acceptate de cameră.

**De prezentat:** o comparație vizuală a celor trei metode și o explicație a rolului filtrării Gaussian.

## Experimentul 4 — Identificarea obiectelor după culoare HSV (40–50 min)

**Fișier:** [`opencv_color_tracking.py`](cod/opencv_color_tracking.py)

```bash
python3 cod/opencv_color_tracking.py
```

Programul convertește fiecare cadru din BGR în HSV, construiește măști pentru roșu, verde și albastru, efectuează operațiile morfologice **OPEN** și **CLOSE**, extrage contururile și desenează dreptunghiuri pentru regiunile detectate.

| Tastă | Selecție |
| --- | --- |
| **r** | Roșu |
| **g** | Verde |
| **b** | Albastru |
| **a** | Toate cele trei culori |
| **0** | Nicio culoare |
| **q** | Închidere |

**De analizat:** în OpenCV, H în HSV este reprezentat uzual în intervalul 0–179, iar roșul necesită două intervale de nuanță, fiind aproape de ambele capete ale axei H. Filtrarea `OPEN` elimină regiuni izolate mici, iar `CLOSE` umple goluri din mască. Programul original ignoră contururile cu aria sub 2% din suprafața cadrului.

**Cerințe:** testați trei obiecte în aceeași scenă; modificați distanța, iluminarea și valoarea `min_area`; observați detecțiile false și obiectele omise; identificați în cod pragurile HSV pentru fiecare culoare și explicați de ce intervine filtrarea morfologică.

## Experimentul 5 — Evaluarea performanței CPU în Jupyter (35–45 min)

**Notebook:** [`pipeline CPU.ipynb`](notebook/pipeline%20CPU.ipynb) (din arhiva furnizată).  
**Date experimentale originale:** `timing_rpi_sobel_pipeline.csv` și `timing_rpi_sobel_pipeline1.csv` din arhiva `RPi_OpenCV.zip`, pentru comparație cu măsurătorile proprii. **Fișierele CSV nu sunt încă publicate în acest director GitHub**; utilizați momentan copiile din arhiva originală.

Notebook-ul parcurge explicit:

```text
Captură BGR → grayscale → threshold → Gaussian Blur
            → Sobel X/Y → combinare gradient → imagine BGR
```

El procesează, în configurația inițială, **100 de cadre la 640×480** și calculează separat:

- timpul mediu **captură + procesare** (`t_capture_plus_proc_ms`);
- timpul mediu **doar procesare** (`t_proc_ms`);
- throughput/FPS corespunzător celor două intervale;
- un jurnal CSV cu timpi individuali, dimensiune și numele pipeline-ului.

**Lucru în desktop Raspberry Pi Connect/VNC:** deschideți Jupyter din mediul Python al laboratorului, selectați notebook-ul și rulați celulele în ordine. Închideți celelalte programe care utilizează camera.

**Cerințe:**

1. Rulați pipeline-ul inițial cu `NUM_FRAMES=100`; notați timpii și FPS.
2. Repetați pentru 320×240 și 1280×720 (dacă modelul camerei permite).
3. Comparați diferența dintre timpul de procesare și timpul de captură + procesare.
4. Comparați măsurătorile proprii cu fișierele CSV originale din arhivă, fără a presupune că au fost realizate cu aceeași configurație hardware/software.

**Interpretare:** FPS calculat numai din timpul de procesare **nu este** FPS efectiv al întregului sistem. Notebook-ul folosește `time.time()`; ca extindere, comparați cu `time.perf_counter()` și explicați diferența dintre latență și throughput.

## Rezultate de prezentat

1. Demonstrarea camerei USB în OpenCV.
2. Două fotografii și un videoclip AVI înregistrat de experimentul 2.
3. Comparația Canny/Sobel/Laplacian în aceeași scenă.
4. Identificarea a trei obiecte colorate și analiza unui caz de detecție falsă.
5. Un tabel cu timpii și FPS măsurate pentru cel puțin două rezoluții, plus fișierul CSV rezultat din notebook.

## Referințe

- Programele originale și notebook-ul furnizate de titular în `RPi_OpenCV.zip`.
- [OpenCV — VideoCapture și video I/O](https://docs.opencv.org/4.x/dd/d43/tutorial_py_video_display.html).
- [OpenCV — Canny](https://docs.opencv.org/4.x/da/d22/tutorial_py_canny.html).
- [OpenCV — Transformări morfologice](https://docs.opencv.org/4.x/d9/d61/tutorial_py_morphological_ops.html).
- [Random Nerd Tutorials — Set Up USB Camera for OpenCV on Raspberry Pi](https://randomnerdtutorials.com/set-up-usb-camera-opencv-raspberry-pi/).

**Stadiu:** îndrumător pentru testare didactică; execuția tuturor experimentelor pe echipamentele concrete ale laboratorului nu a fost încă verificată.
