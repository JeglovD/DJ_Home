#pragma once

#include "DJ_OneWire.h"

namespace home
{
	class DS18B20 :public OneWireDevice
	{
	public:
		DS18B20(const OneWireAddress& address);
		virtual DS18B20* Clone() { return new DS18B20(*this); }
		virtual bool Loop() const;
		virtual const std::string Type() const { return OneWireDevice::Type() + " / DS18B20 (thermometer)"; }
	//	//virtual bool Execute(const std::string& function, const std::string& value) const;
	};
}