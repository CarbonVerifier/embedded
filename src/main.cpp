#include <Arduino.h>

#include "sensors/dht11.hpp"
#include "sensors/gy30.hpp"
#include "sensors/mq135.hpp"

DHT11Sensor dht11;
GY30Sensor gy30;
MQ135Sensor mq135;

struct Package {
	float temperature_celsius;
	float humidity_percent;
	float light_lux;
	float co2_ppm;
};

void setup() {
	Serial.begin(115200);
	dht11.begin();
	gy30.begin();
	mq135.begin();

	mq135.calibrate();
}

void loop() {
	Package package;
	package.temperature_celsius = dht11.getTemperature();
	package.humidity_percent = dht11.getHumidity();
	package.light_lux = gy30.getLux();
	package.co2_ppm = mq135.getCO2PPM();

	Serial.println("Temperature: " + String(package.temperature_celsius) + " C");
	Serial.println("Humidity: " + String(package.humidity_percent) + " %");
	Serial.println("Light: " + String(package.light_lux) + " lux");
	Serial.println("CO2: " + String(package.co2_ppm) + " ppm");
	Serial.println();
	delay(1000);
}