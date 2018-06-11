//---------------------------------------------------------------------------------------------------------------
// LoRa Weather Station
//
// This is a code to work with the Waziup Weather Station solution:
// <https://www.iot-catalogue.com/products/59b1797c763cfc066f6d092b>
//
// Note: Without using the Waziup Weather Station PCB ensure that
// the cable connections between Weather Shield and Feather M0 match with
// the indicated in Annex B of the Assembly Guide.
//
// Copyright © UNPARALLEL Innovation, Lda <http://www.unparallel.pt>
// June 2018
//---------------------------------------------------------------------------------------------------------------


// Send Period (in minutes)
#define PERIOD 10

// Include and create Weather Station object.
#include <WeatherStation.h>
WeatherStation ws(PERIOD);


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
