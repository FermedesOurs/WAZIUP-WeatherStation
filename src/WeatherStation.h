#ifndef WEATHER_STATION
#define WEATHER_STATION

#include <Arduino.h>
#define ENABLE_DEBUG	// Enabling debug prints

#define VBATPIN A7    // Voltage Battery Voltage Pin
#define WDIR_CALIBRATION 0.973    // (990/1017) 990 = max value table, 1017 max ADC acquired

// Weather Station pins to Adafruit Feather M0
#define RAIN 11
#define WSPEED 12
#define WDIR A0

// LoRa module pins to Adafruit Feather M0
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

// Define Baudrate
#define BAUDRATE 9600

// Include Weather Station Objects
#include "SensorRTC.h"
#include "WeatherRecord.h"
#include "WeatherComunication.h"
#include "SensorTemperature.h"
#include "SensorHumidity.h"
#include "SensorPressure.h"
#include "SensorRain.h"
#include "SensorWind.h"


class WeatherStation
{
	// Public Variables/Functions
	public:
	WeatherStation(int period_time);
	void init();
	int begin();
	int beginSensors();
	void clearValues();
	void getWeatherValues();
	void getWeatherAverage();
	void task();
	float getBatteryVoltage();


	//Private Variables/Functions
	private:
	SensorRTC rtc;
	WeatherRecord wr;
	WeatherComunication wc;
	SensorTemperature st;
	SensorHumidity sh;
	SensorPressure sp;
	SensorRain sr;
	SensorWind sw;

	unsigned long minutes;
	unsigned long time_before;
	unsigned long time_after;
	int period;

	//HardwareSerial* DefaultSerial;

	float * temperatures;
	float * humidities;
	float * pressures;
	float * voltages;

	float temperature;
	float humidity;
	float pressure;
	float batteryVoltage;

	float windDirection;
	float windGust;
	float windSpeed;
	float amountRain;

};

#endif
