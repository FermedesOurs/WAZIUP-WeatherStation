
#include "WeatherComunication.h"


// SX1272 configurations
#define MAX_DBM 14
#define DEFAULT_CHANNEL CH_10_868
#define LORAMODE  1
#define node_addr 8
#define DEFAULT_DEST_ADDR 1




//Initialize
// WeatherComunication::WeatherComunication(int cs, int rst, int interrupt, float freq)
// :rf95(cs, interrupt)	// Singleton instance of the radio driver object
// {
// 	lora_cs = cs;			// Get LoRa module chip select pin
// 	lora_rst = rst;			// Get LoRa module reset pin
// 	lora_int = interrupt;	// Get LoRa imodule interrupt pin
// 	lora_freq = freq;		// Get LoRa module frequency
// 	cont=0;
// }

WeatherComunication::WeatherComunication()
:sx()	// Singleton instance of the radio driver object
{
	// lora_cs = cs;			// Get LoRa module chip select pin
	// lora_rst = rst;			// Get LoRa module reset pin
	// lora_int = interrupt;	// Get LoRa imodule interrupt pin
	// lora_freq = freq;		// Get LoRa module frequency
	// cont=0;
}

//Initialize
// WeatherComunication::WeatherComunication(int cs, int rst, int interrupt, float freq, const char* channel)
// :rf95(cs, interrupt)	// Singleton instance of the radio driver object
// {
// 	lora_cs = cs;			// Get LoRa module chip select pin
// 	lora_rst = rst;			// Get LoRa module reset pin
// 	lora_int = interrupt;	// Get LoRa imodule interrupt pin
// 	lora_freq = freq;		// Get LoRa module frequency
// 	ts_channel = channel;
// 	cont=0;
// }
	//----------------------------------------- Config LoRa ----------------------------------------
	int WeatherComunication::configLoRa()
	{
			// if(rf95.init()==1)
			// {
			// 	rf95.setFrequency(lora_freq);
			// 	delay(50);
			// 	rf95.sleep();
			// 	return 0;
			// }
			// return 1;


			// Power ON the module
			sx.ON();

			// Set transmission mode and print the result
			sx.setMode(LORAMODE);

			// enable carrier sense
  		sx._enableCarrierSense=true;

			// Select frequency channel
  		sx.setChannel(DEFAULT_CHANNEL);

			// Enable PA Boost
			sx._needPABOOST=true;

			//e = sx1272.setPowerDBM((uint8_t)MAX_DBM);
		  sx.setPower('X');// setting the power at maximum!, still range is extremely low. it can be 'x' X more power

		  // Set the node address and print the result
		  sx.setNodeAddress(node_addr);

		  delay(500);

			return 0;

	}
	//----------------------------------------------------------------------------------------------

	//---------------------------------	Send Message via Serial ------------------------------------
	void WeatherComunication::sendMessageSerial(WeatherRecord wr)
	{
		Serial.println();
		Serial.println(F("|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"));
		Serial.println(F(" ==> Send Period Reached!!! Weather values average <=="));
		Serial.println();
		Serial.print(F(" Temperature: "));                     Serial.print(wr.getTemperature());		Serial.print(F("ºC "));
		Serial.print(F(" Humidity: "));                     Serial.print(wr.getHumidity());	Serial.print(F("% "));
		Serial.print(F(" Pressure: "));                     Serial.print(wr.getPressure());	Serial.println(F("KPa "));
		Serial.print(F(" Rain: "));           Serial.print(wr.getAmountRain());	Serial.println(F("mm "));
		Serial.print(F(" Wind Speed: "));        Serial.print(wr.getWindSpeed());	Serial.print(F("km/h "));
		Serial.print(F(" Wind Gust: "));        Serial.print(wr.getWindGust());	Serial.print(F("km/h "));
		Serial.print(F(" Wind Direction: "));        Serial.print(wr.getWindDirection());	Serial.println(F("º "));
		Serial.print(F(" Battery Voltage: " ));       		 			Serial.print(wr.getBatteryVoltage());	Serial.print(F("V "));
			#ifdef ENABLE_INA
			Serial.print(F(" Battery Voltages (from INA): " ));       		 			Serial.print(wr.getBatteryINA());	Serial.print(F("V "));
			Serial.print(F(" Maximum Current  (from INA):" ));       	 		Serial.print(wr.getMaxCurrent());		Serial.print(F("mA "));
			Serial.print(F(" Minimum Current  (from INA):" ));       	 		Serial.print(wr.getMinCurrent());		Serial.print(F("mA "));
			#endif
			// #ifdef THINGSPEAK
			Serial.println();
			Serial.print(F(" ThingSpeak Channel: " ));       		 			Serial.print(ts_channel);
			// #endif

		Serial.println();
		Serial.println(F("|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"));
		Serial.println();

		Serial.flush();

	}
	//----------------------------------------------------------------------------------------------

	//---------------------------------	Send Message via LoRa --------------------------------------
	void WeatherComunication::sendMessageLoRa(WeatherRecord wr)
	{
	  	// Aux variables to define the message
			uint8_t r_size;
	  	char auxBuf[20];
	  	uint8_t data[150];

			sx.setPacketType(PKT_TYPE_DATA);


			r_size = sprintf((char*)data , "\\!77LG70JEWEMGKB8M#1#TP/%s/", dtostrf(wr.getTemperature(), 2, 2, auxBuf));
			r_size += sprintf((char*)data + r_size, "HD/%s/", dtostrf(wr.getHumidity(), 2, 2, auxBuf));
			r_size += sprintf((char*)data + r_size, "PA/%s/", dtostrf(wr.getPressure(), 6, 2, auxBuf));
			r_size += sprintf((char*)data + r_size, "WD/%s/", dtostrf(wr.getWindDirection(), 3, 2, auxBuf));
			r_size += sprintf((char*)data + r_size, "WS/%s/", dtostrf(wr.getWindSpeed(), 3, 2, auxBuf));
			r_size += sprintf((char*)data + r_size, "WG/%s/", dtostrf(wr.getWindGust(), 3, 2, auxBuf));
			r_size += sprintf((char*)data + r_size, "RA/%s/", dtostrf(wr.getAmountRain(), 2, 2, auxBuf));
			r_size += sprintf((char*)data + r_size, "BV/%s/", dtostrf(wr.getBatteryVoltage(), 2, 2, auxBuf));
			// 	#ifdef ENABLE_INA
			// 	r_size += sprintf((char*)data + r_size, "BINA;%s;", dtostrf(wr.getBatteryINA(), 2, 2, auxBuf));
			// 	r_size += sprintf((char*)data + r_size, "minC;%s;", dtostrf(wr.getMinCurrent(), 2, 2, auxBuf));
			// 	r_size += sprintf((char*)data + r_size, "maxC;%s;", dtostrf(wr.getMaxCurrent(), 2, 2, auxBuf));
			// 	#endif
			// 	// #ifdef THINGSPEAK
			// 	r_size += sprintf((char*)data + r_size, "TS;%s;", ts_channel);
			// 	// #endif

			sx.sendPacketTimeout(DEFAULT_DEST_ADDR, data, r_size);


				// cont++;
				//
				// r_size = sprintf((char*)data, "WT123 - %d;", cont);
		  	// r_size += sprintf((char*)data + r_size, "TP;%s;", dtostrf(wr.getTemperature(), 2, 2, auxBuf));
		  	// r_size += sprintf((char*)data + r_size, "HD;%s;", dtostrf(wr.getHumidity(), 2, 2, auxBuf));
		  	// r_size += sprintf((char*)data + r_size, "PA;%s;", dtostrf(wr.getPressure(), 6, 2, auxBuf));
		  	// r_size += sprintf((char*)data + r_size, "WD;%s;", dtostrf(wr.getWindDirection(), 3, 2, auxBuf));
		  	// r_size += sprintf((char*)data + r_size, "WS;%s;", dtostrf(wr.getWindSpeed(), 3, 2, auxBuf));
		  	// r_size += sprintf((char*)data + r_size, "WG;%s;", dtostrf(wr.getWindGust(), 3, 2, auxBuf));
		  	// r_size += sprintf((char*)data + r_size, "RA;%s;", dtostrf(wr.getAmountRain(), 2, 2, auxBuf));
		  	// r_size += sprintf((char*)data + r_size, "BV;%s;", dtostrf(wr.getBatteryVoltage(), 2, 2, auxBuf));
				// 	#ifdef ENABLE_INA
				// 	r_size += sprintf((char*)data + r_size, "BINA;%s;", dtostrf(wr.getBatteryINA(), 2, 2, auxBuf));
			  // 	r_size += sprintf((char*)data + r_size, "minC;%s;", dtostrf(wr.getMinCurrent(), 2, 2, auxBuf));
			  // 	r_size += sprintf((char*)data + r_size, "maxC;%s;", dtostrf(wr.getMaxCurrent(), 2, 2, auxBuf));
				// 	#endif
				// 	// #ifdef THINGSPEAK
				// 	r_size += sprintf((char*)data + r_size, "TS;%s;", ts_channel);
				// 	// #endif
				//
				// // Send message via LoRa
				// rf95.send(data, r_size);
				// rf95.waitPacketSent();
				//
				// delay(50);
				// rf95.sleep();
	}
	//----------------------------------------------------------------------------------------------
