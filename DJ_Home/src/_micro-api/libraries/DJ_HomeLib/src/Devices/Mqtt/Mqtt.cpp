#include "Mqtt.h"

// Жеглов
#include <sstream>

namespace home
{
	//Devices* Mqtt::mpDevices(NULL);

	//Mqtt::Mqtt():
	//	mMac{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 },
	//	mMqttServer{ 192, 168, 1, 201 },
	//	mEthernetClient(),
	//	mMqttClient(mMqttServer, 1883, Callback, mEthernetClient)
	//	{}
	//
	//Mqtt& Mqtt::GetInstance()
	//{
	//	static Mqtt mqtt;
	//	return mqtt;
	//}

	//void Mqtt::Publish(const std::string& topic, const std::string& value)
	//{
	//	//std::cout << "Mqtt::Publish()" << std::endl;
	//	//if (Loop())
	//	//	mMqttClient.publish(topic.c_str(), value.c_str());
	//}

	//bool Mqtt::Loop()
	//{
	//	if (mMqttClient.connected())
	//		return mMqttClient.loop();
	//	else
	//		if (Ethernet.begin(mMac, 150, 0) &&
	//			mMqttClient.connect("HomeLib"))
	//			return mMqttClient.subscribe("#");
	//	return false;
	//}

	//void Mqtt::Callback(char* topic, byte* payload, unsigned int length)
	//{
	//	std::cout << "Mqtt::Callback()" << std::endl;
	////	std::stringstream ssTopic(topic);
	////	std::vector<std::string> vTopic;
	////	while (ssTopic.good())
	////	{
	////		std::string s;
	////		std::getline(ssTopic, s, '/');
	////		vTopic.push_back(s);
	////	}
	////	std::string value(payload, length);
	////	//std::cout << "\t" << "[vTopic]" << std::endl;
	////	//for (auto it = vTopic.begin(); it != vTopic.end(); ++it)
	////	//	std::cout << "\t\t" << "[]=" << *it << std::endl;
	////	//std::cout << "\t" << "[value]=" << value << std::endl;

	////	// Вызов функций через Mqtt
	////	if (vTopic.size() >= 2 &&
	////		(*vTopic.rbegin()).find("()") != std::string::npos)
	////	{
	////		auto& deviceName = *(++vTopic.rbegin());
	////		auto& functionName = *vTopic.rbegin();
	////		if (mpDevices->count(deviceName))
	////		{
	////			auto pDevice = (*mpDevices)[deviceName];
	////			if (pDevice->mFunctions.count(functionName))
	////				((*pDevice).*(pDevice->mFunctions[functionName]))(value);
	////			else
	////				Mqtt::Publish("Message", deviceName + "/" + functionName + " is not found.");
	////		}
	////	}
	//}
}