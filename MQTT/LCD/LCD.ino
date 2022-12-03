/******************************************************************
  Cursus Internet Of Things II
  I2C LCD: DHT
  Geschreven voor het ESP8266 development board
  Developer: Patrick S.
*******************************************************************/
//bibliotheken, globale variabelen en constanten worden hier gedeclareerd
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <StreamString.h>

char mqttServer[] = "test.mosquitto.org";
unsigned int mqttPort = 1883; //Dit is de default MQTT poort
char mqttClientID[] = "LPNIOT2-LCD";
char mqttTopic[] = "cvo/iot2/ps/tempc";
String RXBericht; // Het ontvangen MQTT bericht
WiFiClient mijnTCPClient;
PubSubClient mijnMQTT;
StreamString mqttStream;

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Constructor

void setup() {
  // de setup functie wordt een enkele keer uitgevoerd
  Serial.begin(115200);
  lcd.init();  // initializeer LCD
  lcd.backlight();

  makeWiFiConnectie("accespoint", "password"); //iot1/cursusiot , WifiVolt/Volt3001
  mijnMQTT.setClient(mijnTCPClient);  // Associeer een onderliggende TCP laag
  mijnMQTT.setServer(mqttServer, mqttPort);  // Zet de Broker info
  mijnMQTT.setStream(mqttStream);  // Zet de Stream waar de data ontvangen worden
  makeMQTTConnection(mqttTopic);
  
  RXBericht = "00";
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[FOUT] Verbinding met Wifi verbroken");
    makeWiFiConnectie("accespoint", "password");
  }
  if (!mijnMQTT.loop()) {  //onderhoud de verbinding en kijk voor binnenkomende berichten
    Serial.print("[FOUT] Verbinding met MQTT Broker verbroken. Foutcode ");
    Serial.println(mijnMQTT.state());
    makeMQTTConnection(mqttTopic);  //probeer opnieuw te verbinden
  }
  if (mqttStream.available()) {
    Serial.println("[INFO] MQTT Bericht ontvangen");
    RXBericht = mqttStream.readString();
    Serial.println(RXBericht);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TEMP : ");
  lcd.print(RXBericht);
  lcd.print(" " + (String)char(0xDF) + "C");

  delay(2000);
}

void makeMQTTConnection(char* topic) {
  Serial.println("[INFO] Verbinden met MQTT Broker...");
  if (mijnMQTT.connect(mqttClientID)) {
    Serial.println("[INFO] Verbinding met MQTT Broker gelukt.");
    if (strlen(topic) > 0) {
      if (mijnMQTT.subscribe(topic)) {
        Serial.print("[INFO] MQTT Subscribe uitgevoerd: ");
        Serial.println(topic);
      }
      else {
        Serial.print("[FOUT] MQTT Subscribe mislukt, Foutcode ");
        Serial.println(mijnMQTT.state());
        delay(1000);  // Als er iets fout gaat, wacht dan even
      }
    }
  } else {
    Serial.print("[FOUT] Verbinding met MQTT Broker mislukt. Foutcode ");
    Serial.println(mijnMQTT.state());
    delay(1000);  // Als er iets fout gaat, wacht dan even
  }
}

void makeWiFiConnectie(String accesspoint, String wachtwoord) {
  const byte ledWiFi = 2;  //GPIO2
  pinMode(ledWiFi, OUTPUT);
  digitalWrite(ledWiFi, LOW);
  Serial.println();
  WiFi.mode(WIFI_STA);  // De ESP is een client
  WiFi.begin(accesspoint, wachtwoord);
  Serial.print("[INFO] WiFi Verbinding maken");
  while (WiFi.status() != WL_CONNECTED)  //probeer opnieuw zolang het niet lukt
  {
    delay(500);
    Serial.print(".");
    digitalWrite(ledWiFi, not(digitalRead(ledWiFi)));
  }
  Serial.println();
  digitalWrite(ledWiFi, HIGH);
  Serial.print("[INFO] Wifi Verbonden, IP adres: ");
  Serial.println(WiFi.localIP());
}
