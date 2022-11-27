/******************************************************************
 Cursus Internet Of Things
 Aansturing van de DHT11
 Geschreven voor het ESP32 development board
 Developer: Patrick
*******************************************************************/
// bibliotheken, globale variabelen en constanten worden hier gedeclareerd
// Zorg dat volgende Arduino libraries geladen zijn:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor
#include <DHT.h>
const int dhtPin = 4;  // GPIO pin voor de DHT sensor
float h, t;
DHT dht(dhtPin, DHT11);  // DHT11 is een constante gedefiniëerd in de bibiotheek.
                         // Kan ook DHT22 of DHT21 zijn
void setup() {
  Serial.begin(115200);
  Serial.println("ESP Klaar..");
  dht.begin();
}
void loop() {
  // Wacht 3 seconden tussen 2 metingen (de sensor is niet echt snel)
  delay(3000);
  h = dht.readHumidity();     // lees de vochtigheid in
  t = dht.readTemperature();  // lees temperatuur in Celsius (default)
  Serial.print("Vochtigheid: ");
  Serial.print(h);
  Serial.print("% Temperatuur: ");
  Serial.print(t);
  Serial.print("°C ");
  Serial.println();
}