/*
 * post.h
 *
 *  Created on: Dec 2, 2019
 *      Author: micha
 */

#ifndef INC_POST_H_
#define INC_POST_H_

#include <stdint.h>

/* Select the power on self tests to run */
#define POST_DRIVE_MODE_BRUSHLESS	(1 << 0U)
#define POST_DRIVE_MODE_BRUSHED  	(1 << 1U)
#define POST_DRIVER_MODE			(POST_DRIVE_MODE_BRUSHLESS | POST_DRIVE_MODE_BRUSHED)

/* TODO: Determine how to handle individual DRV faults */
#define POST_DRV					(1 << 2U)

/* Contains state of tests conducted */
typedef struct {
	uint32_t enabled_tests;
	uint32_t test_status;
} post_t;

/* Initialize the power on self tests */
void post_init(post_t* post);

/* Conduct power on self tests */
int post_start(post_t* post);

/* post_check_DRV()
 *
 *	@breif		Conduct the power on self test
 *				for the DRV using its driver
 *	@param		None
 *	@retVal		0 if PASS, -1 if FAIL
 */
int post_check_DRV(void);


/* post_check_brushless()
 *
 *	@breif		Conduct the power on self test
 *				for brushless wiring and motor setup
 *	@param		None
 *	@retVal		0 if PASS, -1 if FAIL
 */
int post_check_brushless(void);


/* post_check_brushed()
 *
 *	@breif		Conduct the power on self test
 *				for brushed wiring and motor setup
 *	@param		None
 *	@retVal		0 if PASS, -1 if FAIL
 */
int post_check_brushed(void);

#include "main.h"

#endif /* INC_POST_H_ */
