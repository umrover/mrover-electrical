// in main: enable address listen then use the i2c callback function to call functions in here
// in this file: call the HAL i2c Slave (recieve) and Slave (transmit IT) functions and convert into the array and struct forms


#include "i2c_drivernucl.h"
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
    HAL_I2C_Slave_Transmit_IT(hi2c, &telemetry_packet, TELEMETRY_SIZE);
}

// This function reads Control packet from the Beaglebone into a control struct for the nucleo to use
void I2C_DriverRead(I2C_HandleTypeDef * hi2c)
{
    // Receive Control packet using HAL i2c Slave Recieve
    HAL_I2C_Slave_Receive_IT(hi2c, &control_packet, CONTROL_SIZE);
}


// Helper functions to format the telemetry packet into an array of bytes to be sent out
void telemetry_packet_transform(void)
{
    uint32_t velocity = float_to_fxp_10_22(current_telemetry.VELOCITY);
    telemetry_packet[0] = START_DELIMITER;
    telemetry_packet[1] = (current_telemetry.ID & 0xFF00) >> 8;
    telemetry_packet[2] = (current_telemetry.ID & 0x00FF);
    telemetry_packet[3] = (current_telemetry.ENCODER & 0xFF00) >> 8;
    telemetry_packet[4] = (current_telemetry.ENCODER & 0x00FF);
    telemetry_packet[5] = (current_telemetry.ISENA & 0xFF00) >> 8;
    telemetry_packet[6] = (current_telemetry.ISENA & 0x00FF);
    telemetry_packet[7] = (current_telemetry.ISENB & 0xFF00) >> 8;
    telemetry_packet[8] = (current_telemetry.ISENB & 0x00FF);
    telemetry_packet[9] = (current_telemetry.ISENC & 0xFF00) >> 8;
    telemetry_packet[10] = (current_telemetry.ISENC & 0x00FF);
    telemetry_packet[11] = (velocity & 0xFF000000) >> 24;
    telemetry_packet[12] = (velocity & 0x00FF0000) >> 16;
    telemetry_packet[13] = (velocity & 0x0000FF00) >> 8;
    telemetry_packet[14] = (velocity & 0x000000FF);
    telemetry_packet[15] = (current_telemetry.LIMIT_SWITCH);
    telemetry_packet[16] = END_DELIMITER;
}


// Helper function to take in an array of bytes received into the control struct
void control_packet_transform(void)
{
    current_control.ID = control_packet[1] << 8 | control_packet[2];
    current_control.CONTROL = control_packet[3];
    current_control.COMMAND = control_packet[4] << 8 | control_packet[5];

}




//  def float_to_fxp_10_22(val, signed=False):
//      CONVERSION_CONST = float(0x400000)
//      return int(val * CONVERSION_CONST)

// Helper function to convert a float into a fixed point uint32
uint32_t float_to_fxp_10_22(float val)
{
    float CONVERSION_CONST = 0x400000;
    return (uint32_t)(val * CONVERSION_CONST);
}
