#include "WeatherStation.h"


//Initialize WeatherStation
WeatherStation::WeatherStation(int wind_speed, int rain, int wind_dir, int period_time)
:
wc(),
sr(rain),
sw(wind_speed, WDIR_CALIBRATION, wind_dir)
{
	period=period_time;
	minutes = 0;
	temperature = 0;
	humidity = 0;
	pressure = 0;
	windDirection = 0;
	windGust = 0;
	windSpeed = 0;
	amountRain = 0;
	batteryVoltage = 0;
	time_before=0;
	time_after=0;

	temperatures = new float[period_time];
	humidities = new float[period_time];
	pressures = new float[period_time];
	voltages = new float[period_time];
}



	//---------------------------------	Inicialize Weather Station --------------------------------------
	void WeatherStation::init()
	{
		#ifdef ENABLE_DEBUG
		Serial.begin(BAUDRATE);

		unsigned long before = millis();

		while (!Serial)
		{
			unsigned long after = millis();

			if( (after-before)>5000 )
				break;
  	}

		Serial.println(F("------------------------------------------------------------"));
		Serial.println(F("-                  LoRa Weather Station                    -"));
		Serial.println(F("-                                                          -"));
		Serial.println(F("- Copyright (C) UNPARALLEL Innovation, Lda                 -"));
		Serial.println(F("- http://www.unparallel.pt                                 -"));
		Serial.println(F("------------------------------------------------------------"));
		#endif


		Wire.begin();        // Initiate the Wire library and join the I2C bus

		analogReference(AR_DEFAULT);

		// Initiate the sensors
		if( begin()!=0)
		{
		  #ifdef ENABLE_DEBUG
		    Serial.println();
		    Serial.println(F("Weather Station Sensors Erro!"));
				Serial.println(F("------------------------------------------------------------"));
		  #endif
		}
		else
		{
		  #ifdef ENABLE_DEBUG
		    Serial.println();
		    Serial.println(F("Weather Station Sensors Found!"));
				Serial.println(F("------------------------------------------------------------"));
		  #endif
		}

		interrupts(); 				// Turn on interrupts
		sw.attachWindInterrupt();   // Attach Wind Interrupt
		sr.attachRainInterrupt();   // Attach Rain Interrupt

		clearValues();        // Clear variables, counters
	}
	//---------------------------------------------------------------------------------------------------


	//---------------------------------- To begin RTC & config LoRa -------------------------------------
	int WeatherStation::begin()
	{
		  #ifdef ENABLE_DEBUG
				Serial.println(F("------------------------------------------------------------"));
	      Serial.println(F("Inicialize Weather Station Sensors:"));
				Serial.println();
		  #endif

		  // RTC Sensor
		  if(rtc.begin()!=0)
		  {
		  	#ifdef ENABLE_DEBUG
		    Serial.println(F("RTC Sensor Not Found!"));
		    #endif
		    return 1;
		  }
		  else
		  {
		  	#ifdef ENABLE_DEBUG
		    Serial.println(F("RTC Sensor Found!"));
		    #endif
		    rtc.configRTC(F(__DATE__),F(__TIME__));    // Configure RTC
		  }

			// LoRa module
		  if( wc.configLoRa()!=0)
		  {
		    #ifdef ENABLE_DEBUG
		      Serial.println(F("LoRa module config Erro!"));
		    #endif
		    return 1;
		  }
		  else
		  {
		    #ifdef ENABLE_DEBUG
		      Serial.println(F("LoRa module config with Success!"));
		    #endif
		  }

			// To inicialize the sensors
			if( beginSensors()!=0)
				return 1;

		return 0;
	}
	//---------------------------------------------------------------------------------------------------


	//----------------------- To begin the Weather Station mesurement sensors ---------------------------
	int WeatherStation::beginSensors()
	{
			// Pressure Sensor
			sp.begin();

			// Temperature Sensor
			if(st.begin()!=0)
			{
				#ifdef ENABLE_DEBUG
				Serial.println(F("Temperature Sensor Not Found!"));
				#endif
				return 1;
			}
			else
			{
				#ifdef ENABLE_DEBUG
				Serial.println(F("Temperature Sensor Found!"));
				#endif
			}


			// Humidity Sensor
			if(sh.begin()!=0)
			{
				#ifdef ENABLE_DEBUG
				Serial.println(F("Humidity Sensor Not Found!"));
				#endif
				return 1;
			}
			else
			{
				#ifdef ENABLE_DEBUG
				Serial.println(F("Humidity Sensor Found!"));
				#endif
			}

			return 0;
	}
	//---------------------------------------------------------------------------------------------------


	//----------------------- Clear Weather Station Interrupts, Couters & Values ------------------------
	void WeatherStation::clearValues()
	{
		  temperature = 0;
		  humidity = 0;
		  pressure = 0;
		  windDirection = 0;
		  windGust=0;
		  windSpeed = 0;
		  amountRain = 0;
		  batteryVoltage=0;
			minutes=0;

		  for (int i=0; i<period; i++)
		  {
		    temperatures[i]=0;
		    humidities[i]=0;
		    pressures[i]=0;
		    voltages[i]=0;
		  }

		  sw.clearWind();
		  sr.clearRain();

		  rtc.clearRTCAlarm();
	}
	//---------------------------------------------------------------------------------------------------


	//---------------------- Get the Weather measurements for each minute -------------------------------
	void WeatherStation::getWeatherValues()
	{
	  temperatures[minutes-1] = st.getTemperature(); 	// Temperature (Cº)
	  humidities[minutes-1] = sh.getHumidity();      	// Humidity (%)
		pressures[minutes-1] = sp.getPressureKPa();   	// Pressure (Pa)
		voltages[minutes-1] = getBatteryVoltage(); 	// Battery Voltage (V)
	  int windIndice = sw.incrementWindIndice();		// Increment Wind Indice

	  rtc.clearRTCAlarm();    // Clear RTC Alarm control

	  #ifdef ENABLE_DEBUG
				Serial.println();
				Serial.println(F("-----------------------------------------------------------------------------------------------------------"));
	      Serial.print(F(" ==> Weather values for minute "));     Serial.print(minutes);	Serial.println(F(" <=="));
				Serial.println();
				Serial.print(F(" Temperature: "));                     Serial.print(temperatures[minutes-1]);		Serial.print(F("ºC "));
	      Serial.print(F(" Humidity: "));                     Serial.print(humidities[minutes-1]);	Serial.print(F("% "));
	      Serial.print(F(" Pressure: "));                     Serial.print(pressures[minutes-1]);	Serial.println(F("KPa "));
	      Serial.print(F(" Rain: "));           Serial.print(sr.getRain());	Serial.println(F("mm "));
	      Serial.print(F(" Wind Speed: "));        Serial.print(sw.getWindSpeed(minutes));	Serial.print(F("km/h "));
				Serial.print(F(" Wind Gust: "));        Serial.print(sw.getWindGust());	Serial.print(F("km/h "));
				Serial.print(F(" Wind Direction: "));        Serial.print(sw.getWindDirection());	Serial.println(F("º "));
	      Serial.print(F(" Battery Voltage: " ));       		 			Serial.print(voltages[minutes-1]);	Serial.print(F("V "));
				Serial.println();
	      Serial.println(F("-----------------------------------------------------------------------------------------------------------"));
	  #endif

	}
	//---------------------------------------------------------------------------------------------------


	//------------------------- Get the Weather measurements for the Hour -------------------------------
	void WeatherStation::getWeatherAverage()
	{
		  // Get each minute weather measures --------------------------
		  for (int i=0; i<minutes; i++)
		  {
		    temperature += temperatures[i];
		    humidity += humidities[i];
		    pressure += pressures[i];
		    batteryVoltage += voltages[i];
		  }

		  windDirection = sw.getWindDirection();  // Wind Direction (º)
		  windSpeed = sw.getWindSpeed(minutes);   // Wind Speed (km/h)
		  windGust = sw.getWindGust();   // Wind Gust (km/hh)
		  amountRain = sr.getRain();        // Rain (mm)


		  // Set measures to Weather Record object
		  wr.setTemperature(temperature/minutes); // Temperature (Cº)
		  wr.setHumidity(humidity/minutes);       // Humidity (%)
		  wr.setPressure(pressure/minutes);       // Pressure (Pa)
		  wr.setWindDirection(windDirection);     // Wind Direction (º)
		  wr.setWindSpeed(windSpeed);             // Wind Speed (kPh)
		  wr.setWindClicks(sw.getWindClicks());                 // Wind Clicks (interrupts)
		  wr.setAmountRain(amountRain);           // Amount of Rain (mm)
		  wr.setRainClicks(sr.getRainClicks());                  // Rain Clicks (interrupts)
		  wr.setWindGust(windGust);               // Wind Gust (kPh)
		  wr.setBatteryVoltage(batteryVoltage/minutes);   // Battery Voltage (V)
	}
	//---------------------------------------------------------------------------------------------------


	//------------------------------ Weather Station Operation Task -------------------------------------
	void WeatherStation::task()
	{
		// For each RTC Alarm
		  if (rtc.getRTCAlarm()==1)
		  {

		    minutes++;

		    getWeatherValues();   // For each minute get the respective minute weather measures

		      // For each hour: get weather averages, send mensage and clear the values
		      if (minutes==period)
		      {
		          getWeatherAverage();   // Get weather measure averages

							#ifdef ENABLE_DEBUG
		          	wc.sendMessageSerial(wr);         // Send message
							#endif
		          wc.sendMessageLoRa(wr);

		          clearValues();         // Clear variables, counters
		      }
		  }

			// Wind Direction Tendency
			time_after=millis();
			if ((time_after-time_before) > 500)
			{
				time_before=millis();
				int windIndice = sw.incrementWindIndice();
			}


  		#ifdef ENABLE_DEBUG
		  // for testing
			  if(sw.getWindControl()==1)
			  {
						Serial.println();
						Serial.println(F("++++++++++++++++++++++++++++++ Wind Interrupt ++++++++++++++++++++++++++++++"));
				    Serial.print(F(" WindClicks (in period): "));     Serial.print(sw.getWindClicks());	Serial.println();
						Serial.print(F(" WindDirection: "));    Serial.print(sw.getWindDirection());	Serial.print(F("º "));
						Serial.print(F(" Last WindGust: "));    Serial.print(sw.getGustInterval());	Serial.print(F("ms "));
				    windGust=sw.getWindGust();
				    Serial.print(F(" Actual WindGust: "));     	Serial.print(windGust);	Serial.println(F("ms "));
						Serial.println(F("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"));

				    sw.clearWindControl();
			   }

				 if(sr.getRainControl()==1)
			   {
	 						Serial.println();
	 						Serial.println(F("########## Rain Interrupt ##########"));
	 				    Serial.print(F(" RainClicks (in period): "));    Serial.print(sr.getRainClicks());  Serial.println();
							Serial.print(F(" Rain: "));  Serial.print(sr.getRain()); Serial.println(F("mm "));
							Serial.println(F("####################################"));

				   sr.clearRainControl();
			   }
		  #endif
	}
	//---------------------------------------------------------------------------------------------------


	//------------------------------------ Get Battery Voltage ------------------------------------------
	float WeatherStation::getBatteryVoltage()
	{
		float measuredvbat = analogRead(VBATPIN);

		measuredvbat *= 2;    // we divided by 2, so multiply back
		measuredvbat *= 3.2;  // Multiply by 3.3V, our reference voltage
		measuredvbat /= 1024; // convert to voltage

		return measuredvbat;
	}
	//---------------------------------------------------------------------------------------------------
