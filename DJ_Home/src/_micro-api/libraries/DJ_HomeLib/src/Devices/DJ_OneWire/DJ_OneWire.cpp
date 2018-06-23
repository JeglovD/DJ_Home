#include "DJ_OneWire.h"
#include "DS18B20 (thermometer).h"
#include "DS2413 (switch).h"
#include "Unknown.h"
#include <sstream>
#include <typeinfo>
#include <bitset>

namespace home
{
	////Devices* DJOneWire::mpDevices(NULL);
	////::OneWire DJOneWire::mOneWire(2);

	OneWireAddress::OneWireAddress() :
		mArray()
	{};

	const std::string OneWireAddress::StrDec() const
	{
		//std::cout << "OneWireAddress::StrDec()" << std::endl;
		std::ostringstream result;
		for (int it = 0; it <= 7; ++it)
			result << std::dec << std::showbase << (int)mArray[it] << (it < 7 ? ":" : "");
		//std::cout << "\t" << result.str() << std::endl;
		return result.str();
	}

	const std::string OneWireAddress::StrHex() const
	{
		//std::cout << "OneWireAddress::StrHex()" << std::endl;
		std::ostringstream result;
		for (int it = 0; it <= 7; ++it)
			result << std::hex << std::showbase << (int)mArray[it] << (it < 7 ? ":" : "");
		//std::cout << "\t" << result.str() << std::endl;
		return result.str();
	}

	const std::string OneWireAddress::StrBit() const
	{
		std::ostringstream result;
		for (int it = 0; it <= 7; ++it)
			result << std::bitset<sizeof(mArray[it]) * 8>(mArray[it]) << (it < 7 ? ":" : "");
		return result.str();
	}

	OneWireDevice::OneWireDevice(const OneWireAddress& oneWireAddress) :
		Device(oneWireAddress.StrDec()),
		mOneWireAddress(oneWireAddress),
		mFound(true)
	{
		//std::cout << "OneWireDevice::OneWireDevice()" << std::endl;
	};

	DJOneWire::DJOneWire():
		mOneWire(2)
	{}

	DJOneWire& DJOneWire::GetInstance()
	{
		static DJOneWire djOneWire;
		return djOneWire;
	}

	bool DJOneWire::Loop()
	{
		//std::cout << "DJOneWire::Loop()" << std::endl;
		// Ищем новые OneWire устройства
		OneWireAddress address;
		if (mOneWire.search(address.Array()))
		{
			if (::OneWire::crc8(address.Array(), 7) == address.Array()[7])
			{
				//std::cout << "\t" << address.StrDec() << std::endl;
				Devices& devices = Devices::GetInstance();
				auto pIt = devices.find(address.StrDec());
				if (pIt == devices.end())
					Insert(address);
				else
					if ((*pIt)->Type().find("OneWireDevice") != std::string::npos)
						((OneWireDevice*)(*pIt))->SetFound(true);
			}
		}
		else
		{
			// Удаляем OneWire устройства, не найденные в текущем поиске
			Devices& devices = Devices::GetInstance();
			for (auto pIt = devices.begin(); pIt != devices.end(); ++pIt)
				if ((*pIt)->Type().find("OneWireDevice") != std::string::npos)
				{
					if (((OneWireDevice*)(*pIt))->Found())
						((OneWireDevice*)(*pIt))->SetFound(false);
					else
					{
						delete *pIt;
						devices.erase(pIt);
						continue;
					}
				}

			// Инициализируем новый поиск
			mOneWire.reset_search();
		}
	}

	////const Device* DJOneWire::POneWireDeviceNew(const OneWireAddress& address)
	////{
	////	//std::cout << "DJOneWire::POneWireDeviceNew()" << std::endl;
	////	//std::cout << "\t" << address.StrDec() << std::endl;
	////	if (address.Array()[0] == 40)
	////	{
	////		//std::cout << "\t" << "DS18B20 (thermometer)" << std::endl;
	////		return new DS18B20(address);
	////	}
	////	if (address.Array()[0] == 133)
	////	{
	////		//std::cout << "\t" << "DS2413 (switch)" << std::endl;
	////		return new DS2413(address);
	////	}
	////	//std::cout << "\t" << "Unknown" << std::endl;
	////}
	//OneWireDevice&& DJOneWire::OneWireDeviceNew(const OneWireAddress& address) const
	std::pair<Devices::iterator, bool> DJOneWire::Insert(const OneWireAddress& address) const
	{
		std::cout << "DJOneWire::OneWireDeviceNew()" << std::endl;
		std::cout << "\t" << address.StrDec() << std::endl;
		if (address.Array()[0] == 40)
		{
			std::cout << "\t" << "DS18B20 (thermometer)" << std::endl;
			return Devices::GetInstance().insert(DS18B20(address));
		}
		if (address.Array()[0] == 133)
		{
			std::cout << "\t" << "DS2413 (switch)" << std::endl;
			return Devices::GetInstance().insert(DS2413(address));
		}
		std::cout << "\t" << "Unknown" << std::endl;
		return Devices::GetInstance().insert(Unknown(address));
	}
}