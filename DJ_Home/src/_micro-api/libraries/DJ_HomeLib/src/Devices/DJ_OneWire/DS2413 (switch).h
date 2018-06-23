#pragma once

#include "DJ_OneWire.h"
#include "..\..\std\DJ_bitset.h"

namespace home
{
	class DS2413 :public OneWireDevice
	{
	public:
		DS2413(const OneWireAddress& address);
		virtual DS2413* Clone() { return new DS2413(*this); }
		virtual bool Loop() const;
		virtual const std::string Type() const { return OneWireDevice::Type() + " / DS2413 (switch)"; }
	//	//virtual bool Execute(const std::string& function, const std::string& value) const;
	protected:
		virtual const bool SetProtected(const std::string& option, const std::string& value) const;
	private:
		class DataRead :public std::DJ_bitset<8>
		{
		public:
			DataRead(const uint8_t& data) :
				std::DJ_bitset<8>(data)
			{};
			bool Check() const;
		};
		class DataWrite :public std::DJ_bitset<8>
		{
		public:
			DataWrite(const DS2413::DataRead& dataRead):
				std::DJ_bitset<8>()
			{
				set(0, dataRead.test(0));
				set(1, dataRead.test(2));
			};
		};
	};
}