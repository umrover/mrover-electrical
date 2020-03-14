#include "pwm.h"

/*** Public Functions ***/
/* Initialize timer hardware for PWM */

struct FOC_THROTTLE {
	float Phase_A;
	float Phase_B;
	float Phase_C;
};

void PWM_init(uint32_t controlMode)
{
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};
    TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    htim.Instance = TIM1;
    htim.Init.Prescaler = 0;
    htim.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1;
    htim.Init.Period = PWM_PERIOD;
    htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim.Init.RepetitionCounter = 0;
    htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&htim) != HAL_OK)
    {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_PWM_Init(&htim) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_SET;
    if (HAL_TIM_PWM_ConfigChannel(&htim, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_PWM_ConfigChannel(&htim, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_PWM_ConfigChannel(&htim, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
    {
        Error_Handler();
    }
    sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_ENABLE;
    sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
    /* CHANGING THE CLOCK CONFIGURATION REQUIRES A RECALC OF DEAD TIME VALUE */
    sBreakDeadTimeConfig.DeadTime = 0x40; // 500ns
    sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
    sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
    sBreakDeadTimeConfig.BreakFilter = 0;
    sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
    sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
    sBreakDeadTimeConfig.Break2Filter = 0;
    sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
    if (HAL_TIMEx_ConfigBreakDeadTime(&htim, &sBreakDeadTimeConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /**TIM1 GPIO Configuration
    PC0     ------> TIM1_CH1 write timer to here
    PB13    ------> TIM1_CH1N this matches the previous one
    PA9     ------> TIM1_CH2
    PB0     ------> TIM1_CH2N
    PA10    ------> TIM1_CH3
    PB1     ------> TIM1_CH3N
    */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // PC0     ------> TIM1_CH1
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    // PB13    ------> TIM1_CH1N
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_TIM1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // PA9     ------> TIM1_CH2
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_TIM1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // PB0     ------> TIM1_CH2N
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_TIM1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // PA10    ------> TIM1_CH3
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_TIM1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // PB1     ------> TIM1_CH3N
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_TIM1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    PWM_configOutputs(controlMode);
}

/* Drive mode helper function */
void PWM_driveMotor(float throttle)
{
    motor_dir_t nextDir = (throttle > 0) ? FORWARD : REVERSE;

    switch(deviceControlMode)
    {
    case DRV_MODE_SENSORED_TRPZ:

        if(nextDir != motorDir)
        {
            // enter coast mode
            PWM_setTrapezoidalCoast(1);

            // update direction pin
            PWM_setTrapezoidalDir(nextDir);

            // update duty cycle
            PWM_setDutyCycle(TIM_CHANNEL_1, fabs(throttle));

            // coasting busy loop
            for(uint32_t i = 0; i < COAST_DEAD_TIME; ++i);

            // exit coast mode
            PWM_setTrapezoidalCoast(0);
        }
        else
        {
            // update duty cycle
            PWM_setDutyCycle(TIM_CHANNEL_1, fabs(throttle));
        }

        break;

    case DRV_MODE_BRUSHED_DC:

        if(nextDir != motorDir)
        {
            uint32_t highSideChannel = (motorDir == FORWARD) ? TIM_CHANNEL_1 : TIM_CHANNEL_2;
            uint32_t lowSideChannel = (motorDir == FORWARD) ? TIM_CHANNEL_2 : TIM_CHANNEL_1;

            // turn off high side
            PWM_disableChannel(highSideChannel);
            // turn on low side
            PWM_setDutyCycle(lowSideChannel, 1);

            // coasting busy loop
            for(uint32_t i = 0; i < COAST_DEAD_TIME; ++i);

            // turn off low side
            PWM_disableChannelN(lowSideChannel);

            // update duty cycles
            PWM_setDutyCycle(highSideChannel, fabs(throttle));
            PWM_setDutyCycle(lowSideChannel, fabs(throttle));

            // turn on opposite high side
            PWM_enableChannel(lowSideChannel);
            // turn on opposite low side
            PWM_enableChannelN(highSideChannel);            
        }
        else
        {
            // update duty cycles
            PWM_setDutyCycle(TIM_CHANNEL_1, fabs(throttle));
            PWM_setDutyCycle(TIM_CHANNEL_2, fabs(throttle));
        }

        break;

    case DRV_MODE_SENSORLESS_FOC:
		 //uint32_t highSideChannel = (motorDir == FORWARD) ? TIM_CHANNEL_1 : TIM_CHANNEL_3; // Where do we put Channel 2?
		 //uint32_t lowSideChannel = (motorDir == FORWARD) ? TIM_CHANNEL_3 : TIM_CHANNEL_1;

		 PWM_enableChannel(TIM_CHANNEL_1);
		 PWM_enableChannel(TIM_CHANNEL_2);
		 PWM_enableChannel(TIM_CHANNEL_3);

		 PWM_setDutyCycle(TIM_CHANNEL_1, fabs(FOC_THROTTLE.Phase_A));
		 PWM_setDutyCycle(TIM_CHANNEL_2, fabs(FOC_THROTTLE.Phase_B));
		 PWM_setDutyCycle(TIM_CHANNEL_3, fabs(FOC_THROTTLE.Phase_C));


		 /*
		 // turn off high side
		 PWM_disableChannel(highSideChannel);
		 // turn on low side
		 PWM_setDutyCycle(lowSideChannel, 1);
		 // coasting busy loop
		 for(uint32_t i = 0; i < COAST_DEAD_TIME; ++i);

		 // turn off low side
		 PWM_disableChannelN(lowSideChannel);

		 // update duty cycles
		 PWM_setDutyCycle(highSideChannel, fabs(throttle));
		 PWM_setDutyCycle(lowSideChannel, fabs(throttle));

		 // turn on opposite high side
		 PWM_enableChannel(lowSideChannel);
		 // turn on opposite low side
		 PWM_enableChannelN(highSideChannel);
*/

    	break;

    default:

        break;
    }

    motorDir = nextDir;
}


/*** Private helper functions ***/
void PWM_enableOutput()
{
    htim.Instance->BDTR |= TIM_BDTR_MOE;
}

void PWM_enableCounter()
{
    htim.Instance->CR1 |= TIM_CR1_CEN;
}

void PWM_enableChannel(uint32_t channel)
{
    htim.Instance->CCER |= (uint32_t)(1 << (channel & 0x1FU));
}

void PWM_enableChannelN(uint32_t channel)
{
    htim.Instance->CCER |= (uint32_t)((1 << 2) << (channel & 0x1FU));
}

void PWM_disableChannel(uint32_t channel)
{
    htim.Instance->CCER &= ~(uint32_t)(1 << (channel & 0x1FU));
}

void PWM_disableChannelN(uint32_t channel)
{
    htim.Instance->CCER &= ~(uint32_t)((1 << 2) << (channel & 0x1FU));
}


void PWM_setTrapezoidalDir(motor_dir_t dir)
{
    GPIO_PinState dirPin = (dir == FORWARD) ? GPIO_PIN_RESET : GPIO_PIN_SET;
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, dirPin);
}

void PWM_setTrapezoidalCoast(uint8_t enable)
{
    GPIO_PinState coast = (enable == 1) ? GPIO_PIN_RESET : GPIO_PIN_SET;
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, coast);
}

/**
 * @brief Set output duty cycle on a PWM channel
 * @param dutyCycle: 0-1 duty cycle percentage
 */
void PWM_setDutyCycle(uint32_t channel, float dutyCycle)
{
    uint16_t dc_counts = (uint16_t)(dutyCycle * PWM_PERIOD);
    if(channel == TIM_CHANNEL_1)
    {
        TIM1->CCR1 = dc_counts;
    }
    else if(channel == TIM_CHANNEL_2)
    {
        TIM1->CCR2 = dc_counts;
    }
    else if(channel == TIM_CHANNEL_3)
    {
        TIM1->CCR3 = dc_counts;
    }
}

/* Configure PWM outputs according to motor control algorithm */
void PWM_configOutputs(uint32_t controlMode)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

    deviceControlMode = controlMode;

    switch(deviceControlMode)
    {
    case DRV_MODE_SENSORED_TRPZ:

        // duty cycle input
        PWM_enableChannel(TIM_CHANNEL_1);

        // reinit CH3 pins as GPIO outputs
        // PA10    ------> Dir
        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        // PB1     ------> Coast
        GPIO_InitStruct.Pin = GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        // disable coast mode
        PWM_setTrapezoidalCoast(0);
        PWM_setTrapezoidalDir(FORWARD);
        motorDir = FORWARD;

        break;

    case DRV_MODE_BRUSHED_DC:

        PWM_enableChannel(TIM_CHANNEL_1);
        PWM_enableChannelN(TIM_CHANNEL_2);

        PWM_disableChannelN(TIM_CHANNEL_1);
        PWM_disableChannel(TIM_CHANNEL_2);

        motorDir = FORWARD;

        break;

    case DRV_MODE_SENSORLESS_FOC:

        PWM_enableChannel(TIM_CHANNEL_1);
        PWM_enableChannelN(TIM_CHANNEL_2);
        PWM_enableChannel(TIM_CHANNEL_3);

        PWM_disableChannelN(TIM_CHANNEL_1);
        PWM_disableChannel(TIM_CHANNEL_2);
        PWM_disableChannelN(TIM_CHANNEL_3);

        motorDir = FORWARD;

    	break;

    default:

        break;
    }

    PWM_enableOutput();
    PWM_enableCounter();
}
