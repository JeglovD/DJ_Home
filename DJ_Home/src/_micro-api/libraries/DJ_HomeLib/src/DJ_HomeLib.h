#pragma once

#include <StandardCplusplus.h>
#include <functional>
#include <serstream.h>
#include <string>
#include <sstream>
#include <map>
#include <set>
#include <OneWire.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <PubSubClient.h>

namespace home
{
	class Devices;
	
	class Device
	{
	public:
		Device(const std::string& Address);
		virtual ~Device() { std::cout << "Device::~Device()" << std::endl; };
		virtual Device* Clone() = 0;
		//bool operator<(const Device& device) const { return mAddress < device.mAddress; }
		virtual const std::string& Address() { return mAddress; }
		virtual bool Loop() const = 0;
		virtual const std::string Type() const = 0;
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
		void Set(const std::string& json) const;
		//void Set(const std::string& option, const std::string& value) const;
		//void Device::Set(const std::string& option, const std::string& value) const
		template <typename T>
		void Set(const std::string& option, const T& value) const
		{
			//std::cout << "Device::OptionSet()" << std::endl;
			//std::cout << "\t" << "[" << id << "] = " << value << std::endl;
			if (mValue.find(option) == mValue.end())
				MqttPublish(mAddress + "/Message", "The \"" + option + "\" option is not supported.");
			else
			{
				std::stringstream ss;
				ss << value;
				if (!SetProtected(option, ss.str()))
					MqttPublish(mAddress + "/Message", "The \"" + option + "\" option cannot be set.");
			}
		}


		std::map<std::string, void(Device::*)(const std::string&) const> mFunctions;

	protected:
		// ������������ ��������� ��� ���������� ����� �������� � map mValue
		// ��� ���������� ��������� ���� �� ���������� � ������ ���������������� ����������
		// ��������� ��������� � MQTT
		//void Save(const std::string& option, const std::string& value) const;
		//void Device::Save(const std::string& option, const std::string& value) const
		template <typename T>
		//void Device::Save(const std::string& option, const ValueT& value) const
		void Save(const std::string& option, const T& value) const
		{
			//	//std::cout << "Device::OptionSetProtected()" << std::endl;
			//	//std::cout << "\t" << "[" << option << "]=" << value << std::endl;
			auto pIt = mValue.find(option);
			if (pIt == mValue.end())
				MqttPublish(mAddress + "/Message", "The \"" + option + "\" option is not supported.");
			else
			{
				std::stringstream ss;
				ss << value;
				pIt->second = ss.str();
				MqttPublish(mAddress + "/" + option, pIt->second);
			}
		}

		//void Publish(const std::string& json) const;
		// �������� ��������� ����������, ���������� �� Set()
		// ��� �������� ��� ������� ���������� �������������
		virtual const bool SetProtected(const std::string& option, const std::string& value) const { return false; };

		std::map<std::string, std::string> mValue;
		mutable unsigned long mLoopMillis;

	private:
		void MqttPublish(const std::string& topic, const std::string& value) const;
		const std::string mAddress;
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
		void MqttPublish(const std::string& topic, const std::string& value) const;
		//	//void OptionPublish(const std::string& device, const std::string& option = "") const;
		//void Set(const std::string& device, const std::string& option, const std::string& value) const;
		//void Devices::Set(const std::string& device, const std::string& option, const std::string& value) const
		template <typename T>
		void Set(const std::string& topic, const T& value)
		{
			auto pos = topic.rfind("/");
			if (pos != std::string::npos)
			{
				std::string device = topic.substr(0, pos);
				std::string option = topic.substr(pos + 1);
				auto pIt = this->find(device);
				if (pIt == end())
					MqttPublish("Message", "Device \"" + device + "\" is not found.");
				else
					(**pIt).Set(option, value);
			}
		}
		template <typename T>
		const T Get(const std::string& topic)
		{
			auto pos = topic.rfind("/");
			if (pos != std::string::npos)
			{
				std::string device = topic.substr(0, pos);
				std::string option = topic.substr(pos + 1);
				auto pIt = this->find(device);
				if (pIt != end())
					return (**pIt).Get<T>(option);
			}
			return T();
		}
	};
}
