#ifndef SENSOR_PRESSURE
#define SENSOR_PRESSURE

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

// -------- Uncomment the sensor to be used --------
#define USE_MPL3115A2
//--------------------------------------------------


#ifdef USE_MPL3115A2
	#include "SparkFunMPL3115A2.h"
#endif



class SensorPressure
{
	public:
	//Public Variables/Functions
	SensorPressure();
	void begin();			// Begin the Pressure sensor selected (return 0 if ok, return 1 if erro)
	float getPressure();	// Return the Pressure sensor measure (Pa)
	float getPressurePa();	// Return Pressure (Pa)
	float getPressureKPa();	// Return Pressure (kPa)

	//Public Variables/Functions
	private:
	#ifdef USE_MPL3115A2
		MPL3115A2 mpl31;		// Define the sensor object
	#endif
};

#endif
