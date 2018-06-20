#pragma once

#include <StandardCplusplus.h>
#include <functional>
#include <serstream.h>
#include <string>
#include <map>
#include <set>
#include <OneWire.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <PubSubClient.h>

#include <iostream>
#include <sstream>

namespace home
{
	class Devices;
	
	class Device
	{
	public:
		Device(const std::string& Address);
		virtual ~Device() {};
		virtual Device* Clone() = 0;
		//bool operator<(const Device& device) const { return mAddress < device.mAddress; }
		virtual const std::string& Address() { return mAddress; }
		virtual bool Loop() const = 0;
		//virtual const std::string Type() const = 0;
		////virtual bool Execute(const std::string& function, const std::string& value) const;
		template <typename T>
		const T Get(const std::string& option) const
		{
			T result = T();
			auto pIt = mValue.find(option);
			if (pIt == mValue.end())
				return result;
			std::stringstream ss(pIt->second);
			ss >> result;
			return result;
		}
		//void OptionPublish(const String& option) const;
		//void Set(const std::string& json) const;
		//void Set(const std::string& option, const std::string& value) const;

		//std::map<std::string, void(Device::*)(const std::string&) const> mFunctions;

	protected:
		// Используется датчиками для сохранения своих значений в map mValue
		// При сохранении проверяет есть ли переменная в списке соответствующего устройства
		// Публикует сообщение в MQTT
		void Save(const std::string& option, const std::string& value) const;
		//void Publish(const std::string& json) const;
		//// Изменяет состояние устройства
		//// Код задается для каждого устройства индивидуально
		//virtual const bool SetProtected(const std::string& option, const std::string& value) const { return false; };

		const std::string mAddress;
		std::map<std::string, std::string> mValue;
		mutable unsigned long mLoopMillis;
	};

	class DeviceCompare
	{
	public:
		bool operator()(const Device* pDevice1, const Device* pDevice2) const
		{
			return pDevice1->Address() < pDevice2->Address();
		}
	};

	//class Devices :public std::map<std::string, Device*>
	class Devices :public std::set<Device*, DeviceCompare>
	{
	private:
		Devices();
		Devices(const Devices&);
		Devices& operator=(Devices&);
		~Devices();
	public:
		static Devices& GetInstance();
		std::pair<iterator, bool> insert(Device&& device);
		iterator find(const std::string& address);
		bool Loop() const;
	//	//void OptionPublish(const std::string& device, const std::string& option = "") const;
	//	//void Set(const std::string& device, const std::string& option, const std::string& value) const;
	//	//const std::string Get(const std::string& option) const;

	//	//std::map<std::string, Device*> mDevices;
	};
}
