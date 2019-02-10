#include "mbed.h"
#include "humiditysensor.h"
#include <iostream>

AnalogIn humidityVoltage(PA_5);

const float tempCelsius = 25; //Set by default to room temperature

Serial pc(SERIAL_TX, SERIAL_RX);

int main() {
    double output;
    while(1){
        output = readHumidityTempAdjusted(25,humidityVoltage);
        wait(1);
        
        //Printouts for the data collected
        pc.printf("Sensor Data: %f \n", output);
        //pc.printf("Adjusted for Temp: %f \n", trueRH);
        //pc.printf("Raw Voltage: %f \n",humidityVoltage.read());        
    }
}