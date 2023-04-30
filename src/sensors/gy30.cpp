#include "sensors/gy30.hpp"

GY30Sensor::GY30Sensor() : m_gy30(0x23) {}	// 0x23 is the default address of the sensor

void GY30Sensor::begin() {
	Wire.begin();
	// Start measurement at 1lx resolution. Take 120ms for each measurement.
	this->m_gy30.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
}

uint16_t GY30Sensor::getLux() { return this->m_gy30.readLightLevel(); }
