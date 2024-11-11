#include <Wire.h>
#include <math.h>

// I2C-Adressen der Sensoren
const int MPU_ADDR = 0x68;  // MPU-9250 (Inertial Measurement Unit)
const int MAG_ADDR = 0x0C;  // AK8963 Magnetometer

// Variablen für die Kalibrierung
int magMin[2] = {32767, 32767};  // Minimale Messwerte für X und Y
int magMax[2] = {-32768, -32768}; // Maximale Messwerte für X und Y
int magCenter[2];              // Mittelwerte für X und Y (Kalibrierungsoffset)

// Variablen für die Kalibrierungsintervalle
unsigned long lastCalibrationTime = 0;
const unsigned long calibrationInterval = 5000; // Neukalibrierung alle 5 Sekunden

// Filterkonstante für den Tiefpassfilter
float alpha = 0.1;  // Je kleiner, desto stärker gefiltert

// Variablen für die gefilterten Werte
float filteredX, filteredY;

void setup() {
  // Initialisiere die I2C-Schnittstelle
  Wire.begin();
  Serial.begin(9600);

  // Konfiguriere das MPU-9250 und das AK8963 Magnetometer
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);  // PWR_MGMT_1 Register
  Wire.write(0);    // MPU-9250 aktivieren
  Wire.endTransmission();

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x37);  // INT_PIN_CFG Register
  Wire.write(0x02);  // Bypass-Modus aktivieren
  Wire.endTransmission();
  delay(10);

  Wire.beginTransmission(MAG_ADDR);
  Wire.write(0x0A);  // CNTL1 Register
  Wire.write(0x16);  // Modus 2, 16-bit Auflösung
  Wire.endTransmission();
  delay(10);

  // Initialisiere die gefilterten Werte
  filteredX = 0;
  filteredY = 0;
}

void loop() {
  // Lese die Rohdaten vom Magnetometer
  Wire.beginTransmission(MAG_ADDR);
  Wire.write(0x03);  // Startadresse der Magnetometer-Daten
  Wire.endTransmission(false);
  Wire.requestFrom(MAG_ADDR, 7);  // Lese 7 Bytes (6 Daten + 1 Status)

  if (Wire.available() == 7) {
    Wire.read();  // Lese und verwerfe das Status-Byte
    int16_t magX = Wire.read() | Wire.read() << 8;
    int16_t magY = Wire.read() | Wire.read() << 8;

    // Kalibrierung regelmäßig durchführen
    if (millis() - lastCalibrationTime > calibrationInterval) {
      if (magX < magMin[0]) magMin[0] = magX;
      if (magX > magMax[0]) magMax[0] = magX;
      if (magY < magMin[1]) magMin[1] = magY;
      if (magY > magMax[1]) magMax[1] = magY;
      magCenter[0] = (magMin[0] + magMax[0]) / 2;
      magCenter[1] = (magMin[1] + magMax[1]) / 2;
      lastCalibrationTime = millis();
    }

    // Kalibriere die Rohdaten und wende den Tiefpassfilter an
    float calibratedX = (float)(magX - magCenter[0]);
    float calibratedY = (float)(magY - magCenter[1]);
    filteredX = alpha * calibratedX + (1 - alpha) * filteredX;
    filteredY = alpha * calibratedY + (1 - alpha) * filteredY;

    // Berechne die Kompassrichtung in Grad
    float heading = atan2(calibratedY, calibratedX) * 180 / PI;

    // Gib die berechnete Kompassrichtung aus
    Serial.print("Kompassrichtung: ");
    Serial.print(heading);
    Serial.println(" Grad");
  }

  delay(100);  // Wartezeit zwischen den Messungen
}