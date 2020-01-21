
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
    uint16_t ID;
    uint8_t CONTROL;
    uint16_t COMMAND;
};

#define TELEMETRY_SIZE 17
#define CONTROL_SIZE 7

// Global Telemetry struct
struct Telemetry current_telemetry;

// Global telemetry packet
uint8_t telemetry_packet[TELEMETRY_SIZE];

//Global Control struct
struct Control current_control;

// Global Control packet
uint8_t control_packet[CONTROL_SIZE];



void I2C_DriverTransmit(I2C_HandleTypeDef * hi2c, struct Telemetry telemetry_struct);
void I2C_DriverRead(I2C_HandleTypeDef * hi2c);
void control_packet_transform(void);
void telemetry_packet_transform(void);
