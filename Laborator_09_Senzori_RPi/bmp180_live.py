#!/usr/bin/env python3
"""Laborator 9, task 3.5: temperatura si presiunea BMP180 (GY-87), in timp real.

Ruleaza din terminalul desktopului Raspberry Pi (Connect Screen Sharing / VNC).
Afiseaza doua grafice Matplotlib si salveaza CSV + PNG in ~/lab9.
"""

import csv
import time
from datetime import datetime
from pathlib import Path

import board
import bmp180
import matplotlib.pyplot as plt

INTERVAL_S = 2.0       # interval intre citiri
DURATA_S = 180.0       # 3 minute; modificati dupa necesitate
DIRECTOR = Path.home() / "lab9"
CSV_PATH = DIRECTOR / "bmp180_live.csv"
PNG_PATH = DIRECTOR / "bmp180_live.png"


def main():
    DIRECTOR.mkdir(parents=True, exist_ok=True)

    i2c = board.I2C()
    senzor = bmp180.BMP180(i2c, address=0x77)

    plt.ion()
    fig, (ax_t, ax_p) = plt.subplots(2, 1, figsize=(10, 7), sharex=True)
    fig.suptitle("GY-87 / BMP180 — temperatura si presiune, in timp real")
    linie_t, = ax_t.plot([], [], color="tab:red", linewidth=2)
    linie_p, = ax_p.plot([], [], color="tab:blue", linewidth=2)
    ax_t.set_ylabel("Temperatura [°C]")
    ax_p.set_ylabel("Presiune [hPa]")
    ax_p.set_xlabel("Timp de la pornire [s]")
    for ax in (ax_t, ax_p):
        ax.grid(True, alpha=0.3)
        ax.set_xlim(0, 30)
    fig.tight_layout()
    plt.show(block=False)

    timpi, temperaturi, presiuni = [], [], []
    inceput = time.monotonic()
    urmatoarea_citire = inceput

    print("BMP180 (0x77): citire la fiecare 2 s; oprire Ctrl+C sau inchiderea graficului.")
    print("Date CSV:", CSV_PATH)

    try:
        with CSV_PATH.open("w", newline="", encoding="utf-8") as fisier:
            writer = csv.writer(fisier)
            writer.writerow(["timestamp", "timp_s", "temperatura_C", "presiune_hPa"])

            while time.monotonic() - inceput < DURATA_S and plt.fignum_exists(fig.number):
                if time.monotonic() < urmatoarea_citire:
                    plt.pause(0.1)  # permite actualizarea ferestrei grafice
                    continue

                timp_s = time.monotonic() - inceput
                try:
                    temperatura = float(senzor.temperature)
                    presiune = float(senzor.pressure)  # biblioteca: hPa
                except (OSError, RuntimeError) as eroare:
                    print("Eroare de citire BMP180:", eroare)
                else:
                    timestamp = datetime.now().astimezone().isoformat(timespec="seconds")
                    writer.writerow([timestamp, round(timp_s, 1),
                                     round(temperatura, 2), round(presiune, 2)])
                    fisier.flush()

                    timpi.append(timp_s)
                    temperaturi.append(temperatura)
                    presiuni.append(presiune)
                    linie_t.set_data(timpi, temperaturi)
                    linie_p.set_data(timpi, presiuni)

                    ax_t.set_xlim(0, max(30, timp_s + 5))
                    marja_t = max(0.3, (max(temperaturi) - min(temperaturi)) * 0.15)
                    marja_p = max(0.5, (max(presiuni) - min(presiuni)) * 0.15)
                    ax_t.set_ylim(min(temperaturi) - marja_t, max(temperaturi) + marja_t)
                    ax_p.set_ylim(min(presiuni) - marja_p, max(presiuni) + marja_p)
                    ax_t.set_title(f"T = {temperatura:.2f} °C")
                    ax_p.set_title(f"p = {presiune:.2f} hPa")
                    fig.canvas.draw_idle()

                    print(f"{timestamp}  T={temperatura:.2f} °C  p={presiune:.2f} hPa")

                urmatoarea_citire += INTERVAL_S
                if urmatoarea_citire < time.monotonic():
                    urmatoarea_citire = time.monotonic() + INTERVAL_S
                plt.pause(0.05)

    except KeyboardInterrupt:
        print("\nAchizitie intrerupta de utilizator.")
    finally:
        if timpi:
            fig.savefig(PNG_PATH, dpi=150)
            print("Grafic salvat:", PNG_PATH)
        else:
            print("Nu au fost obtinute esantioane.")
        plt.close(fig)
        plt.ioff()
        print("Inregistrare finalizata:", CSV_PATH)


if __name__ == "__main__":
    main()
