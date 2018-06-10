#include "DJ_HomeLib.h"
#include "Devices\DJ_OneWire\DJ_OneWire.h"
#include "Devices\Mqtt\Mqtt.h"
#include "Devices\Mqtt\MqttDevice.h"
#include "Devices\Serial\MH-Z16 (CO2 sensor).h"
#include "Devices\Executor\Ventmachine.h"
#include <sstream>
#include <ArduinoJson\ArduinoJson.h>

namespace home
{
	Device::Device(const std::string& Address):
		mAddress(Address),
		mLoopMillis(0)
	{
		mValue["LoopMillis"] = "60000";
		mFunctions["Set()"] = &Device::Set;
	}

	void Device::Set(const std::string& json) const
	{
		//std::cout << "Device::Set()" << std::endl;
		//std::cout << "\t" << "[json]=" << json << std::endl;
		ArduinoJson::StaticJsonBuffer<200> jsonBuffer;
		ArduinoJson::JsonObject& jsonObject = jsonBuffer.parseObject(json.c_str());
		for (auto pIt = jsonObject.begin(); pIt != jsonObject.end(); ++pIt)
			Set(pIt->key, pIt->value.asString());
	}

	void Device::Set(const std::string& option, const std::string& value) const
	{ 
		//std::cout << "Device::OptionSet()" << std::endl;
		//std::cout << "\t" << "[" << id << "] = " << value << std::endl;
		if (mValue.find(option) == mValue.end())
			Mqtt::GetInstance().Publish(mAddress + "/Message", "The \"" + option + "\" option is not supported.");
		else
			if(!SetProtected(option, value))
				Mqtt::GetInstance().Publish(mAddress + "/Message", "The \"" + option + "\" option cannot be set.");
	}

	void Device::SetValueProtected(const std::string& option, const std::string& value) const
	{
		//std::cout << "Device::OptionSetProtected()" << std::endl;
		//std::cout << "\t" << "[" << option << "]=" << value << std::endl;
		if (mValue.find(option) == mValue.end())
			Mqtt::GetInstance().Publish(mAddress + "/Message", "The \"" + option + "\" option is not supported.");
		else
		{
			mValue[option] = value;
			Mqtt::GetInstance().Publish(mAddress + "/" + option, value);
		}
	}

	// Жеглов
	//const std::string Device::OptionGet(const std::string& id) const
	//{
	//	auto pIt = mValue.find(id);
	//	return pIt == mValue.end() ? "" : pIt->second;
	//}
	
	void Device::OptionPublish(const String& option) const
	{
		//std::cout << "Device::OptionPublish()" << std::endl;
		//std::cout << "\t" << "[option]=" << parameter << std::endl;
		//std::cout << "\t" << "[mAdress]=" << mAddress << std::endl;
		if (option == "")
			for (auto pIt = mValue.begin(); pIt != mValue.end(); ++pIt)
				Mqtt::GetInstance().Publish(mAddress + "/" + pIt->first, pIt->second);
		else
		{
			if (option == "?")
			{
				std::stringstream ss;
				ss << "Type: " << std::endl;
				ss << "\t" << Type() << std::endl;
				ss << "Options:" << std::endl;
				for (auto pIt = mValue.begin(); pIt != mValue.end(); ++pIt)
					ss << "\t" << pIt->first << "\t" << pIt->second << std::endl;
				Mqtt::GetInstance().Publish(mAddress, ss.str());
			}
			else
			{
				auto pIt = mValue.find(option.c_str());
				if (pIt != mValue.end())
					Mqtt::GetInstance().Publish(mAddress + "/" + pIt->first, pIt->second);
			}
		}
	}

	Devices::Devices()
	{
		std::cout << "Devices::Devices()" << std::endl;
	//	Mqtt::Init(this);
	//	DJOneWire::Init(this);
	//	(*this)["Mqtt"] = new MqttDevice("Mqtt");
		(*this)["MH_Z16"] = new MH_Z16("MH_Z16");
	//	//mDevices["Ventmachine"] = new Ventmachine(*this);
	//	(*this)["Ventmachine"] = new Ventmachine("Ventmachine");
	}

	Devices& Devices::GetInstance()
	{
		static Devices devices;
		return devices;
	}

	Devices::~Devices()
	{
		for (auto pIt = begin(); pIt != end(); ++pIt)
			delete pIt->second;
	}

	bool Devices::Loop() const
	{
	//	//std::cout << "bool Devices::Loop()" << std::endl;
		Mqtt::GetInstance().Loop();
	//	DJOneWire::Loop();
		//for (auto pIt = begin(); pIt != end(); ++pIt)
		//	pIt->second->Loop();
	}

	// Жеглов
	//void Devices::Set(const std::string& id, const std::string& value) const
	//{
	//	auto pos = id.rfind("/");
	//	if (pos != std::string::npos)
	//	{
	//		std::string device = id.substr(0, pos);
	//		std::string parametr = id.substr(pos + 1);
	//		auto pIt = mDevices.find(device);
	//		if (pIt != mDevices.end())
	//			pIt->second->Set(parametr, value);
	//	}
	//}

	//const std::string Devices::Get(const std::string& option) const
	//{
	//	std::string result;
	//	auto pos = id.rfind("/");
	//	if (pos != std::string::npos)
	//	{
	//		std::string device = id.substr(0, pos);
	//		std::string option = id.substr(pos + 1);
	//		auto pIt = find(device);
	//		if (pIt != end())
	//			result = pIt->second->OptionGet(option);
	//	}
	//	return result;
	//}

	//void Devices::OptionPublish(const std::string& device, const std::string& option) const
	//{
	//	//std::cout << "Devices::OptionPublish()" << std::endl;
	//	//std::cout << "\t" << "[device]=" << device << std::endl;
	//	//std::cout << "\t" << "[option]=" << parameter << std::endl;
	//	if (device == "?")
	//	{
	//		if (size())
	//			for (auto pIt = begin(); pIt != end(); ++pIt)
	//				pIt->second->OptionPublish("?");
	//		else
	//			Mqtt::Publish("Message", "No devices.");
	//	}
	//	else
	//	{
	//		auto pIt = find(device);
	//		if (pIt == end())
	//		{
	//			if (option == "?")
	//				Mqtt::Publish("Message", "Device \"" + device + "\" is not found.");
	//		}
	//		else
	//			pIt->second->OptionPublish(option.c_str());
	//	}
	//}

	//void Devices::Set(const std::string& device, const std::string& option, const std::string& value) const
	//{
	//	std::cout << "Devices::OptionSet()" << std::endl;
	//	std::cout << "\t" << "[device]=" << device << std::endl;
	//	std::cout << "\t" << "[option]=" << option << std::endl;
	//	std::cout << "\t" << "[value]=" << value << std::endl;

	//	auto pIt = find(device);
	//	if (pIt == end())
	//		Mqtt::Publish("Message", "Device \"" + device + "\" is not found.");
	//	else
	//		pIt->second->Set(option, value);
	//}
}