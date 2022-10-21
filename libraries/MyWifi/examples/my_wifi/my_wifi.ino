/******************************************************************
  Cursus Internet Of Things 2
  Een basis Wifi verbinding
  Geschreven voor het ESP32 Development board
  Auteur: Patrick Swiggers
*******************************************************************/
//bibliotheken, globale variabelen en constanten worden hier gedeclareerd
#include <MyWifi.h>
char accesspoint[] = "iot1"; //"iot1"; //vul hier jouw WiFi credentials in
char password[] = "cursusiot"; //"cursusiot";

void setup() {
  //  de setup functie wordt een enkele keer uitgevoerd
  {
    Serial.begin(115200);
    Serial.println();
    MyWifi  wifi(accesspoint, password);
    Serial.println(wifi.myIP());        
  }
}

void loop() {
  // the loop functie herhaalt zich in een oneindige lus
}
