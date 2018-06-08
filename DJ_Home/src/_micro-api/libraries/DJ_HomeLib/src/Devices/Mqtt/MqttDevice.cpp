#include "MqttDevice.h"
#include "Mqtt.h"
#include <ArduinoJson\ArduinoJson.h>

namespace home
{
	MqttDevice::MqttDevice(const std::string& address) :
		Device(address)
	{
		std::cout << "MqttDevice::MqttDevice()" << std::endl;
		auto pFunctions = (std::map<std::string, void(MqttDevice::*)(const std::string&) const>*)&mFunctions;
		(*pFunctions)["Publish()"] = &MqttDevice::Publish;
	}

	void MqttDevice::Publish(const std::string& json) const
	{
		//std::cout << "MqttDevice::Publish()" << std::endl;
		//std::cout << "\t" << "[json]=" << json << std::endl;
		ArduinoJson::StaticJsonBuffer<200> jsonBuffer;
		ArduinoJson::JsonObject& jsonObject = jsonBuffer.parseObject(json.c_str());
		for (auto pIt = jsonObject.begin(); pIt != jsonObject.end(); ++pIt)
			Mqtt::GetInstance().Publish(pIt->key, pIt->value.asString());
	}
}