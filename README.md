# GardenSlave

**Autonomous Soil Moisture & Tank-Level Controller**  
Raspberry Pi Pico (or Arduino Nano) → Soil Moisture Probe → Water Pump → Float-Switch Alarm

---

![GardenSlave PCB](docs/images/pcb_top.jpg)  
*Figure: Assembled GardenSlave board (Pico footprint).*

---

## Overview

GardenSlave automates irrigation based on soil moisture and reservoir level:

- **Soil Moisture Monitoring**  
  Reads an analog moisture probe; if the soil is below your dry threshold, it activates a 12 V water pump.

- **Tank-Level Detection**  
  An active-LOW float switch senses when the reservoir is empty. In that state:  
  1. Watering halts.  
  2. An SOS melody loops until you refill.  
  3. A confirmation tune (“Zelda”) plays once on refill.

- **Dual-Platform Support**  
  Designed for a **Pico** footprint but compatible with an **Arduino Nano** pinned into the same holes.  
  Select via `#define PICO` or `#define NANO` at the top of `gardenslave.ino`.

- **Simple, Blocking Logic**  
  Uses `delay()`-based timing for clarity; easy to bench-test and extend.

---

## Features

- **Configurable Thresholds**  
  Adjust `AIR_VALUE`, `WATER_VALUE`, and `DRY_THRESHOLD` in code to match your sensor’s calibration.

- **Alert Melodies**  
  - **SOS**: … --- … pattern (100 ms/300 ms delays at 1047 Hz).  
  - **Zelda tune**: eight-note confirmation sequence.

- **On-Board Power Regulation**  
  Buck converter steps 12 V down to 5 V for the Pico/Nano and status LEDs.

- **Visual Indicators**  
  - **Power LED**: 5 V regulator output present  
  - **Relay LED**: pump is active

---

## Hardware & Wiring

![Annotated Board](docs/images/pcb_annotated.jpg)  
*Figure: Key components and connectors.*

### Major Components

- **MCU Footprint**  
  - 40-pin Raspberry Pi Pico (RP2040) layout  
  - Many builders use an Arduino Nano inserted into the same holes.

- **Power Input & Regulation**  
  - **12 V barrel jack** → on-board **eBoot MP1584EN** buck converter  
    ([Amazon B07RVG34WR](https://www.amazon.com/dp/B07RVG34WR?ref_=ppx_hzsearch_conn_dt_b_fed_asin_title_1&th=1)) → **5 V** for MCU & LEDs.

- **Pump Driver**  
  - **SRD-05VDC-SL-C** relay  
    ([Amazon B098JPM9S7](https://www.amazon.com/dp/B098JPM9S7?ref_=ppx_hzsearch_conn_dt_b_fed_asin_title_2&th=1))  
  - Driven by **uxcell S9013H** NPN transistor  
    ([Amazon B07KPBS9VK](https://www.amazon.com/dp/B07KPBS9VK?ref_=ppx_hzsearch_a_conn_dt_b_pd_1))  
  - Flyback diode (1N4007) across relay coil  
  - 2-pin Molex power connector  
    ([Molex 1724480002](https://www.digikey.com/en/products/detail/molex/1724480002/5116918)) delivers +12 V to pump.

- **Sensors**  
  - **Soil Moisture Probe** → 3-pin JST S3B-XH-A connector  
    ([Digikey 1651048](https://www.digikey.com/en/products/detail/jst-sales-america-inc/S3B-XH-A/1651048))  
  - **Float Switch (optional)** → glued 2-pin header; use any mini PP float switch (e.g., DP5200 series).

- **Buzzer**  
  - **mxuteuk Mini Active Piezo Buzzer HF-12095** (10 pcs)  
    ([Amazon B07VK1GJ9X](https://www.amazon.com/dp/B07VK1GJ9X?ref_=ppx_hzsearch_conn_dt_b_fed_asin_title_1))  
  - Soldered directly to Pico/Nano pins.

- **Ground Plane & Layout**  
  - Single large top-layer pour; no buried or stitched vias—kept simple for easy fabrication.

### Connectors & Indicators

| Connector                   | Part & Link                                                                                              | Function                                                          |
| --------------------------- | -------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------- |
| **12 V Barrel Jack**        | Standard 2.1 mm barrel jack                                                                              | Main 12 V power input                                             |
| **Buck Converter**          | [eBoot MP1584EN](https://www.amazon.com/dp/B07RVG34WR?ref_=ppx_hzsearch_conn_dt_b_fed_asin_title_1&th=1)  | Steps 12 V down to 5 V for MCU & LEDs                              |
| **Pump Power (Molex)**      | [Molex 1724480002](https://www.digikey.com/en/products/detail/molex/1724480002/5116918)                  | Switched +12 V to pump; pin 1 = +12 V, pin 2 = GND                |
| **Relay**                   | [SRD-05VDC-SL-C](https://www.amazon.com/dp/B098JPM9S7?ref_=ppx_hzsearch_conn_dt_b_fed_asin_title_2&th=1)  | SPDT relay driving pump                                            |
| **Relay Driver Transistor** | [uxcell S9013H](https://www.amazon.com/dp/B07KPBS9VK?ref_=ppx_hzsearch_a_conn_dt_b_pd_1)                 | NPN transistor for relay coil                                      |
| **Soil Moisture (JST)**     | [JST S3B-XH-A](https://www.digikey.com/en/products/detail/jst-sales-america-inc/S3B-XH-A/1651048)         | VCC/GND/analog out to ADC                                          |
| **Float Switch Header**     | 2-pin 0.1″ header                                                                                        | Active-LOW tank-empty sensor                                       |
| **Buzzer**                  | [mxuteuk HF-12095](https://www.amazon.com/dp/B07VK1GJ9X?ref_=ppx_hzsearch_conn_dt_b_fed_asin_title_1)       | Active 5 V tone generator; solder directly                         |
| **LED – Power / Relay**     | On-board LEDs                                                                                            | Power = regulator OK; Relay = pump energized                       |

---

## Pin Mapping

### Raspberry Pi Pico

| Function          | GPIO  | Notes            |
| ----------------- | ----- | ---------------- |
| **Buzzer**        | GP27  | `tone()` output  |
| **Float Switch**  | GP6   | `INPUT_PULLUP`   |
| **Pump Relay**    | GP16  | `digitalWrite()` |
| **Soil Moisture** | GP26  | ADC0 (“A2”)      |

### Arduino Nano

| Function          | Pin   | Notes            |
| ----------------- | ----- | ---------------- |
| **Buzzer**        | D11   | `tone()` output  |
| **Float Switch**  | D6    | `INPUT_PULLUP`   |
| **Pump Relay**    | D13   | `digitalWrite()` |
| **Soil Moisture** | A2    | `analogRead()`   |

---

## Bill of Materials

- **Microcontroller**  
  - 1 × Raspberry Pi Pico (or Arduino Nano)  
- **Power & Switching**  
  - 1 × [eBoot MP1584EN Buck Converter](https://www.amazon.com/dp/B07RVG34WR?ref_=ppx_hzsearch_conn_dt_b_fed_asin_title_1&th=1)  
  - 1 × [SRD-05VDC-SL-C Relay](https://www.amazon.com/dp/B098JPM9S7?ref_=ppx_hzsearch_conn_dt_b_fed_asin_title_2&th=1)  
  - 1 × [uxcell S9013H Transistor](https://www.amazon.com/dp/B07KPBS9VK?ref_=ppx_hzsearch_a_conn_dt_b_pd_1)  
  - 1 × Flyback diode (1N4007)  
- **Connectors**  
  - 1 × [JST S3B-XH-A](https://www.digikey.com/en/products/detail/jst-sales-america-inc/S3B-XH-A/1651048) – soil moisture sensor  
  - 1 × 2-pin 0.1″ header – float switch  
  - 1 × [Molex 1724480002](https://www.digikey.com/en/products/detail/molex/1724480002/5116918) – pump power  
  - 1 × 12 V barrel jack – main power input  
- **Sensors & Actuators**  
  - 1 × Soil moisture probe (analog)  
  - 1 × Plastic PP float switch (e.g., DP5200 series)  
  - 1 × 12 V water pump  
  - 1 × [mxuteuk HF-12095 Piezo Buzzer](https://www.amazon.com/dp/B07VK1GJ9X?ref_=ppx_hzsearch_conn_dt_b_fed_asin_title_1)  
- **Miscellaneous**  
  - Decoupling capacitors, resistors, headers, solder, PCB

---

## Build & Install

1. **Clone repository**  
   ```bash
   git clone https://github.com/yourname/gardenslave.git
   cd gardenslave
