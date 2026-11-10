# Partea a doua — Detectarea mișcării cu PIR și captură foto pe Raspberry Pi 5

[Partea întâi — Raspberry Pi 5 și camera USB](README.md)

**Obiectiv.** Construiți un post care fotografiază scena când senzorul HC-SR501 detectează mișcare. La final, o echipă trebuie să poată demonstra două declanșări distincte, cu o singură fotografie pentru fiecare activare continuă.

**Durată orientativă pentru ambele părți: 3–4 ore.** Rezervați aproximativ o oră pentru camera USB din prima parte și două ore pentru montaj, testarea senzorului și integrare. Pentru varianta de patru ore, adăugați istoricul web și demonstrația între echipe.

## 1. Montați senzorul HC-SR501

Opriți serverul de streaming cu Ctrl+C și verificați că ați eliminat captura periodică din prima parte. Notați dispozitivul video și rezoluția deja testate. Opriți Raspberry Pi din meniul sistemului, așteptați oprirea și deconectați alimentarea USB-C înainte de cablare. Camera rămâne conectată prin USB.

| Bornă HC-SR501 | Conexiune Raspberry Pi 5 | Pin fizic |
| --- | --- | --- |
| VCC | 5 V | 2 |
| OUT | GPIO4 (numerotare BCM), intrare | 7 |
| GND | Masă | 6 |

Identificați bornele după inscripțiile modulului. Pentru acest HC-SR501 alimentarea este de 5 V, iar ieșirea OUT are nivel logic de aproximativ 3,3 V. Verificați traseele înainte de realimentare: tensiunea de 5 V se aplică exclusiv la VCC. În cod, `MotionSensor(4)` desemnează **GPIO4**, aflat pe **pinul fizic 7**. Tutorialul cu AM312 folosește alt model de senzor și altă alimentare.

Realimentați placa, reconectați-vă la desktop și lăsați senzorul să se stabilizeze aproximativ un minut. Orientați lentila PIR și camera spre aceeași zonă de trecere.

## 2. Observați ce consideră senzorul un eveniment

În directorul `~/lab7`, creați fișierul `pir_test.py` cu următorul conținut:

```python
from datetime import datetime
from gpiozero import MotionSensor

with MotionSensor(4) as pir:
    print("Aștept revenirea senzorului în repaus.", flush=True)
    pir.wait_for_no_motion()
    while True:
        pir.wait_for_motion()
        print(datetime.now().isoformat(timespec="seconds"), "OUT activ", flush=True)
        pir.wait_for_no_motion()
        print(datetime.now().isoformat(timespec="seconds"), "OUT în repaus", flush=True)
```

Rulați `python3 ~/lab7/pir_test.py`. Treceți transversal prin fața senzorului, apoi ieșiți din zonă. Notați cele două ore afișate și calculați cât timp a rămas activ semnalul. Repetați testul cu două treceri apropiate. Câte activări distincte a înregistrat programul? Folosiți revenirea în repaus drept reper pentru următoarea trecere. Un senzor PIR răspunde la variația radiației infraroșii din câmpul său; o persoană care stă nemișcată poate rămâne în cadru după ce ieșirea a revenit în repaus.

Observați poziția inițială a reglajului de temporizare al modulului. Modificați ușor temporizarea, repetați aceeași trecere și comparați duratele. Restabiliți reglajul inițial. Opriți programul cu Ctrl+C înainte de a începe captura foto.

## 3. Declanșați camera la începutul fiecărei activări

Creați `~/lab7/pir_foto.py`. Adaptați dispozitivul și rezoluția la valorile verificate în prima parte. Programul folosește ora detecției în numele fotografiei; astfel, la repornire, capturile anterioare se păstrează.

```python
from datetime import datetime
from pathlib import Path
from subprocess import CalledProcessError, run

from gpiozero import MotionSensor

camera = "/dev/video0"
rezolutie = "640x480"
folder = Path.home() / "lab7" / "detectii"
folder.mkdir(parents=True, exist_ok=True)

with MotionSensor(4) as pir:
    print("Aștept repausul inițial.", flush=True)
    pir.wait_for_no_motion()
    while True:
        print("Sistem pregătit pentru o trecere.", flush=True)
        pir.wait_for_motion()
        moment = datetime.now()
        cale = folder / moment.strftime("%Y-%m-%d_%H-%M-%S_%f.jpg")
        print("Mișcare detectată la", moment.isoformat(timespec="seconds"), flush=True)
        try:
            run(["fswebcam", "-d", camera, "-r", rezolutie,
                 "--no-banner", str(cale)], check=True)
            print("Fotografie salvată:", cale.name, flush=True)
        except CalledProcessError as eroare:
            cale.unlink(missing_ok=True)
            print("Captură eșuată. Cod fswebcam:", eroare.returncode, flush=True)
        pir.wait_for_no_motion()
```

Rulați `python3 ~/lab7/pir_foto.py`. În timpul fotografierii, programul așteaptă finalizarea comenzii `fswebcam`. După captură, se rearmează când ieșirea PIR revine în repaus.

| Probă | Ce verificați |
| --- | --- |
| Scena în repaus | După stabilizare, notați câte fișiere sunt în folder. Urmăriți dacă numărul rămâne constant timp de 30 de secunde. |
| Două treceri separate | La fiecare mesaj „Sistem pregătit”, treceți o dată prin zonă. Identificați cele două fișiere noi și verificați imaginile. |
| Mișcare continuă | Urmăriți numărul de fotografii cât timp OUT rămâne activ. Corelați rezultatul cu durata măsurată la testul PIR. |
| Poziționare | Dacă senzorul declanșează, dar persoana a ieșit deja din cadru, ajustați orientarea camerei și a senzorului. Repetați aceeași trecere și comparați imaginile. |

Pentru demonstrație, folosiți o zonă de trecere controlată de echipă. La final, opriți detectorul cu Ctrl+C. Dacă apar detecții fără imagini, testați separat comanda de captură din prima parte și verificați dacă alt program folosește camera.

## 4. Extensie pentru a patra oră — istoricul detecțiilor în browser

Creați o pagină Flask care afișează ultimele cinci fotografii, de la cea mai recentă la cea mai veche. Directorul `~/lab7/detectii` este comun detectorului și paginii web. Detectorul gestionează camera; pagina citește fișierele. Numele fiecărui fișier conține data și ora evenimentului.

**Pași de implementare**

1. Într-un fișier separat, creați o aplicație Flask. Pentru ruta `/`, listați imaginile cu `sorted(folder.glob("*.jpg"), reverse=True)[:5]`.
2. Afișați pentru fiecare rezultat ora detecției și o legătură către imagine. Pentru ruta `/foto/<nume>`, folosiți `send_from_directory(folder, nume)`. Afișați un mesaj când directorul este gol.
3. Porniți aplicația pe interfața rețelei de laborator, la portul 5000, cu modul debug dezactivat. Într-un al doilea terminal reporniți detectorul. De pe PC deschideți `http://ADRESA_IP:5000/`.
4. Declanșați o captură, reîncărcați pagina și deschideți imaginea. Pentru a verifica limita de cinci, produceți șase evenimente și comparați lista din browser cu fișierele de pe placă. Opriți ambele programe cu Ctrl+C.

**De prezentat:** montajul și pinul folosit, o măsurătoare a duratei semnalului activ, două fotografii asociate unor treceri distincte și rezultatul probei de mișcare continuă. La varianta de patru ore, demonstrați și actualizarea istoricului din browser.

## Materiale de referință

- [Lucrarea 12 — Senzorul PIR HC-SR501 și Raspberry Pi](https://cti.ubm.ro/si/Lucrarea%2012%20-%20Senzorul%20PIR.pdf)
- Fișa de laborator „RPi 5 – Camera – Detectare Mișcare (PIR)”, pentru integrarea GPIO4 cu o cameră USB și fswebcam.
- [Detectarea mișcării în Python cu GPIO Zero — Random Nerd Tutorials](https://randomnerdtutorials.com/raspberry-pi-detect-motion-pir-python/)
- [Captură foto la detecție — Random Nerd Tutorials](https://randomnerdtutorials.com/raspberry-pi-motion-detector-photo-capture/). Exemplul de referință folosește o cameră Raspberry Pi V2; montajul acestei lucrări utilizează camera USB din prima parte.
