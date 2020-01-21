/*
 * quad.h
 *
 *  Created on: Nov 25, 2019
 *      Author: micha
 */

#ifndef CUSTOM_QUAD_QUAD_H_
#define CUSTOM_QUAD_QUAD_H_

#include <stdint.h>

/* getQuadVelocity()
 *
 * @brief		returns velocity in revolutions per second.
 * 				requires that the encoder pulses per rev and
 * 				sampling frequency macros are correct. Up to
 * 				the user to call at a frequency high enough
 * 				to prevent aliasing
 *
 * @params		None
 *
 * @retVal		velocity in rev/s
 */
float getQuadVelocity();

/* updateAbsPos()
 *
 * @brief		Determines to the abosolute position of the encoder since
 * 				start up. Up to the user to call at a frequency high enough
 * 				to prevent aliasing
 *
 * @params		None
 *
 * @retVal		position in ticks
 */
int32_t updateAbsPos();


/* resetAbsPos()
 *
 * @brief		Resets the absolute encoder position counter
 * 				to zero.
 *
 * @params		None
 *
 * @retVal		None
 */
void resetAbsPos(void);


#endif /* CUSTOM_QUAD_QUAD_H_ */
