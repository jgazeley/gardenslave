// Uncomment the target platform and recompile:
// #define NANO
#define PICO

// Pin definitions for Raspberry Pi Pico
#ifdef PICO
  const int buzzer       = 27; // Piezo buzzer output pin
  const int float_sensor = 6;  // Float switch input pin (active-LOW with pullup)
  const int pump         = 16; // Water pump control pin
#endif

// Pin definitions for Arduino Nano (disabled)
// #ifdef NANO
//   const int buzzer       = 11;
//   const int float_sensor = 6;
//   const int pump         = 13;
//   const int pump2        = 18; // second pump (if needed)
// #endif

// Soil moisture calibration (raw ADC values)
const int AirValue   = 632; // reading in air (dry)
const int WaterValue = 450; // reading in water (wet)
const int DryValue = 600;   // dry threshold
int soilMoistureValue   = 0; // raw ADC reading
int soilMoistureValue2  = 0; // (unused) second sensor reading
int soilMoisturePercent = 0; // mapped percentage (0–100)

// Buzzer melodies: delays for SOS, frequencies for Zelda tune
int sos[]   = { 100,100,100, 300,300,300, 100,100,100 };
int zelda[] = { 784,740,622,440,415,659,831,1047 };

bool low_water = false; // state of the float switch

void setup() {
  pinMode(buzzer, OUTPUT);          // buzzer as output
  pinMode(float_sensor, INPUT_PULLUP); // float switch with internal pull-up
  pinMode(pump, OUTPUT);            // pump as output
  // #ifdef NANO
  //   pinMode(pump2, OUTPUT);        // optional second pump
  // #endif
}

// Play SOS: 3 short, 3 long, 3 short tones
void Sos() {
  for (int i = 0; i < 9; i++) {
    tone(buzzer, 1047);            // fixed pitch
    delay(sos[i]);                 // short or long duration
    noTone(buzzer);                // stop tone
    delay(100);                    // brief pause
    if (i == 2 || i == 5 || i == 8) {
      delay(400);                  // pause between letters
    }
  }
}

// Play Zelda melody: eight notes
void Zelda() {
  for (int i = 0; i < 8; i++) {
    tone(buzzer, zelda[i]);        // play each frequency
    delay(140);                    // fixed note length
  }
  noTone(buzzer);                  // stop buzzer at end
}

// Main control loop
void loop() {
  // Read float switch (LOW when water present)
  low_water = digitalRead(float_sensor);

  if (!low_water) {
    // If float switch indicates water, check soil moisture
    soilMoistureValue = analogRead(A2); // raw reading from moisture sensor

    if (soilMoistureValue >= DryValue) {
      // If very dry, run pump for 1 second, then wait 5 seconds
      digitalWrite(pump, HIGH);
      delay(1000);
      digitalWrite(pump, LOW);
      delay(5000);
    }

    // Optional second pump logic (commented out)
    // soilMoistureValue2 = analogRead(A1);
    // if (soilMoistureValue2 >= 600) {
    //   digitalWrite(pump2, HIGH);
    //   delay(1000);
    //   digitalWrite(pump2, LOW);
    //   delay(5000);
    // }
  }

  if (low_water) {
    // If tank is empty, repeatedly sound SOS until refilled
    while (digitalRead(float_sensor)) {
      Sos();
    }
    // Once float switch reads water again, play Zelda tune once
    Zelda();
  }
}
