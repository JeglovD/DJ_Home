#include "DJ_OneWire.h"
#include "DS18B20 (thermometer).h"
#include "DS2413 (switch).h"
#include <sstream>
#include <typeinfo>
#include <bitset>

namespace home
{
	Devices* DJOneWire::mpDevices(NULL);
	::OneWire DJOneWire::mOneWire(2);

	const std::string OneWireAddress::StrDec() const
	{
		//std::cout << "OneWireAddress::StrDec()" << std::endl;
		std::ostringstream result;
		for (int it = 0; it <= 7; ++it)
			result << std::dec << std::showbase << (int)mArray[it] << (it < 7 ? "_" : "");
		//std::cout << "\t" << result.str() << std::endl;
		return result.str();
	}

	const std::string OneWireAddress::StrHex() const
	{
		//std::cout << "OneWireAddress::StrHex()" << std::endl;
		std::ostringstream result;
		for (int it = 0; it <= 7; ++it)
			result << std::hex << std::showbase << (int)mArray[it] << (it < 7 ? "_" : "");
		//std::cout << "\t" << result.str() << std::endl;
		return result.str();
	}

	const std::string OneWireAddress::StrBit() const
	{
		std::ostringstream result;
		for (int it = 0; it <= 7; ++it)
			result << std::bitset<sizeof(mArray[it]) * 8>(mArray[it]) << (it < 7 ? "_" : "");
		return result.str();
	}

	OneWireDevice::OneWireDevice(const OneWireAddress& address) :
		Device(address.StrDec()),
		mAddress(address),
		mFound(true)
	{
		//std::cout << "OneWireDevice()" << std::endl;
	};

	const bool DJOneWire::Loop()
	{
		//std::cout << "DJOneWire::Loop()" << std::endl;
		// Ищем новые OneWire устройства
		OneWireAddress address;
		if (mOneWire.search(address.Array()))
		{
			if (::OneWire::crc8(address.Array(), 7) == address.Array()[7])
			{
				//std::cout << "\t" << address.StrDec() << std::endl;
				auto pDevice = mpDevices->find(address.StrDec());
				if (pDevice == mpDevices->end())
				{
					//std::cout << "\t\t" << "new" << std::endl;
					(*mpDevices)[address.StrDec()] = POneWireDeviceNew(address);
				}
				else
					if (pDevice->second->Type().find("OneWireDevice") != std::string::npos)
						((OneWireDevice*)(pDevice->second))->SetFound(true);
			}
		}
		else
		{
			// Удаляем OneWire устройства, не найденные в текущем поиске
			for (auto pIt = mpDevices->begin(); pIt != mpDevices->end(); ++pIt)
			{
				if (pIt->second->Type().find("OneWireDevice") != std::string::npos)
				{
					if (((OneWireDevice*)(pIt->second))->Found())
						((OneWireDevice*)(pIt->second))->SetFound(false);
					else
					{
						delete pIt->second;
						mpDevices->erase(pIt);
						continue;
					}
				}
			}
			// Инициализируем новый поиск
			mOneWire.reset_search();
		}
	}

	const Device* DJOneWire::POneWireDeviceNew(const OneWireAddress& address)
	{
		//std::cout << "DJOneWire::POneWireDeviceNew()" << std::endl;
		//std::cout << "\t" << address.StrDec() << std::endl;
		if (address.Array()[0] == 40)
		{
			//std::cout << "\t" << "DS18B20 (thermometer)" << std::endl;
			return new DS18B20(address);
		}
		if (address.Array()[0] == 133)
		{
			//std::cout << "\t" << "DS2413 (switch)" << std::endl;
			return new DS2413(address);
		}
		//std::cout << "\t" << "Unknown" << std::endl;
	}
}