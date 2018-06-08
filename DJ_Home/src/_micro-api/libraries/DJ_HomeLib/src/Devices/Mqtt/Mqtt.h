#pragma once

#include "..\..\DJ_HomeLib.h"

namespace home
{
	class Mqtt
	{
	private:
		Mqtt();
		Mqtt(const Mqtt&);
		Mqtt& operator=(Mqtt&);
	public:
		static Mqtt& GetInstance();
		// Жеглов
		//static void Publish(const std::string& address, const std::string& option, const std::string& value);
		void Publish(const std::string& topic, const std::string& value = "");
		bool Loop();
		//static void Init(const Devices* pDevices) { mpDevices = pDevices; };
	private:
		static void Callback(char* topic, byte* payload, unsigned int length);

		byte mMac[6];
		byte mMqttServer[4];
		EthernetClient mEthernetClient;
		PubSubClient mMqttClient;
		//static Devices* mpDevices;
	};
}