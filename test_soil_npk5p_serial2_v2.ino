#include <Wire.h>

// RE and DE Pins set the RS485 module to Receiver or Transmitter mode
#define RE 5  // Sesuai dengan GPIO 5 yang kamu tentukan untuk RE (Receiver Enable)
#define DE 4  // Sesuai dengan GPIO 4 yang kamu tentukan untuk DE (Driver Enable)

// Modbus RTU requests for reading NPK and other values
const byte humidityQuery[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};   // Humidity
const byte temperatureQuery[] = {0x01, 0x03, 0x00, 0x01, 0x00, 0x01, 0xD5, 0xCA}; // Temperature
const byte phQuery[] = {0x01, 0x03, 0x00, 0x02, 0x00, 0x01, 0x25, 0xCA};          // Soil pH
const byte conductivityQuery[] = {0x01, 0x03, 0x00, 0x03, 0x00, 0x01, 0x74, 0x0A}; // Conductivity
const byte nitroQuery[] = {0x01, 0x03, 0x00, 0x1E, 0x00, 0x01, 0xE4, 0x0C};       // Nitrogen
const byte phosQuery[] = {0x01, 0x03, 0x00, 0x1F, 0x00, 0x01, 0xB5, 0xCC};        // Phosphorous
const byte potaQuery[] = {0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xC0};        // Potassium

// A variable used to store NPK and other values
byte values[11];

void setup() {
  Serial.begin(9600);   // Serial monitor
  Serial2.begin(9600, SERIAL_8N1, 16, 17);  // Serial2 using GPIO16 for RX and GPIO17 for TX on ESP32

  // Define pin modes for RE and DE
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
  
  delay(500);
}
 
void loop() {
  // Read values for all parameters
  float humidity = readSensor(humidityQuery);
  delay(250);
  float temperature = readSensor(temperatureQuery);
  delay(250);
  float soilPH = readSensor(phQuery);
  delay(250);
  float conductivity = readSensor(conductivityQuery);
  delay(250);
  float nitrogen = readSensor(nitroQuery);
  delay(250);
  float phosphorous = readSensor(phosQuery);
  delay(250);
  float potassium = readSensor(potaQuery);
  delay(250);

  // Print values to the serial monitor
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Soil pH: ");
  Serial.print(soilPH);
  Serial.println();

  Serial.print("Conductivity: ");
  Serial.print(conductivity);
  Serial.println(" µS/cm");

  Serial.print("Nitrogen: ");
  Serial.print(nitrogen);
  Serial.println(" mg/kg");

  Serial.print("Phosphorous: ");
  Serial.print(phosphorous);
  Serial.println(" mg/kg");

  Serial.print("Potassium: ");
  Serial.print(potassium);
  Serial.println(" mg/kg");

  delay(2000);
}

// Function to read sensor data
float readSensor(const byte query[]) {
  digitalWrite(DE, HIGH); // Set to transmit mode
  digitalWrite(RE, HIGH);
  delay(10);
  
  if(Serial2.write(query, sizeof(query)) == 8) {
    digitalWrite(DE, LOW);  // Set to receive mode
    digitalWrite(RE, LOW);
    
    for (byte i = 0; i < 7; i++) {
      values[i] = Serial2.read();
    }

    // Combine the two bytes into a single value (high byte and low byte)
    int result = (values[3] << 8) | values[4];
    return result / 10.0; // Convert to appropriate scale (e.g., temperature, pH, etc.)
  }
  return 0;
}
