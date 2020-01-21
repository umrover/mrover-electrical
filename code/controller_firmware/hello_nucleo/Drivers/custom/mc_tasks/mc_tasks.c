/*
 * mc_tasks.c
 *
 *  Created on: Dec 2, 2019
 *      Author: micha
 */

#include "mc_tasks.h"

void MC_DISABLED_TASK(void)
{
	/* Set PWM to brake mode */


	/* wait for UI command to initiate enable */
}


void MC_STARTUP_TASK(void)
{
	/* Conduct power on self tests */
	if (post_start(&mc_posts) == -1)
	{
		/* Go back to the disabled state */
		FSM_Transition(&fsm, DISABLED);

		/* TODO: Transmit enable failure or something */
	}
}


void MC_DRIVE_TASK(void)
{
	static char buffer[3];

	//float vel = getQuadVelocity();
	int32_t pos = updateAbsPos();

	/* Run pi controller on pos/vel error */
	float dutyFB = PI_Controller(&vel_pid, 1000.0 - pos); /* rev per second */

	/* TODO: Implement some sort of dead band? */
	HAL_GPIO_WritePin(
		  GPIOA,
		  GPIO_PIN_10,
		  (dutyFB < 0) ? 1 : 0
	);


	/* TODO: Determine DC offset for motor */
	/* 40 Duty cycle ticks for maxon testing motor */
	uint16_t dutyCycleCounts = (uint16_t)abs(dutyFB) + 40U;

	/* Update duty cycle */
	TIM1->CCR1 = dutyCycleCounts;

	sprintf(buffer,"%d\n\r", (uint8_t)(pos >> 3));
	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 0xFFFF);
}


void MC_STOP_TASK(void)
{
	/* Wait for UI command that is valid to re-enable*/

	/* TODO: Implement Hold position for limit switch */
}


void MC_FAULT_TASK(void)
{
	/* Set PWM config to COAST mode */

	/* TODO: Implement */
	while (/* Current above threshold */);

	/* Go to disabled state */
	FSM_Transition(&fsm, DISABLED);

}
