#ifndef WEATHER_COMUNICATION
#define WEATHER_COMUNICATION

#include <Arduino.h>
#include <avr/dtostrf.h>    // Uncomment to use in Feather M0
#include "WeatherRecord.h"

#include "SX1272.h"

// SX1272 Configurations
#define DEFAULT_CHANNEL CH_10_868
#define LORAMODE  1
#define node_addr 25
#define DEFAULT_DEST_ADDR 1



class WeatherComunication
{
	// Public Variables/Functions
	public:
	WeatherComunication();		// Receive the LoRa module configurations
	int configLoRa();															// Begin & Config the LoRa module (return 0 if ok, return 1 if erro)
	void sendMessageSerial(WeatherRecord wr);			// Receive the Weather Record & send message via Serial Monitor
	void sendMessageLoRa(WeatherRecord wr);				// Receive the Weather Record & send message via LoRa

	//Private Variables/Functions
	private:
			SX1272 sx;
};

#endif
