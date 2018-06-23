#include "Ventmachine.h"

namespace home
{
	Ventmachine::Ventmachine(const std::string& address) :
		Device(address)//,
	//	//mOffMillis(0)
	{
		std::cout << "Ventmachine::Ventmachine()" << std::endl;
		mValue["LoopMillis"] = "10000";
		mValue["CO2"] = "MH_Z16/CO2";
		mValue["Switch"] = "133:154:12:119:192:3:16:180/SwitchA";
		mValue["CO2delta"] = "";
	}

	const unsigned int Ventmachine::CO2min() const
	{
		unsigned int result(0);
		for (auto pIt = mCO2.begin(); pIt != mCO2.end(); ++pIt)
			result = result ? min(pIt->second, result) : pIt->second;
		return result;
	}

	const unsigned int Ventmachine::CO2max() const
	{
		unsigned int result(0);
		for (auto pIt = mCO2.begin(); pIt != mCO2.end(); ++pIt)
			result = result ? max(pIt->second, result) : pIt->second;
		return result;
	}

	bool Ventmachine::Loop() const
	{
		if (millis() - mLoopMillis < Get<unsigned long>("LoopMillis"))
			return false;
		mLoopMillis = millis();
		std::cout << "Ventmachine::Loop()" << std::endl;
		Devices& devices = Devices::GetInstance();
	//	//unsigned long mMillis(millis());
	//	//// Если millis() обнулился - очищаем mCO2
	//	//if (mCO2.size() && 
	//	//	mMillis < (--mCO2.end())->first)
	//	//	mCO2.clear();
	//	////
		auto co2 = devices.Get<unsigned int>(mValue["CO2"]);
		if (co2)
		{
			mCO2[mLoopMillis] = co2;
			Save("CO2delta", CO2max() - CO2min());
		}
		if (devices.Get<bool>(mValue["Switch"]))
		{
			// Приточка включена
			if (mCO2.size())
			{
				// Время работы
				unsigned long workingTime = mCO2.rbegin()->first - mCO2.begin()->first;
				std::cout << "\t" << "[workingTime]=" << workingTime << std::endl;
				unsigned long workingTimeMax = 1 * 60000;
				std::cout << "\t" << "[workingTimeMax]=" << workingTimeMax << std::endl;
				std::cout << "\t" << "[workingTime >= workingTimeMax]=" << (workingTime >= workingTimeMax) << std::endl;
				std::cout << "\t" << "[CO2delta]=" << Get<unsigned int>("CO2delta") << std::endl;

				// Если работает долго, проверяем не нужно ли выключить
				// Выключаем, если разница между CO2max - CO2min небольшая
				if (workingTime >= workingTimeMax //&& 
					//Get<unsigned int>("CO2delta") < 100
					)
				{
					std::cout << "\t" << "Off" << std::endl;
					Devices::GetInstance().Set(mValue["Switch"], 0);
					mCO2.clear();
				}
			}
		}
		else
		{
			// Приточка выключена
		}
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
	}
}