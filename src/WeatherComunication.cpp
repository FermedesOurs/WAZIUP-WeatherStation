
#include "WeatherComunication.h"


//Initialize WeatherComunication
WeatherComunication::WeatherComunication()
:sx()	// Singleton instance of the radio driver object
{
}

	//----------------------------------------- Config LoRa ----------------------------------------
	int WeatherComunication::configLoRa()
	{
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

			r_size = sprintf((char*)data , "\\!");
			// "SGSH52UGPVAUYG3S" is the defaulf Thing Speak API Key. Changed to you ThingSpeak API Key or Comment and the Gateway will define the default key
			//r_size += sprintf((char*)data + r_size, "SGSH52UGPVAUYG3S");
			r_size += sprintf((char*)data + r_size, "#1#TP/%s/", dtostrf(wr.getTemperature(), 2, 2, auxBuf));
			r_size += sprintf((char*)data + r_size, "HD/%s/", dtostrf(wr.getHumidity(), 2, 2, auxBuf));
			r_size += sprintf((char*)data + r_size, "PA/%s/", dtostrf(wr.getPressure(), 6, 2, auxBuf));
			r_size += sprintf((char*)data + r_size, "WD/%s/", dtostrf(wr.getWindDirection(), 3, 2, auxBuf));
			r_size += sprintf((char*)data + r_size, "WS/%s/", dtostrf(wr.getWindSpeed(), 3, 2, auxBuf));
			r_size += sprintf((char*)data + r_size, "WG/%s/", dtostrf(wr.getWindGust(), 3, 2, auxBuf));
			r_size += sprintf((char*)data + r_size, "RA/%s/", dtostrf(wr.getAmountRain(), 2, 2, auxBuf));
			r_size += sprintf((char*)data + r_size, "BV/%s", dtostrf(wr.getBatteryVoltage(), 2, 2, auxBuf));

			sx.sendPacketTimeout(DEFAULT_DEST_ADDR, data, r_size);
	}
	//----------------------------------------------------------------------------------------------
