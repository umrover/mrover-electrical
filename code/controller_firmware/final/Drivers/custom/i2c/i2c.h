
#ifndef I2C_H_
#define	I2C_H_

#include <stdint.h>
#include "main.h"


#define START_DELIMITER 0xFF
#define END_DELIMITER 0xAA

struct Telemetry{
    uint16_t ID;
    uint16_t ENCODER;
    uint16_t ISENA;
    uint16_t ISENB;
    uint16_t ISENC;
    float VELOCITY;
    uint8_t LIMIT_SWITCH;
};

struct Control{
    uint16_t ADDR;
    uint8_t CONTROL_MODE;
    int16_t SETPOINT;
    uint8_t ENABLE;
};

#define TELEMETRY_SIZE 17
#define CONTROL_SIZE 6

// I2C data buffers
uint8_t telemetryBuffer[TELEMETRY_SIZE];
uint8_t controlBuffer[CONTROL_SIZE];

// Global Telemetry struct
struct Telemetry currentTelemetry;

//Global Control struct
struct Control currentControl;



void I2C_DriverTransmit(I2C_HandleTypeDef * hi2c, struct Telemetry telemetry_struct);
void I2C_DriverRead(I2C_HandleTypeDef * hi2c);
void control_packet_transform(void);
void telemetry_packet_transform(void);

#endif	/* I2C_H_*/
