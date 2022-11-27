#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
String commando;
int ledPin=2;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,LOW);
  SerialBT.begin("ESP32PS"); //Bluetooth device name
  Serial.println("BT device is ready");
}

void loop() {
  if (SerialBT.available()) {
    commando=(SerialBT.readStringUntil('\r'));
    Serial.println(commando);
    if (commando=="aan"){
      digitalWrite(ledPin,HIGH);
      SerialBT.println("Led werd aangezet");
    } else if (commando=="uit"){
      digitalWrite(ledPin,LOW);
      SerialBT.println("Led werd uitgezet");
    }
  }
}
