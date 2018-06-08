#include "DS2413 (switch).h"

namespace home
{
	DS2413::DS2413(const OneWireAddress& address) :
		OneWireDevice(address)
	{
		mValue["SwitchA"] = "";
		mValue["SwitchB"] = "";
	};

	//bool DS2413::Execute(const std::string& function, const std::string& value) const
	//{
	//	std::cout << "DS2413::Execute()" << std::endl;
	//	return false;
	//}

	bool DS2413::DataRead::Check() const
	{
		for (size_t i = 0; i <= 3; ++i)
			if (test(i) == test(i + 4))
				return false;
		return true;
	}
	
	bool DS2413::Loop() const
	{
		if (millis() - mLoopMillis < OptionGet<unsigned long>("LoopMillis"))
			return false;
		mLoopMillis = millis();
		std::cout << "bool DS2413::Loop()" << std::endl;
		//SetProtected("SwitchA", "1");
		// Сброс шины
		if (!DJOneWire::mOneWire.reset())
			return false;
		// Выбор устройства
		DJOneWire::mOneWire.select(mAddress.Array());
		// PIO Доступ на чтение
		DJOneWire::mOneWire.write(0xF5, 1);
		// Чтение
		DS2413::DataRead dataRead(DJOneWire::mOneWire.read());
		// Проверка целостности
		if (!dataRead.Check())
			return false;
		SetValueProtected("SwitchA", dataRead.test(0) ? "0" : "1");
		SetValueProtected("SwitchB", dataRead.test(2) ? "0" : "1");
		return true;
	}

	const bool DS2413::SetProtected(const std::string& option, const std::string& value) const
	{
		//std::cout << "DS2413::SetProtected()" << std::endl;
		//std::cout << "\t" << "[" << option << "]=" << value << std::endl;
		if (option != "SwitchA" &&	option != "SwitchB")
			return false;
		if (value != "0" && value != "1")
			return false;
		// ----------------------------------------------------------
		// Чтение
		// Сброс шины
		if (!DJOneWire::mOneWire.reset())
			return false;
		// Выбор устройства
		DJOneWire::mOneWire.select(mAddress.Array());
		// PIO Доступ на чтение
		DJOneWire::mOneWire.write(0xF5, 1);
		// Чтение
		DS2413::DataRead dataRead(DJOneWire::mOneWire.read());
		// Проверка целостности
		if (!dataRead.Check())
			return false;
		// ----------------------------------------------------------
		// Запись
		// Сброс шины
		if (!DJOneWire::mOneWire.reset())
			return false;
		// Выбор устройства
		DJOneWire::mOneWire.select(mAddress.Array());
		// PIO Доступ на запись
		DJOneWire::mOneWire.write(0x5A, 1);
		// Запись данных
		DS2413::DataWrite dataWrite(dataRead);
		if (option == "SwitchA")
			dataWrite.set(0, value == "0");
		if (option == "SwitchB")
			dataWrite.set(1, value == "0");
		//std::cout << "\t" << "[dataWrite]=" << dataWrite << std::endl;
		DJOneWire::mOneWire.write(dataWrite.to_uint(), 1);
		// Запись инвертированных данных
		dataWrite.flip();
		//std::cout << "\t" << "[dataWrite]=" << dataWrite << std::endl;
		DJOneWire::mOneWire.write(dataWrite.to_uint(), 1);
		// Чтение подтверждения
		if (DJOneWire::mOneWire.read() != 0xAA)
			return false;
		// Публикация новых значений
		SetValueProtected(option, value);
		return true;
	}
}