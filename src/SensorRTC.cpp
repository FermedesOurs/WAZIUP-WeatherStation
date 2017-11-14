
#include "SensorRTC.h"


//Initialize SensorRTC
SensorRTC::SensorRTC()
{
	alarme=0;
}

	//----------------------------------------- Begin RTC -------------------------------------------
	int SensorRTC::begin()
	{
		#ifdef USE_RCTInt
			rtc.begin(TIME_H24);      //RTC initializing with 24 hour representation mode
			return 0;
		#else
			return 1;
		#endif
	}
	//----------------------------------------------------------------------------------------------

	//---------------------------------------- Config RTC ------------------------------------------
	void SensorRTC::configRTC(const __FlashStringHelper* date, const __FlashStringHelper* times)
	{
		#ifdef USE_RCTInt

			//Get system Date and Time
			char buff[11];
			uint8_t y, m, d, hh, mm, ss;

			memcpy_P(buff, date, 11);
			y = conv2d(buff + 9);
			// Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
			switch (buff[0])
			{
					case 'J': m = buff[1] == 'a' ? 1 : m = buff[2] == 'n' ? 6 : 7; break;
					case 'F': m = 2; break;
					case 'A': m = buff[2] == 'r' ? 4 : 8; break;
					case 'M': m = buff[2] == 'r' ? 3 : 5; break;
					case 'S': m = 9; break;
					case 'O': m = 10; break;
					case 'N': m = 11; break;
					case 'D': m = 12; break;
			}
			d = conv2d(buff + 4);
			memcpy_P(buff, times, 8);
			hh = conv2d(buff);
			mm = conv2d(buff + 3);
			ss = conv2d(buff + 6);

			// Config RTC
			rtc.setTime(hh,0,mm,ss);     //setting time (hour, meridian, minutes and seconds)

			rtc.setDate(d,m,y);     //setting date

			rtc.enableAlarm(SEC, ALARM_INTERRUPT, isr2); //enabling alarm and match on second
			SensorRTC_instance = this;

			rtc.local_time.second=59;  //setting second to match

			rtc.setAlarm();  //write second in alarm register
		#endif
	}
	//----------------------------------------------------------------------------------------------


#ifdef USE_RCTInt
	//------------------------------ RTC Alarm Interrupt Routine -----------------------------------
	void SensorRTC::ISR_RTC()
	{
		  alarme=1;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------- Auxiliar RTC Interrupt ISR -------------------------------------
	void SensorRTC::isr2()
	{
		SensorRTC_instance->ISR_RTC();
	}
	//----------------------------------------------------------------------------------------------


	SensorRTC * SensorRTC::SensorRTC_instance;		// Auxiliar instance to RTC Interrupt ISR


	//---------------------------------- Clear RTC alarme state ------------------------------------
	void SensorRTC::clearRTCAlarm()
	{
		alarme=0;
	}
	//----------------------------------------------------------------------------------------------

	//------------------------------------ Get RTC alarme state ------------------------------------
	int SensorRTC::getRTCAlarm()
	{
		return alarme;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------
	uint8_t SensorRTC::conv2d(const char* p)
	{
		uint8_t v = 0;
		if ('0' <= *p && *p <= '9')
				v = *p - '0';
		return 10 * v + *++p - '0';
	}
	//----------------------------------------------------------------------------------------------
#endif
