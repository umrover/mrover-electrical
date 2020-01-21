/*
 * mc_tasks.c
 *
 *  Created on: Dec 2, 2019
 *      Author: micha
 */

#include "mc_tasks.h"
#include "i2c.h"

void MC_DISABLED_TASK(void)
{
	/* Set PWM to brake mode */
	PWM_driveMotor(-0);

	PID_SetIntegralTerm(&vel_pid, 0);
}


void MC_STARTUP_TASK(void)
{
	/* Conduct power on self tests */
	if (post_start(&mc_posts) == -1)
	{
		/* Go back to the disabled state */
		FSM_Transition(&mc_state, DISABLED);

		/* TODO: Transmit enable failure or something */
	}
	else
	{
		FSM_Transition(&mc_state, ENABLED);
	}
}


void MC_DRIVE_TASK(void)
{
	static char buffer[3];
	float fdbk;
	float dutyFB;
	float setpoint = currentControl.SETPOINT;

	switch(currentControl.CONTROL_MODE)
	{
		case (0x01):
			/* Open Loop Throttle */
			dutyFB = (float)currentControl.SETPOINT / 0xFFFF;

			break;

		case (0x02):
			/* Closed Loop Position */
			fdbk = (float)updateAbsPos();

			/* Run pi controller on pos/vel error */
		    dutyFB = PI_Controller(&vel_pid, setpoint - fdbk); /* rev per second */

			break;

		case (0x04):

			/* Get velocity from quad encoder */
			fdbk = getQuadVelocity();

			/* Closed Loop Velocity */
		    dutyFB = PI_Controller(&vel_pid, setpoint - fdbk); /* rev per second */

			break;
	}

	/* Drive Motor */
	PWM_driveMotor(dutyFB);
}


void MC_STOP_TASK(void)
{
	/* TODO: Implement Hold position for limit switch */
	/* COAST */
	PWM_driveMotor(-0);
}


void MC_FAULT_TASK(void)
{
	float threshold = 10; // rev/s

	/* Set PWM config to COAST mode */
	PWM_driveMotor(-0);

	/* TODO: Implement */
	while (getQuadVelocity() > threshold);

	/* Go to disabled state */
	FSM_Transition(&mc_state, DISABLED);

}
