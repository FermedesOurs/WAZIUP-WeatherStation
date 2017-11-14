#ifndef SENSOR_WIND
#define SENSOR_WIND

#include <Arduino.h>



class SensorWind
{
	// Public Variables/Functions
	public:
	SensorWind(int interrupt_pin, double calibration, const uint8_t direction_pin);
	float getWindSpeed(long minutes);													// Return the Wind Speed (kPh)
	int incrementWindIndice();															// Incremente vector wind indice
	int getWindIndice();																// Return the Wind indice
	float getWindDirection();								// Return Wind Direction (º)
	float getWindGust();																// Return Wind Gust (kPh)
	long getWindClicks();																// Return the nº of Wind interrupts
	long getGustInterval();																// Return the minimum time between 2 wind interrutps (ms)
	void attachWindInterrupt();															// Attach the Wind interrupt
	void clearWind();
	void clearWindControl();
	int getWindControl();

	//Private Variables/Functions
	private:
	void ISR_WIND();								// Wind Interrupt Routine
	static void isr0();								// Auxiliar Wind Interrupt ISR
	static SensorWind * SensorWind_instance;		// Auxiliar instance to Wind Interrupt ISR
  double w_calibration; 					// Wind Direction calibration value
	uint8_t w_dir_pin;						// Wind Direction pin
	int w_int_pin;							// Wind interrupt pin
	unsigned long windPos[16];				// Wind indice vector
	volatile unsigned long wind;			// Wind interrupt counter
	volatile unsigned long lastWindIRQ;		// Last Wind check time (ms)
	volatile unsigned long t_min;			// Minimum time between 2 wind interrutps (ms)
	volatile unsigned long t_previous;		// Auxiliar var to t_min
	volatile unsigned long t_interval;		// Auxiliar var to t_min
	volatile unsigned int control;
};

#endif
