#include "mbed.h"
#include "humiditysensor.h"


// Assumes 25C and make no adjustments
double readHumidity(AnalogIn humidityVoltage) {
    return (double)((((humidityVoltage.read())*3.3/5) - 0.16)/0.0062);
}

//returns the humidity given a temperature
double readHumidityTempAdjusted(double tempCelsius, AnalogIn humidityVoltage){
    return (double)((readHumidity(humidityVoltage))/(1.0546 - (0.00216*tempCelsius)));
}  