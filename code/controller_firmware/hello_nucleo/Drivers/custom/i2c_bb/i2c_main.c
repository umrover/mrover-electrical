
#include "i2c_drivernucl.h"

// Enable Address listen

// Do i2c callback
    // within callback
    // read in control packet
    // transmit telemetry



// PUT THE CODE BELOW ABOVE MAIN

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef * hi2c) {
	__HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_ADDR);
	HAL_I2C_EnableListen_IT(hi2c);
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef * hi2c) {
	control_packet_transform();
	__HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_ADDR);
	HAL_I2C_EnableListen_IT(hi2c);
}


void HAL_I2C_AddrCallback(I2C_HandleTypeDef * hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
	HAL_I2C_DisableListen_IT(hi2c);

	if (TransferDirection == I2C_DIRECTION_TRANSMIT)
	{
        I2C_DriverRead(hi2c);
	}
	else
	{
        I2C_DriverTransmit(hi2c, current_telemetry);
	}

	HAL_I2C_EnableListen_IT(hi2c);
}


// PUT THE CODE BELOW INSIDE MAIN (barring the actual void main() declaration)

void main(){
    MX_I2C1_Init();
    HAL_I2C_EnableListen_IT(&hi2c1);
}