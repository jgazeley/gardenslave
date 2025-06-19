# GardenSlave

**Autonomous Soil Moisture & Tank-Level Controller**  
Raspberry Pi Pico (or Arduino Nano) → Soil Moisture Probe → Water Pump → Float-Switch Alarm

---

![GardenSlave PCB](https://github.com/user-attachments/assets/807e94d5-aabe-4f0c-85b5-37407a0cc3f2)  
*Top view of the GardenSlave PCB in KiCad showing the Pico footprint, relay, buck converter, and connectors.*

![GardenSlave Enclosure](https://github.com/user-attachments/assets/d1894482-3f5e-43a2-b8ec-4f3f3e52a7fc)  
*GardenSlave installed in a custom 3D-printed enclosure, with openings for the soil sensor cable and pump power lead.*

![Assembled Unit in Operation](https://github.com/user-attachments/assets/39d3a17f-0748-4503-b9fb-3a0773ff0711)  
*GardenSlave powered up and connected to the reservoir float switch and soil probe.*

![KiCad 3D View](https://github.com/user-attachments/assets/962257ac-9c27-4b5b-bc6f-b8512ae104a8)  
*KiCad 3D render of the PCB layout, illustrating component placement.*

---

## Overview

GardenSlave automates irrigation:

- **Soil Moisture Monitoring**  
  Reads an analog moisture probe on **A2**; if the raw reading ≥ `DRY_THRESHOLD` (600), the pump runs for 1 s every 6 s.

- **Tank-Level Detection**  
  Float switch on **GP6** (`INPUT_PULLUP`) senses “empty” when HIGH:  
  1. Watering halts and SOS melody loops (`Sos()`).  
  2. Upon refill, `Zelda()` plays once, then normal watering resumes.

- **Dual-Platform Support**  
  By default `#define PICO` (GP27 buzzer, GP16 pump).  
  Uncomment the `#ifdef NANO` block to use an Arduino Nano (D11, D13, A2).

- **Blocking Logic**  
  Uses `delay()` so timing matches exactly your sketch.

---

## Wiring & Pinout

| Signal           | Pico GPIO / Nano Pin | Code Name        |
| ---------------- | -------------------- | ---------------- |
| **Buzzer**       | GP27 / D11           | `buzzer`         |
| **Float Switch** | GP6 / D6             | `float_sensor`   |
| **Pump Relay**   | GP16 / D13           | `pump`           |
| **Soil Moisture**| A2 (GP26 on Pico)    | —                |

### Connectors & Parts

- **Soil Moisture Probe**  
  3-pin JST S3B-XH-A → A2  
  [JST S3B-XH-A (Digikey #1651048)](https://www.digikey.com/en/products/detail/jst-sales-america-inc/S3B-XH-A/1651048)

- **Float Switch (optional)**  
  2-pin header for DP5200 float switch (6-pack)  
  [DP5200 Float Switch on Amazon](https://www.amazon.com/dp/B072QCHQ2P)

- **Pump Power Connector**  
  2-pin Molex 1724480002 (Mini-Fit Jr.)  
  [Molex 1724480002 (Digikey #5116918)](https://www.digikey.com/en/products/detail/molex/1724480002/5116918)

- **Buck Converter**  
  eBoot MP1584EN (12 → 5 V)  
  [Amazon B07RVG34WR](https://www.amazon.com/dp/B07RVG34WR?ref_=ppx_hzsearch_conn_dt_b_fed_asin_title_1&th=1)

- **Relay**  
  SRD-05VDC-SL-C (5 V coil)  
  [Amazon B098JPM9S7](https://www.amazon.com/dp/B098JPM9S7?ref_=ppx_hzsearch_conn_dt_b_fed_asin_title_2&th=1)

- **Relay Driver Transistor**  
  uxcell S9013H NPN  
  [Amazon B07KPBS9VK](https://www.amazon.com/dp/B07KPBS9VK?ref_=ppx_hzsearch_a_conn_dt_b_pd_1)

- **Piezo Buzzer**  
  mxuteuk HF-12095, 5 V active  
  [Amazon B07VK1GJ9X](https://www.amazon.com/dp/B07VK1GJ9X?ref_=ppx_hzsearch_conn_dt_b_fed_asin_title_1)

---

## Calibration Constants

```cpp
const int AirValue      = 632;  // ADC in air (dry)
const int WaterValue    = 450;  // ADC in water (wet)
const int DryValue 		  = 600;  // soilMoistureValue ≥ this → pump on
