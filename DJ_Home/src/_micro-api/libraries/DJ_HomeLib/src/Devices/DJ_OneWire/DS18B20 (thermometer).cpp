#include "DS18B20 (thermometer).h"
#include <sstream>

namespace home
{
	DS18B20::DS18B20(const OneWireAddress& address) :
		OneWireDevice(address)
	{
		mValue["temperature"] = "";
	};

	//bool DS18B20::Execute(const std::string& function, const std::string& value) const
	//{
	//	std::cout << "DS18B20::Execute()" << std::endl;
	//	return false;
	//}

	bool DS18B20::Loop() const
	{
		//std::cout << "DS18B20::Loop()" << std::endl;
		//std::cout << "\t" << mAddress.StrDec() << std::endl;
		// Сброс шины
		if (DJOneWire::mOneWire.reset())
		{
			// Выбор устройства
			DJOneWire::mOneWire.select(mAddress.Array());
			// Команда датчику начать конвертирования температуру, результат запоминается в регистре датчика
			DJOneWire::mOneWire.write(0x44, 1);
			delay(1000);
			// Сброс шины
			DJOneWire::mOneWire.reset();
			// Выбор устройства
			DJOneWire::mOneWire.select(mAddress.Array());
			// Команда датчику начать передавать значение температуры из регистра
			DJOneWire::mOneWire.write(0xBE);
			// Читаем (запоминаем) 9 байт (8 байт температура + 1 байт контрольный код)
			byte data[9];
			DJOneWire::mOneWire.read_bytes(data, 9);
			// Если никакой информации не пришло (не пришло нулей) - значит устройство не подключено
			if (!~(*(uint64_t*)data) && !(uint8_t)~data[8])
				return;
			// Проверяем контрольную сумму и если совпала - запоминаем значение температуры
			if (data[8] == OneWire::crc8(data, 8))
			{
				std::stringstream ss;
				ss << (float)((int)data[0] | (((int)data[1]) << 8)) * 0.0625 + 0.03125;
				SetValueProtected("temperature", ss.str());
				//std::cout << "\t" << ss.str() << std::endl;
			}
		}
	}
}