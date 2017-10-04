//---------------------------------------------------------------------------------------------------------------
// LoRa Weather Station
//
// This is a code to work with the Waziup Weather Station solution:
// <https://iot-catalogue.unparallel.pt/unparallel/#iotSolution/59b1797c763cfc066f6d092b>
//
// Copyright (C) UNPARALLEL Innovation, Lda <http://www.unparallel.pt>
// September 2017
//---------------------------------------------------------------------------------------------------------------

// Uncomment to Debug
#define ENABLE_DEBUG

// Send Period (in minutes)
#define PERIOD 2

// Define LoRa module Frequency
#define RF95_FREQ 868.0


// Weather Station pins to Adafruit Feather M0
#define RAIN 11
#define WSPEED 12
#define WDIR A0

// Include and create Weather Station object.
#include <WeatherStation.h>

WeatherStation ws(WSPEED, RAIN, WDIR, PERIOD);


// Setup
void setup()
{
  ws.init();
}

// Loop
void loop()
{
  ws.task();
}
