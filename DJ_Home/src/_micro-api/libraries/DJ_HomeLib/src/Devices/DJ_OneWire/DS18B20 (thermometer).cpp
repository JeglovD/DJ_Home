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
		// ����� ����
		if (DJOneWire::mOneWire.reset())
		{
			// ����� ����������
			DJOneWire::mOneWire.select(mAddress.Array());
			// ������� ������� ������ ��������������� �����������, ��������� ������������ � �������� �������
			DJOneWire::mOneWire.write(0x44, 1);
			delay(1000);
			// ����� ����
			DJOneWire::mOneWire.reset();
			// ����� ����������
			DJOneWire::mOneWire.select(mAddress.Array());
			// ������� ������� ������ ���������� �������� ����������� �� ��������
			DJOneWire::mOneWire.write(0xBE);
			// ������ (����������) 9 ���� (8 ���� ����������� + 1 ���� ����������� ���)
			byte data[9];
			DJOneWire::mOneWire.read_bytes(data, 9);
			// ���� ������� ���������� �� ������ (�� ������ �����) - ������ ���������� �� ����������
			if (!~(*(uint64_t*)data) && !(uint8_t)~data[8])
				return;
			// ��������� ����������� ����� � ���� ������� - ���������� �������� �����������
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