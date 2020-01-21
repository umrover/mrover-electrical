/*
 * quad.c
 *
 *  Created on: Nov 25, 2019
 *      Author: micha
 */

#include "quad.h"
#include "main.h"

#define MAXON_PPR 2000.0
#define QUAD_TIM TIM3
#define QUAD_SAMPLE_FREQ_HZ 2000.0

static uint16_t prev_cnts = 0;
static int32_t absPos = 0;

float avg_weight = 0.65;


float getQuadVelocity()
{
	/* Velocity Average */
	static float prev_vel = 0;

	/* Retrieve encoder counts and update absolute position */
	uint16_t cur_cnts = (QUAD_TIM->CNT & 0xFFFF);

	/* Calculate velocity */
	float vel = (int16_t)(cur_cnts - (int16_t)prev_cnts);

	/* Update state*/
	prev_cnts = cur_cnts;
	return prev_vel = vel*(avg_weight) + prev_vel*(1-avg_weight);
}

int32_t updateAbsPos()
{
  uint16_t cur_cnts = (QUAD_TIM->CNT & 0xFFFF);
  absPos = absPos + (int16_t)(cur_cnts - prev_cnts);
  prev_cnts = cur_cnts;
  return absPos;
}

void resetAbsPos(void)
{
	absPos = 0;
}

