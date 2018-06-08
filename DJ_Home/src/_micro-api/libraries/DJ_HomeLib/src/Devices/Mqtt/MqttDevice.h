#pragma once

#pragma once

#include "..\..\DJ_HomeLib.h"
#include <string>

namespace home
{
	class MqttDevice :public Device
	{
	public:
		MqttDevice(const std::string& address);
		virtual bool Loop() const { return true; };
		virtual const std::string Type() const { return "MqttDevice"; };
		void Publish(const std::string& json) const;
	};
}