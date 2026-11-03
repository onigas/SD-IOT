# Laboratorul 6 — ESP-NOW: comunicații între ESP32

**Comunicații directe între noduri de senzori · Comparație cu ESP-MESH · Gateway ESP-NOW + Wi-Fi**

## Materiale

- **[Introducere teoretică — citire directă pe GitHub](documentatie/Laborator_06.md)**: principiu de funcționare, adrese MAC, topologii, comparație cu ESP-MESH, arhitectura gateway-ului și datele GY-87.

## Cele patru experimente

| Task | Activitate |
| --- | --- |
| T1 | ESP-NOW One-to-One / Two-Way, cu GY-87 |
| T2 | ESP-NOW One-to-Many |
| T3 | ESP-NOW Many-to-One, date de la mai multe grupe |
| T4 | ESP-NOW + Wi-Fi Web Server, dashboard centralizat |

**Condiții ale laboratorului:** routerul Wi-Fi este utilizat pentru gateway și interfața web; nodurile care transmit prin ESP-NOW nu trebuie să se conecteze la router. Toate dispozitivele ESP-NOW trebuie să opereze pe canalul radio corespunzător gateway-ului.

**Senzor:** utilizați modulul **GY-87** în locul modulelor BME280/DHT22 din tutorialele de referință.

[Înapoi la lista laboratoarelor](../README.md)
