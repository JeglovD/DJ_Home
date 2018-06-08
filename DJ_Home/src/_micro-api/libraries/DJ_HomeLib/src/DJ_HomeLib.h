#pragma once

#include <StandardCplusplus.h>
#include <serstream.h>
#include <string>
#include <map>
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
		virtual bool Loop() const = 0;
		virtual const std::string Type() const = 0;
		//virtual bool Execute(const std::string& function, const std::string& value) const;
		template <typename T>
		const T OptionGet(const std::string& option) const
		{
			T result = T();
			auto pIt = mValue.find(option);
			if (pIt == mValue.end())
				return result;
			std::stringstream ss(pIt->second);
			ss >> result;
			return result;
		}
		void OptionPublish(const String& option) const;
		void Set(const std::string& json) const;
		void Set(const std::string& option, const std::string& value) const;

		std::map<std::string, void(Device::*)(const std::string&) const> mFunctions;

	protected:
		// ������������ ������������ ��� ���������� ����� �������� � map mValue
		// ��������� ���� �� ���������� � ������ ���������������� ����������
		//		���� ��� - �������� �� ����������, ������� MQTT ���������
		void SetValueProtected(const std::string& option, const std::string& value) const;
		void Publish(const std::string& json) const;
		// �������� ��������� ����������
		// ��� �������� ��� ������� ���������� �������������
		virtual const bool SetProtected(const std::string& option, const std::string& value) const { return false; };

		std::string mAddress;
		std::map<std::string, std::string> mValue;
		mutable unsigned long mLoopMillis;
	};

	class Devices :public std::map<std::string, Device*>
	{
	private:
		Devices();
		Devices(const Devices&);
		Devices& operator=(Devices&);
	public:
		static Devices& GetInstance();
		~Devices();
		bool Loop() const;
		//void OptionPublish(const std::string& device, const std::string& option = "") const;
		//void Set(const std::string& device, const std::string& option, const std::string& value) const;
		//const std::string Get(const std::string& option) const;

		//std::map<std::string, Device*> mDevices;
	};
}