/******************************************************************
  Cursus Internet Of Things 2

  Geschreven voor het ESP32 Development board
  Auteur: Patrick De Boeck
*******************************************************************/

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define PROJECT BlUETOOTH
#define COMPILE_ID STR(PROJECT) STR(/) __DATE__ STR(/) __TIME__

#include <BluetoothSerial.h>
#include <map>


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;
BTAdvertisedDevice* pDevice = NULL; // pointer naar een Bluetooth device
BTScanResults* pDeviceList = NULL; // pointer naar een lijst van ontdekte bluetooth apparaten
std::map<int, std::string> channels;  // dit is eenmap van key/values voor de beschikbare kanalen
BTAddress addr;       // bluetooth adres waarnaar we gaan verbinden
int channel = 0;      // het geselecteerde kanaal waar we naar gaan verbinden

struct RemoteDevice {
  BTAdvertisedDevice* pRemoteDevice;
  int remoteChannel;
};
RemoteDevice remoteDevice;    // maak een variabele aan
std::map<int, RemoteDevice> keuzelijst; // hier gaan we de devices opslaan met welke we kunnen verbinden
int aantalRemoteDevices = 0;
int keuze=0;

void btAdvertisedDeviceFound(BTAdvertisedDevice* pDevice) {
  Serial.print("Gevonden apparaat:");
  Serial.println(pDevice->toString().c_str());
}

void setup() {
  Serial.begin(115200);
  if (! SerialBT.begin(COMPILE_ID, true) ) {  // We zijn nu een master (client)
    Serial.println("========== serialBT mislukt!");
    abort();
  }

  Serial.println("Starting discoverAsync...");

  if (SerialBT.discoverAsync(btAdvertisedDeviceFound)) {
    delay(5000);
    Serial.print("Stoppen van discoverAsync... ");
    SerialBT.discoverAsyncStop();
    Serial.println("gestopt");
    Serial.println();
    pDeviceList = SerialBT.getScanResults();  //pointer naar de resultaten van de scan
    if (pDeviceList->getCount() > 0) {

      Serial.println("Zoeken naar beschikbare kanalen:");
      for (int i = 0; i < pDeviceList->getCount(); i++) {
        pDevice = pDeviceList->getDevice(i);
        Serial.println(pDevice->toString().c_str());
        channels = SerialBT.getChannels(pDevice->getAddress());
        Serial.println(String(channels.size()) + " kana(a)l(en) gevonden");
        for (auto it = channels.begin(); it != channels.end(); it++) { // bekijk de hele set
          Serial.print(it->first);      // dit is de key
          Serial.println(" - " + String(it->second.c_str())); // dit is de value
        }

        if (channels.size() > 0) {
          aantalRemoteDevices++;    // we hebben een kandidaat gevonden
          remoteDevice.pRemoteDevice = pDevice;
          remoteDevice.remoteChannel = channels.begin()->first; // selecteer het eerste vrije kanaal
          keuzelijst[aantalRemoteDevices] = remoteDevice;
        }

      }

      if (aantalRemoteDevices) {    // er zijn dus geschikte bluetooth devices
        Serial.println("Maak een keuze:");
        for (auto it = keuzelijst.begin(); it != keuzelijst.end(); it++) {
          Serial.print(it->first);
          Serial.print("  ->  ");
          Serial.println(it->second.pRemoteDevice->toString().c_str());
        }
        while (keuze == 0) {    // wacht tot er een keuze gemaakt is
          if (Serial.available()) { // kijk of er iets ingetikt wordt
            keuze = Serial.parseInt();  // kijk wat er ingetikt is
            if ((keuze > 0) and (keuze <= aantalRemoteDevices)) { // geldige invoer?
              addr = keuzelijst[keuze].pRemoteDevice->getAddress();
              channel = keuzelijst[keuze].remoteChannel;
              Serial.print("Verbinden met adres:");
              Serial.print(addr.toString().c_str());
              Serial.print("  --- kanaal=");
              Serial.println(keuzelijst[keuze].remoteChannel);
              if (SerialBT.connect(addr, channel, ESP_SPP_SEC_NONE, ESP_SPP_ROLE_MASTER)) {
                Serial.println("Verbinding Gelukt");
              }
            }
            else {
              Serial.println("Ongeldige keuze, probeer opnieuw");
              keuze = 0;  // ongeldige keuze, wacht dus op een nieuwe keuze
            }
          }
        }
      }
    } else {
      Serial.println("Geen Bluetooth devices gevonden");
    }
  } else {
    Serial.println("Fout bij autodiscovery");
  }
}

String sendData = "Ik ben " + String(COMPILE_ID);

void loop() {
  if (SerialBT.connected()) {
    Serial.println("tx: " + sendData);
    SerialBT.println(sendData);
    if (SerialBT.available()) {
      Serial.print("rx: ");
      while (SerialBT.available()) {
        Serial.print((char)SerialBT.read());
      }
      Serial.println();
    }
  }
  else {
    Serial.println("not connected");
  }
  delay(1000);
}
