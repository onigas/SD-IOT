# Laboratorul 6 — ESP-NOW: comunicații între ESP32

**Comunicații directe fără router pentru nodurile de senzori · Comparație cu ESP-MESH · Gateway ESP-NOW + Wi-Fi**

## Materiale

- **[Introducere teoretică — citire directă pe GitHub](documentatie/Laborator_06.md)**: principiu de funcționare, adrese MAC, topologii, comparație cu ESP-MESH, arhitectura gateway-ului și datele GY-87.
- `cod/` — proiecte Arduino separate pentru experimente (vor fi adăugate în etapa următoare).
- `imagini/` — scheme și ilustrații (vor fi adăugate ulterior).

## Cele patru experimente planificate

| Task | Activitate | Stadiu |
| --- | --- | --- |
| T1 | ESP-NOW One-to-One / Two-Way, cu GY-87 | În pregătire |
| T2 | ESP-NOW One-to-Many | În pregătire |
| T3 | ESP-NOW Many-to-One, date de la mai multe grupe | În pregătire |
| T4 | ESP-NOW + Wi-Fi Web Server, dashboard centralizat | În pregătire |

**Condiții ale laboratorului:** avem un router Wi-Fi pentru gateway și interfața web; plăcile care transmit prin ESP-NOW nu trebuie să se conecteze la el. Toate dispozitivele ESP-NOW trebuie să opereze pe canalul corespunzător gateway-ului.

**Senzor:** se folosește modulul **GY-87**, nu BME280/DHT22 din tutorialele-sursă.

[Înapoi la lista laboratoarelor](../README.md)
