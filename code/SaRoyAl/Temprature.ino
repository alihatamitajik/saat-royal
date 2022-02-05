#define TEMP D7

////Setup Temprature Sensor
#include <OneWire.h>
#include <DallasTemperature.h>
// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(TEMP);
// Pass oneWire reference to DallasTemperature library
DallasTemperature tempSensor(&oneWire);

void initTEMP() {
  //Start Temprature Library
  tempSensor.begin(); // Start up the library
}

int8_t getTemprature() {
  // Returns the temprature in degree Celcius
  return tempSensor.getTempCByIndex(0);
}
