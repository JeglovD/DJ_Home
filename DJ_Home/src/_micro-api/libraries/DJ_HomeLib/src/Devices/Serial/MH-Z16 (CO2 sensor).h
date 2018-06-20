#pragma once

#include "..\..\DJ_HomeLib.h"
#include "..\Mqtt\Mqtt.h"

namespace home
{
	class MH_Z16 :public Device
	{
	public:
		MH_Z16(const std::string& address);
		virtual MH_Z16* Clone() { return new MH_Z16(*this); }
		virtual bool Loop() const;
	//	virtual const std::string Type() const { return "SerialDevice / MH-Z16 (CO2 sensor)"; };
	};
}