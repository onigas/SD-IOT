# Laboratorul 9 — Achiziția, înregistrarea și vizualizarea datelor de la senzori

[Înapoi la Partea a II-a — Sisteme cu Raspberry Pi](../README.md#raspberry-pi)

**Platformă:** Raspberry Pi 5 cu Raspberry Pi OS; **durată orientativă:** 3–4 ore.  
**Stadiu:** îndrumător inițial pentru testare pe posturile de laborator; codul nu este încă validat pe hardware-ul concret.

## Obiective

La finalul lucrării studenții vor putea să:
1. conecteze și citească un senzor DHT11/DHT22 pe GPIO și să înregistreze măsurători cu marcă de timp;
2. detecteze periferice I²C și să citească accelerația, viteza unghiulară, temperatura și presiunea de la GY-87;
3. salveze date structurate în CSV, să verifice calitatea eșantioanelor și să reprezinte evoluția măsurătorilor;
4. controleze orientarea unei plăci virtuale 3D în browser prin înclinarea efectivă a modulului MPU6050.

**Sursa de pornire:** [DHT data logger — Random Nerd Tutorials](https://randomnerdtutorials.com/raspberry-pi-temperature-humidity-data-logger/) și [BME280 data logger — Random Nerd Tutorials](https://randomnerdtutorials.com/raspberry-pi-bme280-data-logger/). Al doilea experiment este **adaptat pentru GY-87**, deci nu folosește codul sau biblioteca BME280.

## Echipament și arhitectură

- Raspberry Pi 5 și acces prin terminal;
- DHT11 sau DHT22, preferabil pe modul cu rezistor de pull-up (altfel adăugați un rezistor de aproximativ 4,7–10 kΩ între DATA și 3,3 V);
- modul GY-87 cu MPU6050 și BMP180; cabluri, breadboard;
- PC/telefon în aceeași rețea pentru vizualizarea 3D.

~~~text
DHT11/DHT22 -- GPIO4 ------------------+
                                       |
GY-87 -------- I²C (GPIO2 / GPIO3) ----+--> Raspberry Pi 5
                                              |--> fișiere CSV
                                              |--> HTTP/Flask --> browser: placă 3D
~~~

Senzorii pot fi testați succesiv; nu trebuie rulați simultan pentru a îndeplini cerințele. Ambele proiecte au propriul director de lucru, în ~/lab9.

## 1. Configurarea mediului

În Raspberry Pi OS, deschideți un terminal și pregătiți mediul Python:

~~~bash
sudo apt update
sudo apt install -y python3-venv python3-pip i2c-tools
mkdir -p ~/lab9
cd ~/lab9
python3 -m venv .venv --system-site-packages
source ~/lab9/.venv/bin/activate
python -m pip install adafruit-blinka adafruit-circuitpython-dht \
  adafruit-circuitpython-mpu6050 circuitpython-bmp180 flask matplotlib
~~~

La fiecare terminal nou reactivați mediul cu ~~~source ~/lab9/.venv/bin/activate~~~.

Verificați pachetele:

~~~bash
python -c "import board, adafruit_dht, adafruit_mpu6050, bmp180, flask; print('Importuri OK')"
~~~

Dacă interfața I²C nu este activă: `sudo raspi-config` → **Interface Options → I2C → Enable**, apoi reporniți placa. Verificați:

~~~bash
ls /dev/i2c-1
~~~

**Compatibilitate:** pe Raspberry Pi 5 și versiunile actuale Raspberry Pi OS, funcționarea DHT prin GPIO poate depinde de versiunea Blinka și de backend-ul GPIO. Dacă la test apar erori de backend sau sincronizare, notați versiunile cu `python -m pip freeze` și mesajul exact; nu presupuneți că un cod pentru modele Raspberry Pi mai vechi rulează identic.

## 2. Experimentul 1 — DHT11/DHT22: temperatură, umiditate, data logger

### 2.1. Conexiuni

| DHT11/DHT22 | Raspberry Pi 5 |
| --- | --- |
| VCC (+) | 3,3 V — pin fizic 1 |
| DATA (S) | GPIO4 (BCM) — pin fizic 7 |
| GND (–) | GND — pin fizic 6 |

Confirmați ordinea bornelor inscripționată pe **modulul efectiv**, deoarece modulele cu 3/4 pini au pinout diferit. Un modul DHT11/DHT22 cu rezistor de pull-up integrat nu necesită un al doilea rezistor. Pentru senzorul neechipat, adăugați rezistorul extern.

### 2.2. Programul de înregistrare

Creați `/lab9/dht_logger.py`:

~~~python
import csv
import time
from datetime import datetime
from pathlib import Path

import board
import adafruit_dht

TIP_SENZOR = "DHT11"  # sau "DHT22"
INTERVAL = 10  # secunde
FISIER = Path.home() / "lab9" / "dht.csv"

clasa = adafruit_dht.DHT11 if TIP_SENZOR == "DHT11" else adafruit_dht.DHT22
senzor = clasa(board.D4, use_pulseio=False)
FISIER.parent.mkdir(parents=True, exist_ok=True)

try:
    with FISIER.open("a", newline="", encoding="utf-8") as fisier:
        csv_writer = csv.writer(fisier)
        if fisier.tell() == 0:
            csv_writer.writerow(["timestamp", "temperatura_C", "umiditate_pct"])
        while True:
            try:
                temp = senzor.temperature
                umiditate = senzor.humidity
                if temp is not None and umiditate is not None:
                    stamp = datetime.now().astimezone().isoformat(timespec="seconds")
                    csv_writer.writerow([stamp, round(temp, 2), round(umiditate, 2)])
                    fisier.flush()
                    print(stamp, f"T={temp:.1f} °C  RH={umiditate:.1f} %")
            except RuntimeError as e:
                print("Citire nereușită:", e)
            time.sleep(INTERVAL)
except KeyboardInterrupt:
    print("\nÎnregistrarea s-a încheiat.")
finally:
    senzor.exit()
~~~

Executați:

~~~bash
cd ~/lab9
source .venv/bin/activate
python dht_logger.py
~~~

Observați cel puțin 10 eșantioane. Apropiați moderat palma de senzor și observați variația temperaturii. Pentru umiditate, comparați rezultatul înainte și după o schimbare controlată a mediului. Opriți cu **Ctrl+C** și verificați:

~~~bash
head -n 5 ~/lab9/dht.csv
tail -n 5 ~/lab9/dht.csv
~~~

**De analizat:** intervalul dintre măsurători, eșantioanele ratate și consecințele unui format CSV cu antet și timestamp. DHT11 și DHT22 diferă ca domeniu și rezoluție; consemnați ce model ați folosit.

### 2.3. Reprezentarea grafică a datelor

Creați `/lab9/plot_dht.py`:

~~~python
import csv
from datetime import datetime
from pathlib import Path

import matplotlib.pyplot as plt

cale = Path.home() / "lab9" / "dht.csv"
t, temp, rh = [], [], []
with cale.open(newline="", encoding="utf-8") as f:
    for rand in csv.DictReader(f):
        t.append(datetime.fromisoformat(rand["timestamp"]))
        temp.append(float(rand["temperatura_C"]))
        rh.append(float(rand["umiditate_pct"]))

if not t:
    raise SystemExit("Fișierul CSV nu conține măsurători.")
fig, axe = plt.subplots(2, 1, sharex=True, figsize=(9, 6))
axe[0].plot(t, temp, marker=".")
axe[0].set_ylabel("Temperatură [°C]")
axe[1].plot(t, rh, marker=".")
axe[1].set_ylabel("Umiditate relativă [%]")
axe[1].set_xlabel("Timp")
fig.autofmt_xdate()
fig.tight_layout()
fig.savefig(str(cale.with_suffix(".png")), dpi=130)
plt.show()
~~~

~~~bash
python plot_dht.py
~~~

În sesiunea grafică Raspberry Pi, accesată prin **Raspberry Pi Connect** sau **VNC**, graficul se afișează direct la executarea `python plot_dht.py`. O copie a graficului se salvează automat în `~/lab9/dht.png`.

## 3. Experimentul 2 — GY-87: I²C și înregistrarea datelor

### 3.1. Ce conține modulul

| Componentă | Măsurători | Adresă I²C uzuală |
| --- | --- | --- |
| MPU6050 | accelerație (3 axe), viteză unghiulară (3 axe), temperatură internă | 0x68 (uneori 0x69) |
| BMP180 | temperatură și presiune | 0x77 |
| Magnetometru | câmp magnetic; varianta depinde de placă | HMC5883L: 0x1E; QMC5883L: 0x0D; alte variante există |

**GY-87 nu măsoară umiditatea.** Magnetometrul nu este utilizat în versiunea de bază a acestui laborator: unele plăci au clone sau acces prin magistrala auxiliară a MPU6050, iar implementarea necesită identificare și calibrare suplimentară.

### 3.2. Conexiuni

| GY-87 | Raspberry Pi 5 |
| --- | --- |
| GND | GND — pin fizic 6 |
| SDA | GPIO2 / SDA1 — pin fizic 3 |
| SCL | GPIO3 / SCL1 — pin fizic 5 |
| 3V3/VCC | alimentare **conform reviziei reale a plăcii** |

**Verificare hardware necesară:** unele module GY-87 au intrare **VCC_IN** cu regulator și pin separat **3.3V** (ieșire de regulator). Nu tratați arbitrar aceste două borne ca echivalente. Asigurați-vă că nivelurile I²C către Raspberry Pi nu depășesc 3,3 V; verificați alimentarea potrivită pentru revizia existentă la post.

Cu modulul conectat, executați:

~~~bash
sudo i2cdetect -y 1
~~~

Căutați în mod obișnuit **0x68** și **0x77**. Dacă una lipsește, verificați mai întâi cablarea și caracteristicile modulului. Adresele detectate nu reprezintă singure o identificare definitivă a cipului.

### 3.3. Data logger GY-87

Creați `/lab9/gy87_logger.py`:

~~~python
import csv
import time
from datetime import datetime
from pathlib import Path

import board
import adafruit_mpu6050
import bmp180

I2C = board.I2C()
mpu = adafruit_mpu6050.MPU6050(I2C)
baro = bmp180.BMP180(I2C)
cale = Path.home() / "lab9" / "gy87.csv"
cale.parent.mkdir(parents=True, exist_ok=True)

coloane = [
    "timestamp", "ax_m_s2", "ay_m_s2", "az_m_s2",
    "gx_rad_s", "gy_rad_s", "gz_rad_s",
    "temp_mpu_C", "temp_bmp_C", "presiune_hPa",
]
try:
    with cale.open("a", newline="", encoding="utf-8") as f:
        w = csv.writer(f)
        if f.tell() == 0:
            w.writerow(coloane)
        while True:
            a = mpu.acceleration      # m/s²
            g = mpu.gyro              # rad/s
            t_mpu = mpu.temperature
            t_bmp = baro.temperature
            p = baro.pressure         # hPa, în biblioteca circuitpython-bmp180
            stamp = datetime.now().astimezone().isoformat(timespec="seconds")
            w.writerow([stamp, *[round(x, 4) for x in a],
                        *[round(x, 5) for x in g],
                        round(t_mpu, 2), round(t_bmp, 2), round(p, 2)])
            f.flush()
            print(stamp, "a=", tuple(round(x, 2) for x in a),
                  "g=", tuple(round(x, 2) for x in g),
                  f"p={p:.2f} hPa")
            time.sleep(1)
except KeyboardInterrupt:
    print("\nLogger oprit.")
~~~

~~~bash
python gy87_logger.py
~~~

Înregistrați 15 secunde cu modulul nemișcat, apoi înclinați-l lent succesiv în jurul a două axe. Opriți cu **Ctrl+C**. Deschideți fișierul `/lab9/gy87.csv` și comparați valorile în repaus și în mișcare.

**Verificări:** în repaus, norma vectorului accelerației trebuie să fie aproximativ 9,81 m/s², în funcție de calibrare și orientare; viteza unghiulară ideală este aproape de zero. Temperatura MPU6050 este temperatura internă a circuitului, nu neapărat temperatura aerului.

### 3.4. Extindere: grafice pentru GY-87

Folosind modelul de la secțiunea 2.3, reprezentați în funcție de timp:
- cele trei accelerații `ax_m_s2`, `ay_m_s2`, `az_m_s2`;
- vitezele unghiulare `gx_rad_s`, `gy_rad_s`, `gz_rad_s`;
- presiunea `presiune_hPa`.

Explicați diferența dintre o schimbare a **orientării statice** și o **rotație în curs**.

## 4. Experimentul 3 — Placă virtuală 3D controlată de GY-87

Vom realiza o reprezentare 3D într-un browser, folosind **Flask + CSS 3D**, fără biblioteci grafice externe. Placa este un paralelipiped stilizat cu marcaje X/Y; ea se înclină la mișcarea senzorului real. Aceasta este o **vizualizare a înclinării** (roll și pitch), nu o soluție AHRS completă.

Estimare simplificată din accelerometru:

~~~text
roll  = atan2(ay, az)
pitch = atan2(-ax, sqrt(ay² + az²))
~~~

Unghiurile sunt raportate la axele senzorului. În cazul accelerațiilor dinamice mari, înclinarea calculată din gravitație devine perturbată. Componenta **yaw** nu este determinabilă absolut numai din accelerometru; integrarea giroscopului fără corecție produce derivă.

### 4.1. Serverul pentru vizualizare

Creați `/lab9/gy87_3d.py`:

~~~python
import math
import threading
import time

import board
import adafruit_mpu6050
from flask import Flask, jsonify, render_template_string

i2c = board.I2C()
mpu = adafruit_mpu6050.MPU6050(i2c)
app = Flask(__name__)
lock = threading.Lock()
ultimul = {"roll": 0, "pitch": 0, "ax": 0, "ay": 0, "az": 0}

def cititor():
    # Filtrare exponențială a înclinării din accelerometru.
    global ultimul
    while True:
        try:
            ax, ay, az = mpu.acceleration
            roll = math.degrees(math.atan2(ay, az))
            pitch = math.degrees(math.atan2(-ax, math.hypot(ay, az)))
            with lock:
                # În apropierea inversării plăcii, unghiul roll are
                # discontinuitate ±180°; păstrăm experimentul pentru
                # înclinări moderate, fără răsturnare completă.
                ultimul = {
                    "roll": 0.75 * ultimul["roll"] + 0.25 * roll,
                    "pitch": 0.75 * ultimul["pitch"] + 0.25 * pitch,
                    "ax": round(ax, 2), "ay": round(ay, 2),
                    "az": round(az, 2),
                }
        except (OSError, RuntimeError) as eroare:
            print("Citire MPU6050:", eroare)
        time.sleep(0.08)

PAGINA = """
<!DOCTYPE html><html lang="ro"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>GY-87 — orientare 3D</title>
<style>
body{background:#101a29;color:#eaf2ff;font:16px system-ui;
  text-align:center;margin:0;padding:24px}
h1{font-size:24px;margin:8px 0}
p{color:#bdd0e5}
.scena{perspective:850px;height:360px;display:grid;place-items:center}
.placa{position:relative;width:260px;height:170px;
  transform-style:preserve-3d;transition:transform .1s linear}
.fata,.spate{position:absolute;inset:0;border:4px solid #82a58b;
  border-radius:14px;backface-visibility:hidden}
.fata{background:linear-gradient(140deg,#207456,#124e44);
  transform:translateZ(9px);box-shadow:0 12px 30px #0008}
.spate{background:#0f4036;transform:rotateY(180deg) translateZ(9px)}
.fata:before{content:"MPU6050";position:absolute;left:70px;top:52px;
  padding:16px;background:#17293b;border:2px solid #6a8fa9;border-radius:5px}
.fata:after{content:"GY-87   X →     Y ↑";position:absolute;
  left:28px;bottom:20px;color:#d9f7e6;font-weight:bold}
.margine{position:absolute;background:#376b5a;transform-style:preserve-3d}
.sus,.jos{width:260px;height:18px;left:0}
.sus{top:0;transform-origin:top;transform:rotateX(-90deg)}
.jos{bottom:0;transform-origin:bottom;transform:rotateX(90deg)}
.stanga,.dreapta{width:18px;height:170px;top:0}
.stanga{left:0;transform-origin:left;transform:rotateY(90deg)}
.dreapta{right:0;transform-origin:right;transform:rotateY(-90deg)}
#date{font:18px ui-monospace,monospace;color:#a8f0c6}
</style></head><body>
<h1>Orientarea 3D a modulului GY-87</h1>
<p>Înclinați modulul fizic; plăcuța virtuală urmărește roll/pitch.</p>
<div class="scena"><div class="placa" id="placa">
<div class="fata"></div><div class="spate"></div>
<div class="margine sus"></div><div class="margine jos"></div>
<div class="margine stanga"></div><div class="margine dreapta"></div>
</div></div><div id="date">Se așteaptă măsurători…</div>
<p>Model demonstrativ: accelerație → înclinare (nu yaw absolut).</p>
<script>
async function actualizeaza(){
 try{
   const r = await fetch('/date', {cache:'no-store'});
   if(!r.ok) throw new Error('HTTP '+r.status);
   const d = await r.json();
   document.getElementById('placa').style.transform =
      'rotateX('+d.roll+'deg) rotateY('+d.pitch+'deg)';
   document.getElementById('date').textContent =
      'Roll: '+d.roll.toFixed(1)+'°  Pitch: '+d.pitch.toFixed(1)+'°'+
      '   a=['+d.ax+', '+d.ay+', '+d.az+'] m/s²';
 }catch(e){document.getElementById('date').textContent='Eroare: '+e;}
}
setInterval(actualizeaza, 120);
actualizeaza();
</script></body></html>
"""

@app.get("/")
def pagina():
    return render_template_string(PAGINA)

@app.get("/date")
def date():
    with lock:
        return jsonify(ultimul)

if __name__ == "__main__":
    threading.Thread(target=cititor, daemon=True).start()
    app.run(host="0.0.0.0", port=5000, debug=False)
~~~

**Pornire:**

~~~bash
cd ~/lab9
source .venv/bin/activate
python gy87_3d.py
~~~

În alt terminal aflați IP-ul Raspberry Pi:

~~~bash
hostname -I
~~~

Pe PC deschideți:

~~~text
http://ADRESA_IP:5000/
~~~

Înlocuiți `ADRESA_IP` cu adresa plăcii. Înclinați lent GY-87 în jurul axelor X și Y, comparând modelul virtual cu mișcarea modulului fizic. Dacă un sens apare inversat, investigați orientarea convenției de axe și semnele unghiurilor; sensurile depind de poziționarea plăcii și de transformarea vizuală.

### 4.2. Probe și limitări

1. **Repaus:** puneți modulul pe masă și urmăriți stabilitatea valorilor.
2. **Înclinare:** rotiți lent în jurul axei X, apoi în jurul axei Y.
3. **Mișcare dinamică:** deplasați placa liniar fără a-i modifica intenționat orientarea; analizați de ce afișajul poate indica o înclinare falsă.
4. **Rotație în plan:** rotiți modulul în jurul axei verticale și explicați de ce yaw nu poate fi recuperat din accelerometru.
5. **Extindere opțională:** comparați estimarea de mai sus cu un filtru complementar accelerometru + giroscop; identificați avantajul și deriva reziduală.

## 5. Rezultate de prezentat

- fișierul `dht.csv` și graficul `dht.png`;
- rezultatul scanării I²C, plus fișierul `gy87.csv` cu măsurători din repaus și din mișcare;
- demonstrarea plăcii 3D live într-un browser de pe alt dispozitiv;
- răspuns scurt: ce măsoară DHT11/DHT22, ce oferă GY-87, ce înseamnă roll/pitch/yaw și de ce yaw nu poate fi determinat direct numai din accelerație.

**Observație pentru îndrumător:** aceasta este o versiune de dezvoltare. Bibliotecile, adresele, cablarea exactă și sensurile de rotație trebuie confirmate pe GY-87 și Raspberry Pi 5 din laborator înainte de publicarea ca versiune finală testată.

## 6. Materiale de referință

- [Random Nerd Tutorials — Raspberry Pi DHT11/DHT22 Data Logger](https://randomnerdtutorials.com/raspberry-pi-temperature-humidity-data-logger/)
- [Random Nerd Tutorials — Raspberry Pi BME280 Data Logger](https://randomnerdtutorials.com/raspberry-pi-bme280-data-logger/) — referință metodologică, **adaptată pentru GY-87**
- [Adafruit — MPU6050 Python/CircuitPython](https://learn.adafruit.com/mpu6050-6-dof-accelerometer-and-gyro/python-and-circuitpython)
- [CircuitPython BMP180 — exemplu](https://circuitpython-bmp180.readthedocs.io/en/stable/examples.html)
- [SunFounder — GY-87 și variantele de magnetometru](https://docs.sunfounder.com/projects/elite-explorer-kit/en/latest/basic_projects/09_basic_gy87.html)
- [Random Nerd Tutorials — instalare Mosquitto](https://randomnerdtutorials.com/how-to-install-mosquitto-broker-on-raspberry-pi/) și [testarea brokerului](https://randomnerdtutorials.com/testing-mosquitto-broker-and-client-on-raspbbery-pi/) — propuse pentru **Laboratorul 10**, separat.
