#ifndef WEATHER_COMUNICATION
#define WEATHER_COMUNICATION

#include <Arduino.h>

#include <avr/dtostrf.h>    // Uncomment to use in Feather M0

#include "WeatherRecord.h"


#include "SX1272.h"



class WeatherComunication
{
	public:
	//Public Functions
	WeatherComunication();		// Receive the LoRa module configurations
	// WeatherComunication(int lora_cs, int lora_rst, int lora_int, float lora_freq);		// Receive the LoRa module configurations
	// WeatherComunication(int lora_cs, int lora_rst, int lora_int, float lora_freq, const char* channel);		// Receive the LoRa module configurations
	int configLoRa();																	// Begin & Config the LoRa module (return 0 if ok, return 1 if erro)
	//void sendMessageSerial(WeatherRecord wr, HardwareSerial* DefaultSerial);			// Receive the Weather Record & send message via Serial Monitor
	void sendMessageSerial(WeatherRecord wr);			// Receive the Weather Record & send message via Serial Monitor
	void sendMessageLoRa(WeatherRecord wr);												// Receive the Weather Record & send message via LoRa

	//Public Variables

	private:
			SX1272 sx;


	//Private Functions

	//Private Variables
	int lora_cs;			// LoRa module chip select pin
	int lora_rst;			// LoRa module reset pin
	int lora_int;			// LoRa imodule nterrup pin
	float lora_freq;		// LoRa module frequency
	const char* ts_channel;
	unsigned long int cont;
};

#endif
