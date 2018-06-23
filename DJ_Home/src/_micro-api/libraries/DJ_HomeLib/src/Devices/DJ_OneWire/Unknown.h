#pragma once

#include "DJ_OneWire.h"

namespace home
{
	class Unknown :public OneWireDevice
	{
	public:
		Unknown(const OneWireAddress& address);
		virtual Unknown* Clone() { return new Unknown(*this); }
		virtual bool Loop() const;
	};
}