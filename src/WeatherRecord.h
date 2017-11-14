#ifndef WEATHER_RECORD
#define WEATHER_RECORD

#include "Arduino.h"


class WeatherRecord
{
	// Public Variables/Functions
	public:
	WeatherRecord();
	void setTemperature(float value);			// Set Weather Record Temperature (ºC)
	float getTemperature();						// Return Weather Record Temperature (ºC)
	void setHumidity(float value);				// Set Weather Record Humidity (%)
	float getHumidity();						// Return Weather Record Humidity (%)
	void setPressure(float value);				// Set Weather Record Pressure (kPa)
	float getPressure();						// Return Weather Record Pressure (kPa)
	void setWindSpeed(float value);				// Set Weather Record Wind Speed (kPh)
	float getWindSpeed();						// Return Weather Record Wind Speed (kPh)
	void setWindDirection(float value);			// Set Weather Record Wind Direction (º)
	float getWindDirection();					// Return Weather Record Wind Direction (º)
	void setWindGust(float value);				// Set Weather Record Wind Gust (kPh)
	float getWindGust();						// Return Weather Record Wind Gust (kPh)
	void setAmountRain(float value);			// Set Weather Record Amount of Rain (mm)
	float getAmountRain();						// Return Weather Record Amount of Rain (mm)
	void setBatteryVoltage(float value);		// Set Weather Record Battery Voltage (V)
	float getBatteryVoltage();					// Return Weather Record Battery Voltage (V)
	void setRainClicks(unsigned long value);	// Set Weather Record Rain Clicks (nº of interrupts)
	unsigned long getRainClicks();				// Return Weather Record Rain Clicks (nº of interrupts)
	void setWindClicks(unsigned long value);	// Set Weather Record Wind Clicks (nº of interrupts)
	unsigned long getWindClicks();				// Return Weather Record Rain Clicks (nº of interrupts)

	// Private Variables/Functions
	private:
	float temperature;
	float humidity;
	float pressure;
	float windDirection;
	float windGust;
	float windSpeed;
	float amountRain;
	float battery;
	unsigned long rainClicks;
	unsigned long windClicks;
};

#endif
