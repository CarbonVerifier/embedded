#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <time.h>

#include "sensors/dht11.hpp"
#include "sensors/gy30.hpp"
#include "sensors/mq135.hpp"

#define WIFI_SSID "---"
#define WIFI_PASSWORD "---"
#define BROKER_SERVER "---"

#define BROKER_PORT 8883
#define BROKER_USER "---"
#define BROKER_PASSWORD "---"

#define CLIENT_ID 0

DHT11Sensor dht11;
GY30Sensor gy30;
MQ135Sensor mq135;

WiFiClientSecure espClient;
PubSubClient client(espClient);

struct Package {
	float temperature_celsius;
	float humidity_percent;
	float light_lux;
	float co2_ppm;
};

void setDateTime() {
	// Brazilian time zone
	configTime(0, -3 * 3600, "pool.ntp.org", "time.nist.gov");

	Serial.print("Waiting for NTP time sync: ");
	time_t now = time(nullptr);
	while (now < 8 * 3600 * 2) {
		delay(100);
		Serial.print(".");
		now = time(nullptr);
	}
	Serial.println();

	struct tm timeinfo;
	gmtime_r(&now, &timeinfo);
	Serial.printf("%s %s", tzname[0], asctime(&timeinfo));
}

void connectMQTT() {
	Serial.print("Connecting to MQTT broker...");
	espClient.setInsecure();
	client.setServer(BROKER_SERVER, BROKER_PORT);
	while (!client.connected()) {
		if (client.connect("MicrocontrollerClient", BROKER_USER, BROKER_PASSWORD)) {
			Serial.println("  done!.");
		} else {
			Serial.print("failed with state ");
			Serial.print(client.state());
			delay(2000);
		}
	}
}

void connectWIFI() {
	Serial.print("Connecting to WiFi...");
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println("  done!.");
}

void setup() {
	Serial.begin(115200);

	dht11.begin();
	gy30.begin();
	mq135.begin();

	mq135.calibrate();

	connectWIFI();

	setDateTime();

	connectMQTT();
}

void publish(Package package) {
	client.publish("temperature", String(package.temperature_celsius).c_str());
	client.publish("humidity", String(package.humidity_percent).c_str());
	client.publish("light", String(package.light_lux).c_str());
	client.publish("co2", String(package.co2_ppm).c_str());
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

	publish(package);
	delay(1000);
}