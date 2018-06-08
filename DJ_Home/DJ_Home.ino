#include "DJ_HomeLib.h"

namespace std
{
	ohserialstream cout(Serial);
}

//home::Devices* pDevices;
//home::Devices devices;

void setup()
{
	Serial.begin(115200);
	//pDevices = new home::Devices();
	//Devices::Init();
}


void loop()
{
	//pDevices->Loop();
	home::Devices::GetInstance().Loop();
}
