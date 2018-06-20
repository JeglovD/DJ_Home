#include "DJ_HomeLib.h"

namespace std
{
	ohserialstream cout(Serial);
}

void setup()
{
	Serial.begin(115200);
}


void loop()
{
	home::Devices::GetInstance().Loop();
	//delay(5000);
}
