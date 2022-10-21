/******************************************************************
  Cursus Internet Of Things 2
  Een basis Wifi verbinding
  Geschreven voor het ESP32 Development board
  Auteur: Patrick Swiggers
*******************************************************************/

#ifndef MyWifi_h
#define MyWifi_h

#include <Arduino.h>
#include <WiFi.h>

class MyWifi {
public:
	MyWifi(char accesspoint[], char password[]);
	IPAddress myIP();

};

#endif