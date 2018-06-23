#include "DS2413 (switch).h"

namespace home
{
	DS2413::DS2413(const OneWireAddress& address) :
		OneWireDevice(address)
	{
		std::cout << "DS2413::DS2413()" << std::endl;
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
		if (millis() - mLoopMillis < Get<unsigned long>("LoopMillis"))
			return false;
		mLoopMillis = millis();
		std::cout << "bool DS2413::Loop()" << std::endl;
		::OneWire& oneWire = DJOneWire::GetInstance().mOneWire;
		// ����� ����
		if (!oneWire.reset())
			return false;
		// ����� ����������
		oneWire.select(mOneWireAddress.Array());
		// PIO ������ �� ������
		oneWire.write(0xF5, 1);
		// ������
		DS2413::DataRead dataRead(oneWire.read());
		// �������� �����������
		if (!dataRead.Check())
			return false;
		Save("SwitchA", dataRead.test(0) ? "0" : "1");
		Save("SwitchB", dataRead.test(2) ? "0" : "1");
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
		::OneWire& oneWire = DJOneWire::GetInstance().mOneWire;
		// ������
		// ����� ����
		if (!oneWire.reset())
			return false;
		// ����� ����������
		oneWire.select(mOneWireAddress.Array());
		// PIO ������ �� ������
		oneWire.write(0xF5, 1);
		// ������
		DS2413::DataRead dataRead(oneWire.read());
		// �������� �����������
		if (!dataRead.Check())
			return false;
		// ----------------------------------------------------------
		// ������
		// ����� ����
		if (!oneWire.reset())
			return false;
		// ����� ����������
		oneWire.select(mOneWireAddress.Array());
		// PIO ������ �� ������
		oneWire.write(0x5A, 1);
		// ������ ������
		DS2413::DataWrite dataWrite(dataRead);
		if (option == "SwitchA")
			dataWrite.set(0, value == "0");
		if (option == "SwitchB")
			dataWrite.set(1, value == "0");
		//std::cout << "\t" << "[dataWrite]=" << dataWrite << std::endl;
		oneWire.write(dataWrite.to_uint(), 1);
		// ������ ��������������� ������
		dataWrite.flip();
		//std::cout << "\t" << "[dataWrite]=" << dataWrite << std::endl;
		oneWire.write(dataWrite.to_uint(), 1);
		// ������ �������������
		if (oneWire.read() != 0xAA)
			return false;
		// ���������� ����� ��������
		Save(option, value);
		return true;
	}
}