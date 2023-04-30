#pragma once

#include <BH1750.h>
#include <Wire.h>

class GY30Sensor {
public:
	GY30Sensor();

	void begin();

	uint16_t getLux();

private:
	BH1750 m_gy30;
};