#include "Ventmachine.h"

namespace home
{
	//Ventmachine::Ventmachine(const std::string& address) :
	//	Device(address)//,
	//	//mDevices(devices),
	//	//mOffMillis(0)
	//{
	//	std::cout << "Ventmachine::Ventmachine()" << std::endl;
	//	mValue["LoopMillis"] = "10000";
	//	//SetValueProtected("CO2", "MH_Z16/CO2");
	//	mValue["CO2"] = "MH_Z16/[CO2]";
	//	//SetValueProtected("Switch", "0.0.0.0.0.0.0.0/Switch0");
	//	mValue["Switch"] = "133_154_12_119_192_3_16_180/[SwitchA]";
	//}

	//const unsigned int Ventmachine::CO2Min() const
	//{
	//	unsigned int result(0);
	//	for (auto pIt = mCO2.begin(); pIt != mCO2.end(); ++pIt)
	//		result = result ? min(pIt->second, result) : pIt->second;
	//	return result;
	//}

	//const unsigned int Ventmachine::CO2Max() const
	//{
	//	unsigned int result(0);
	//	for (auto pIt = mCO2.begin(); pIt != mCO2.end(); ++pIt)
	//		result = result ? max(pIt->second, result) : pIt->second;
	//	return result;
	//}

	//bool Ventmachine::Loop() const
	//{
	//	if (millis() - mLoopMillis < OptionGet<unsigned long>("LoopMillis"))
	//		return false;
	//	mLoopMillis = millis();
	//	std::cout << "Ventmachine::Loop()" << std::endl;
	//	//std::cout << "\t" << "[CO2]=" << pDevices->Get(mValue["CO2"]) << std::endl;
	//	//unsigned long mMillis(millis());
	//	//// Если millis() обнулился - очищаем mCO2
	//	//if (mCO2.size() && 
	//	//	mMillis < (--mCO2.end())->first)
	//	//	mCO2.clear();
	//	////
	//	//mCO2[mMillis] = atoi(mDevices.Get(Get("CO2")).c_str());
	//	//if (mDevices.Get(Get("Switch")) == "true")
	//	//{
	//	//	std::cout << "\t" << "Switch true" << std::endl;
	//	//	// Запоминаем последнее время работы приточки
	//	//	mOffMillis = mMillis;
	//	//	// Чистим значения CO2, выходящие за последние 30 мин
	//	//	unsigned long millis30min = (unsigned long)1000 * (unsigned long)60 * (unsigned long)30;
	//	//	std::cout << "\t" << "[millis30min] = " << millis30min << std::endl;
	//	//	unsigned long eraseMillis = mOffMillis > millis30min ? mOffMillis - millis30min : 0;
	//	//	if (mCO2.begin() != mCO2.end())
	//	//	{
	//	//		std::cout << "\t" << "[eraseMillis] = " << eraseMillis << std::endl;
	//	//		std::cout << "\t" << "[mCO2.begin()] = " << mCO2.begin()->first << std::endl;
	//	//		if (eraseMillis >= mCO2.begin()->first)
	//	//		{
	//	//			auto pEraseMillisIt = mCO2.lower_bound(eraseMillis);
	//	//			if (pEraseMillisIt != mCO2.end())
	//	//			{
	//	//				mCO2.erase(mCO2.begin(), pEraseMillisIt);
	//	//				// Если разница max-min меньше 100 - выключаем приточку
	//	//				std::cout << "\t" << "CO2: " << CO2Max() << " - " << CO2Min() << " = " << CO2Max() - CO2Min() << std::endl;
	//	//				if (CO2Max() - CO2Min() < 100)
	//	//					mDevices.Set(Get("Switch"), "false");
	//	//			}
	//	//		}
	//	//	}
	//	//}
	//	//else
	//	//{
	//	//	std::cout << "\t" << "Switch false" << std::endl;
	//	//	std::cout << "\t" << "CO2: " << CO2Max() << " - " << CO2Min() << " = " << CO2Max() - CO2Min() << std::endl;
	//	//	// Включаем приточку если
	//	//	if (// Разница CO2 max-min больше 100
	//	//		CO2Max() - CO2Min() >= 100 ||				
	//	//		// Начался новый круг millis()
	//	//		mMillis < mOffMillis ||						
	//	//		// прошло более 6 часов
	//	//		mMillis - mOffMillis >= (unsigned long)1000 * (unsigned long)60 * (unsigned long)60 * (unsigned long)6)
	//	//	{
	//	//		mDevices.Set(Get("Switch"), "true");
	//	//		mCO2.clear();
	//	//	}
	//	//}
	//	//std::cout << "\t" << mCO2.size() << std::endl;
	//}
}