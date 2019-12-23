#include "mbed.h"


// Assumes 25C and make no adjustments
//Inputs 
//humidityVoltage: The anaolog ADC pin assigned for the sensor
double readHumidity(AnalogIn humidityVoltage);

//returns the humidity given a temperature
//Inputs 
//humidityVoltage: The anaolog ADC pin assigned for the sensor
//tempCelsius: the temperature in celsius
double readHumidityTempAdjusted(double tempCelsius, AnalogIn humidityVoltage);  