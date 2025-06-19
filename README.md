# GardenSlave

**Autonomous Plant Watering System**  
Raspberry Pi Pico (or Arduino Nano) → Soil Moisture Sensor → 12V Water Pump → Float-Switch Alarm

---

## Overview

GardenSlave automates irrigation:

- **Soil Moisture Monitoring**  
  Reads an analog moisture probe on **A2**; if the raw reading ≥ `DryValue` (600), the pump runs for 1 s every 6 s.

- **Tank-Level Detection**  
  Float switch on **GP6** (`INPUT_PULLUP`) senses “empty” when HIGH:  
  1. Watering halts and SOS melody loops (`Sos()`).  
  2. Upon refill, `Zelda()` plays once, then normal watering resumes.

- **Dual-Platform Support**  
  By default `#define PICO` (GP27 buzzer, GP16 pump).  
  Uncomment the `#ifdef NANO` block to use an Arduino Nano (D11, D13, A2).

---

## PCB & 3D Model
**PCB:**
![GardenSlave PCB](https://github.com/user-attachments/assets/807e94d5-aabe-4f0c-85b5-37407a0cc3f2)  
*Top view of the GardenSlave PCB in KiCad showing the Pico, relay, buck converter, and connector footprints.*

![KiCad 3D View](https://github.com/user-attachments/assets/962257ac-9c27-4b5b-bc6f-b8512ae104a8)  
*KiCad 3D render of the PCB layout.*

**3D Printed Enclosure:**
![GardenSlave Enclosure](https://github.com/user-attachments/assets/d1894482-3f5e-43a2-b8ec-4f3f3e52a7fc)  
*GardenSlave installed in a custom 3D-printed enclosure, with openings for the soil sensor cable and pump power lead.*

![Assembled Unit in Operation](https://github.com/user-attachments/assets/39d3a17f-0748-4503-b9fb-3a0773ff0711)  
*GardenSlave powered up and connected to the reservoir float switch and soil probe.*

---

## Wiring & Pinout

| Signal           | Pico GPIO / Nano Pin | Code Name        |
| ---------------- | -------------------- | ---------------- |
| **Buzzer**       | GP27 / D11           | `buzzer`         |
| **Float Switch** | GP6 / D6             | `float_sensor`   |
| **Pump Relay**   | GP16 / D13           | `pump`           |
| **Soil Moisture**| A2 (GP26 on Pico)    | —                |


### Parts

- **Moisture Sensor Connector**  
  Connector Header Through Hole, Right-Angle, 3-position, 0.098″ (2.50 mm)  
  [JST S3B-XH-A (Digikey #1651048)](https://www.digikey.com/en/products/detail/jst-sales-america-inc/S3B-XH-A/1651048)

- **Float Switch**  
  6 Pieces Plastic PP Float Switch for Fish-Tank Liquid Level Sensing  
  [Anndason DP5200 (Amazon #B072QCHQ2P)](https://www.amazon.com/dp/B072QCHQ2P)

- **Pump Power Connector**  
  Connector Header Through Hole, Right-Angle, 2-position  
  [Molex 1724480002 (Digikey #5116918)](https://www.digikey.com/en/products/detail/molex/1724480002/5116918)

- **Buck Converter (12V to 5V)**  
  DC-DC Buck Converter Module, adjustable output (e.g. 24 V→5 V/9 V/12 V/3 V)  
  [eBoot MP1584EN (Amazon #B07RVG34WR)](https://www.amazon.com/dp/B07RVG34WR)

- **Relay**  
  5 V DC Mini Power Relay (SPDT) - Auto PCB Mount Electric Relay With 5 Pins for Universal Household Appliance  
  [Songle SRD-05VDC-SL-C (Amazon #B098JPM9S7)](https://www.amazon.com/dp/B098JPM9S7)

- **Relay Driver Transistor**  
  30pcs S9013H Transistor Plastic-Encapsulate Power TO-92 Transistor NPN 25V 500mA 625mW  
  [uxcell S9013H (Amazon #B07KPBS9VK)](https://www.amazon.com/dp/B07KPBS9VK)

- **Piezo Buzzer**  
  10pcs DC5V Black Mini Active Piezo Buzzers Electronic Buzzer Alarm Sounder Speaker Beep Tone    
  [mxuteuk HF-12095 (Amazon #B07VK1GJ9X)](https://www.amazon.com/dp/B07VK1GJ9X)


---

## Calibration Constants

```cpp
const int AirValue      = 632;  // ADC in air (dry)
const int WaterValue    = 450;  // ADC in water (wet)
const int DryValue      = 600;  // soilMoistureValue ≥ this → pump on
