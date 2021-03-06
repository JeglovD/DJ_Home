#include "Mqtt.h"

// ������
#include <sstream>

namespace home
{
	//Devices* Mqtt::mpDevices(NULL);

	Mqtt::Mqtt():
		mMac{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 },
		mMqttServer{ 192, 168, 1, 201 },
		mEthernetClient(),
		mMqttClient(mMqttServer, 1883, Callback, mEthernetClient)
		{}
	
	Mqtt& Mqtt::GetInstance()
	{
		static Mqtt mqtt;
		return mqtt;
	}

	void Mqtt::Publish(const std::string& topic, const std::string& value)
	{
		//std::cout << "Mqtt::Publish()" << std::endl;
		if (Loop())
			mMqttClient.publish(topic.c_str(), value.c_str());
	}

	bool Mqtt::Loop()
	{
		if (mMqttClient.connected())
			return mMqttClient.loop();
		else
			if (Ethernet.begin(mMac, 150, 0) &&
				mMqttClient.connect("HomeLib"))
				return mMqttClient.subscribe("#");
		return false;
	}

	void Mqtt::Callback(char* topic, byte* payload, unsigned int length)
	{
		std::cout << "Mqtt::Callback()" << std::endl;
		std::stringstream ssTopic(topic);
		std::cout << "\t" << "[topic]=" << ssTopic.str() << std::endl;
		std::vector<std::string> vTopic;
		while (ssTopic.good())
		{
			std::string s;
			std::getline(ssTopic, s, '/');
			vTopic.push_back(s);
		}
		std::string value(payload, length);
		std::cout << "\t" << "[value]=" << value << std::endl;

		// ����� ������� ����� Mqtt
		if (vTopic.size() >= 2 &&
			(*vTopic.rbegin()).find("()") != std::string::npos)
		{
			auto& deviceName = *(++vTopic.rbegin());
			auto& functionName = *vTopic.rbegin();
			Devices& devices = Devices::GetInstance();
			auto pIt = devices.find(deviceName);
			if (pIt != devices.end())
			{
				if ((**pIt).mFunctions.count(functionName))
					((**pIt).*((**pIt).mFunctions[functionName]))(value);
				else
					Mqtt::GetInstance().Publish("Message", deviceName + "/" + functionName + " is not found.");
			}
		}
	}
}