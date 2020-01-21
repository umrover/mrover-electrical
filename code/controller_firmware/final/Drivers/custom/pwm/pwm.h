#ifndef _PWM_H
#define _PWM_H

#include <stdint.h>
#include "stm32f3xx_hal.h"
#include "drv8353rx.h"

#define PWM_PERIOD 6400
#define COAST_DEAD_TIME 1000

typedef enum {
    FORWARD,
    REVERSE
} motor_dir_t;

TIM_HandleTypeDef htim;

static uint32_t deviceControlMode;
static motor_dir_t motorDir;

/*** Public Functions ***/
/* Initialize timer hardware for PWM */
void PWM_init(uint32_t controlMode);

/* Motor Drive abstraction function */
void PWM_driveMotor(float throttle);


/*** Private helper functions ***/
void PWM_enableOutput();

void PWM_enableCounter();

void PWM_enableChannel(uint32_t channel);

void PWM_enableChannelN(uint32_t channel);

void PWM_disableChannel(uint32_t channel);

void PWM_disableChannelN(uint32_t channel);


void PWM_setTrapezoidalDir(motor_dir_t dir);

void PWM_setTrapezoidalCoast(uint8_t enable);

/**
 * @brief Set output duty cycle on a PWM channel
 * @param dutyCycle: 0-1 duty cycle percentage
 */
void PWM_setDutyCycle(uint32_t channel, float dutyCycle);

/* Configure PWM outputs according to motor control algorithm */
void PWM_configOutputs(uint32_t controlMode);

#endif
