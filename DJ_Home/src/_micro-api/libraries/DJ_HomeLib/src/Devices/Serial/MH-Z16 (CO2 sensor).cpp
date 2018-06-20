#include "MH-Z16 (CO2 sensor).h"
#include <sstream>

namespace home
{
	MH_Z16::MH_Z16(const std::string& address):
		Device(address)
	{
		std::cout << "MH_Z16::MH_Z16()" << std::endl;
		Serial3.begin(9600);
		mValue["CO2"] = "0";
	}
	
	//bool MH_Z16::Execute(const std::string& function, const std::string& value) const
	//{
	//	std::cout << "MH_Z16::Execute()" << std::endl;
	//	return false;
	//}

	bool MH_Z16::Loop() const
	{
	//	if (millis() - mLoopMillis < OptionGet<unsigned long>("LoopMillis"))
	//		return false;
		mLoopMillis = millis();
		std::cout << "bool MH_Z16::Loop()" << std::endl;
		byte request[9] = { 0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79 };
		byte response[9];
		Serial3.write(request, 9);
		memset(response, 0, 9);
		Serial3.readBytes(response, 9);
		byte crc = 0;
		for (int i = 1; i<8; i++) 
			crc += response[i];
		crc = 255 - crc;
		crc++;
		if (!(response[0] == 0xFF && response[1] == 0x86 && response[8] == crc))
		{
			std::cout << "\t" << "CRC error: " << (int)crc << " / " << (int)response[8] << std::endl;
			return false;
		}
		unsigned int responseHigh = (unsigned int)response[2];
		unsigned int responseLow = (unsigned int)response[3];
		std::stringstream ss;
		ss << (unsigned int)(256 * responseHigh + responseLow);
		std::cout << "\t" << "CO2: " << ss.str() << std::endl;
		//mValue["CO2"] = ss.str();
	//	//mPMqtt->Publish(mAddress + "/CO2", Get("CO2"));
	//	SetValueProtected("CO2", ss.str());
	//	std::cout << "\t" << "[CO2]=" << OptionGet<int>("CO2") << std::endl;
		return true;
	}
}