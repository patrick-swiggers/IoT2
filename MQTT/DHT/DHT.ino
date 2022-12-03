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

#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

char accesspoint[] = "accesspoint";  // iot1/cursusiot , WifiVolt/Volt3001
char wachtwoord[] = "password";

char mqttServer[] = "test.mosquitto.org";
unsigned int mqttPort = 1883;            //Dit is de default MQTT poort
char mqqtClientID[] = "LPNIOT2-DHT";     //Pas dit aan voor jou
char mqqtTopic[] = "cvo/iot2/ps/tempc";  //Pas dit aan voor jou

const int dhtPin = 4;  // GPIO pin voor de DHT sensor
float h, t;
DHT dht(dhtPin, DHT11);  // DHT11 is een constante gedefiniëerd in de bibiotheek.
                         // Kan ook DHT22 of DHT21 zijn

WiFiClient mijnTCPClient;
PubSubClient mijnMQTT;

void setup() {
  Serial.begin(115200);
  makeWiFiConnectie(accesspoint, wachtwoord);
  mijnMQTT.setClient(mijnTCPClient);         //associeer een onderliggende TCP laag
  mijnMQTT.setServer(mqttServer, mqttPort);  //Zet de Broker info
  Serial.println("[INFO] Verbinden met MQTT Broker...");
  if (mijnMQTT.connect(mqqtClientID)) {
    Serial.println("[INFO] Verbinding met MQTT Broker gelukt.");
  } else {
    Serial.print("[FOUT] Verbinding met MQTT Broker mislukt. Foutcode ");
    Serial.println(mijnMQTT.state());
  }
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

  String tempString = String(dht.readTemperature());
  if (mijnMQTT.publish(mqqtTopic,tempString.c_str())){
    Serial.println("[INFO] MQTT Publish bericht verzonden");
  } else {
    Serial.print("[FOUT] MQTT Publish bericht verzenden mislukt Foutcode ");
    Serial.println(mijnMQTT.state());
  }
}


void makeWiFiConnectie(char accesspoint[], char wachtwoord[]) {
  WiFi.begin(accesspoint, wachtwoord);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)  //probeer zolang het niet lukt
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Verbonden, IP adres: ");
  Serial.println(WiFi.localIP());
}