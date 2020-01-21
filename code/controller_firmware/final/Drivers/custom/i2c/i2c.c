// in main: enable address listen then use the i2c callback function to call functions in here
// in this file: call the HAL i2c Slave (recieve) and Slave (transmit IT) functions and convert into the array and struct forms


#include "i2c.h"
#include "main.h"

void telemetry_packet_transform(void);
void control_packet_transform(void);
uint32_t float_to_fxp_10_22(float val);


//This function transmits Telemetry packet to the Beaglebone, takes in the global struct current_telemetry
void I2C_DriverTransmit(I2C_HandleTypeDef * hi2c, struct Telemetry telemetry_struct)
{
    // Transform Telemetry data into byte size array
	telemetry_packet_transform();

	//I2C Slave Transmit
    HAL_I2C_Slave_Transmit_IT(hi2c, &currentTelemetry, TELEMETRY_SIZE);
}

// This function reads Control packet from the Beaglebone into a control struct for the nucleo to use
void I2C_DriverRead(I2C_HandleTypeDef * hi2c)
{
    // Receive Control packet using HAL i2c Slave Recieve
    HAL_I2C_Slave_Receive_IT(hi2c, &controlBuffer, CONTROL_SIZE);
}


// Helper functions to format the telemetry packet into an array of bytes to be sent out
void telemetry_packet_transform(void)
{
    uint32_t velocity = float_to_fxp_10_22(currentTelemetry.VELOCITY);
    telemetryBuffer[0] = START_DELIMITER;
    telemetryBuffer[1] = (currentTelemetry.ID & 0xFF00) >> 8;
    telemetryBuffer[2] = (currentTelemetry.ID & 0x00FF);
    telemetryBuffer[3] = (currentTelemetry.ENCODER & 0xFF00) >> 8;
    telemetryBuffer[4] = (currentTelemetry.ENCODER & 0x00FF);
    telemetryBuffer[5] = (currentTelemetry.ISENA & 0xFF00) >> 8;
    telemetryBuffer[6] = (currentTelemetry.ISENA & 0x00FF);
    telemetryBuffer[7] = (currentTelemetry.ISENB & 0xFF00) >> 8;
    telemetryBuffer[8] = (currentTelemetry.ISENB & 0x00FF);
    telemetryBuffer[9] = (currentTelemetry.ISENC & 0xFF00) >> 8;
    telemetryBuffer[10] = (currentTelemetry.ISENC & 0x00FF);
    telemetryBuffer[11] = (velocity & 0xFF000000) >> 24;
    telemetryBuffer[12] = (velocity & 0x00FF0000) >> 16;
    telemetryBuffer[13] = (velocity & 0x0000FF00) >> 8;
    telemetryBuffer[14] = (velocity & 0x000000FF);
    telemetryBuffer[15] = (currentTelemetry.LIMIT_SWITCH);
    telemetryBuffer[16] = END_DELIMITER;
}


// Helper function to take in an array of bytes received into the control struct
void control_packet_transform(void)
{
	currentControl.CONTROL_MODE = (controlBuffer[1] & 0x7C) >> 2;
    currentControl.ADDR = ((controlBuffer[1] & 0x3) << 8) | controlBuffer[2];
    currentControl.SETPOINT = (controlBuffer[3] << 8) | controlBuffer[4];
    currentControl.ENABLE = (controlBuffer[1] & 0x80) >> 7;
}


// Helper function to convert a float into a fixed point uint32
uint32_t float_to_fxp_10_22(float val)
{
    float CONVERSION_CONST = 0x400000;
    return (uint32_t)(val * CONVERSION_CONST);
}
