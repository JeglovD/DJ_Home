#pragma once

#include "..\..\DJ_HomeLib.h"
#include <map>

namespace home
{
	class Ventmachine:public Device
	{
	public:
		Ventmachine(const std::string& address);
		virtual Ventmachine* Clone() { return new Ventmachine(*this); }
		virtual bool Loop() const;
		virtual const std::string Type() const { return "Executor / Ventmachine"; };
	//private:
	//	const Devices& mDevices;
	//	mutable std::map<unsigned long, unsigned int> mCO2;
	//	mutable unsigned long mOffMillis;

	//	const unsigned int CO2Min() const;
	//	const unsigned int CO2Max() const;
	};
}