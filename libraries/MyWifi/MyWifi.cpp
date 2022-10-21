#include <MyWifi.h>

MyWifi::MyWifi(char accesspoint[], char password[])
{
	Serial.println("** MyWifi library **");
	WiFi.begin(accesspoint, password);
	Serial.print("Connecting ");
	Serial.println(accesspoint);
	while (WiFi.status() != WL_CONNECTED)  //probeer zolang het niet lukt
	{
		delay(500);
		Serial.print(".");
	}
	Serial.println();

	Serial.print("Verbonden, IP adres: ");
	Serial.println(WiFi.localIP());
}

IPAddress MyWifi::myIP(){
	return(WiFi.localIP());
}