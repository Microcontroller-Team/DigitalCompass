#include <Wire.h>

const int MPU_ADDR = 0x68;    // I2C-Adresse des MPU-9250
const int MAG_ADDR = 0x0C;    // I2C-Adresse des AK8963 Magnetometers

void setup() {
  //SetupDelay Total: 20 ms 
  Wire.begin();
  Serial.begin(9600);
  Serial.println("MPU-9250 und Magnetometer AK8963 Test");

  // MPU-9250 aktivieren
  Serial.println("Aktiviere MPU-9250...");
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);           // PWR_MGMT_1 Register
  Wire.write(0);              // MPU-9250 aktivieren
  Wire.endTransmission();

  // Bypass-Modus für Magnetometer aktivieren
  Serial.println("Aktiviere Bypass-Modus...");
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x37);           // INT_PIN_CFG Register
  Wire.write(0x02);           // Bypass-Modus aktivieren
  Wire.endTransmission();
  delay(10);  // Wartezeit für Bypass-Modus

  // AK8963 in kontinuierlichen Messmodus 2 versetzen (16-bit Auflösung, 100 Hz)
  Serial.println("Konfiguriere Magnetometer AK8963...");
  Wire.beginTransmission(MAG_ADDR);
  Wire.write(0x0A);           // CNTL1 Register
  Wire.write(0x16);           // Modus 2, 16-bit Auflösung
  Wire.endTransmission();
  delay(10);  // Wartezeit für AK8963 Konfiguration
}

void loop() {
  //LoopDelay Total: 500 ms pro Schleifendurchgang
  int16_t magX, magY, magZ;

  // Magnetometer Rohdaten lesen (6 Byte)
  Wire.beginTransmission(MAG_ADDR);
  Wire.write(0x03);           // Startadresse der Magnetometer-Daten
  Wire.endTransmission(false);
  Wire.requestFrom(MAG_ADDR, 7);  // Lese 7 Bytes (6 Daten + 1 Status)

  if (Wire.available() == 7) {
    uint8_t status = Wire.read(); // Status-Byte (muss 0x01 sein für gültige Daten)
    Serial.print("Status: "); Serial.println(status, HEX);
    if (status & 0x01) {          // Überprüfen, ob neue Daten verfügbar sind
      magX = Wire.read() | Wire.read() << 8;
      magY = Wire.read() | Wire.read() << 8;
      magZ = Wire.read() | Wire.read() << 8;
      
      // Ausgabe der Magnetometer-Daten
      Serial.print("Mag X: "); Serial.print(magX);
      Serial.print(" | Mag Y: "); Serial.print(magY);
      Serial.print(" | Mag Z: "); Serial.println(magZ);
      
    } else {
      Serial.println("Keine neuen Daten vom Magnetometer.");
    }
  } else {
    Serial.println("Fehler: Weniger als 7 Bytes empfangen.");
    Serial.println(Wire.available());
  }

  delay(500); // Wartezeit vor der nächsten Messung
}
