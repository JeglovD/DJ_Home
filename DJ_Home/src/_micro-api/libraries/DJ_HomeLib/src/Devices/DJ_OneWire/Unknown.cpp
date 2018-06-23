#include "Unknown.h"

namespace home
{
	Unknown::Unknown(const OneWireAddress& address) :
		OneWireDevice(address)
	{
		std::cout << "Unknown::Unknown()" << std::endl;
	};

	bool Unknown::Loop() const
	{
		if (millis() - mLoopMillis < Get<unsigned long>("LoopMillis"))
			return false;
		mLoopMillis = millis();
		std::cout << "bool Unknown::Loop()" << std::endl;
	}
}