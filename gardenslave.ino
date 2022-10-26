// #define NANO
#define PICO

//Raspberry Pi Pico
#ifdef PICO
  const int buzzer = 27;
  const int float_sensor = 6;
  const int pump = 16;
#endif

//Arduino Nano
// #ifdef NANO
//   const int buzzer = 11;
//   const int float_sensor = 6;
//   const int pump = 13;
//   const int pump2 = 18;
// #endif

//Soil Moisture Sensor
const int AirValue = 632;
const int WaterValue = 450;
int soilMoistureValue = 0;
int soilMoistureValue2 = 0;
int soilMoisturePercent = 0;

//Piezo Buzzer
int sos[] = { 100, 100, 100, 300, 300, 300, 100, 100, 100 }; // delay
int zelda[] = { 784, 740, 622, 440, 415, 659, 831, 1047 }; // frequency

bool low_water = false;

void setup() {
  pinMode(buzzer, OUTPUT);
  pinMode(float_sensor, INPUT_PULLUP);
  pinMode(pump, OUTPUT);
  // #ifdef NANO
  //   pinMode(pump2, OUTPUT);
  // #endif
}

void Sos() {

  for(int i = 0; i < 9; i++) {
    tone(buzzer, 1047);
    delay(sos[i]);
    noTone(buzzer);
    digitalWrite(buzzer, LOW);
    delay(100);
    if(i == 2 || i == 5 || i == 8) {
      delay(400);
    }
  }
}

void Zelda() {

  for(int i = 0; i < 8; i++) {
    tone(buzzer, zelda[i]);
    delay(140);
  }

  noTone(buzzer);
  digitalWrite(buzzer, LOW);
}

// Main loop
void loop() {
//  soilMoisturePercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);

  low_water = digitalRead(float_sensor);
  
  if(!low_water) {
    soilMoistureValue = analogRead(A2);
    
    if(soilMoistureValue >= 600) {
      digitalWrite(pump, HIGH);
      delay(1000);
      digitalWrite(pump, LOW);
      delay(5000);
    }

//    soilMoistureValue2 = analogRead(A1);

//    if(soilMoistureValue2 >= 600) {
//      digitalWrite(pump2, HIGH);
//      delay(1000);
//      digitalWrite(pump2, LOW);
//      delay(5000);
//    }
  }

  if(low_water) {
    low_water = digitalRead(float_sensor);
    while(low_water) {
      Sos();
      low_water = digitalRead(float_sensor);
    }
    
    Zelda();
  }
}
