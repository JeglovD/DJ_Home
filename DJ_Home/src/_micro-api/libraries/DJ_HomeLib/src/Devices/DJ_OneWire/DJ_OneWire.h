#pragma once

#include "..\..\DJ_HomeLib.h"
#include "..\Mqtt\Mqtt.h"

namespace home
{
	class OneWireAddress
	{
	public:
		OneWireAddress();
		uint8_t* Array() const { return mArray; }
		const std::string StrDec() const;
		const std::string StrHex() const;
		const std::string StrBit() const;
	private:
		uint8_t mArray[8];
	};

	class OneWireDevice: public Device
	{
	public:
		OneWireDevice(const OneWireAddress& address);
	//	virtual const std::string Type() const { return "OneWireDevice"; }
	//	const bool Found() const { return mFound; }
	//	void SetFound(const bool& found) const { mFound = found; }
	private:
		const OneWireAddress mAddress;
		mutable bool mFound;
	};
	
	class DJOneWire
	{
	private:
		DJOneWire();
		DJOneWire(const DJOneWire&);
		DJOneWire& operator=(DJOneWire&);
	public:
		static DJOneWire& GetInstance();
		bool Loop();
	//	static void Init(const Devices* pDevices) { mpDevices = pDevices; };
	//private:
		::OneWire mOneWire;
		//	static Devices* mpDevices;

		//const Device* POneWireDeviceNew(const OneWireAddress& address);
		const OneWireDevice OneWireDeviceNew(const OneWireAddress& address) const;
	};
}