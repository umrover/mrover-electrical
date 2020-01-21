/*
 * post.c
 *
 *  Created on: Dec 2, 2019
 *      Author: micha
 */
#include "post.h"


/**************************************************************
 * Test Framework
 **************************************************************/

/* Initialize the power on self tests */
void post_init(post_t* post)
{
	/* User enabled post */
	post->enabled_tests =	POST_DRIVER_MODE |
							POST_DRV;

	/* Set enabled tests to flagged. Must be set low
	 * by post_start function to pass
	 */
	post->test_status = post->enabled_tests;
}

/* Conduct power on self tests */
int post_start(post_t* post)
{
	if (post->enabled_tests & POST_DRIVE_MODE_BRUSHLESS)
	{
		if (!post_check_brushless())
		{
			/* PASS */
			post->test_status &= ~POST_DRIVE_MODE_BRUSHLESS;
		}
	}
	else if (post->enabled_tests & POST_DRIVE_MODE_BRUSHED)
	{
		if (!post_check_brushed())
		{
			/* PASS */
			post->test_status &= ~POST_DRIVE_MODE_BRUSHED;
		}
	}

	/* Check DRV settings */
	if (post->enabled_tests & POST_DRV)
	{
		if (!post_check_DRV())
		{
			/* PASS */
			post->test_status &= ~POST_DRV;
		}
	}

	/* If any tests not passed */
	if (post->test_status)
	{
		/* Failure of POST
		 * @	Results in disabling of the inverter
		 * @	Must wait for next enabled call through UI
		 */
		return -1;
	}

	/* ALL PASS */
	/* Return with no faults */
	return 0;
}

/**************************************************************
 * Unit Test Implementation
 **************************************************************/

/*
 *	@breif		Conduct the power on self test
 *				for the DRV using its driver
 *	@param		None
 *	@retVal		0 if PASS, -1 if FAIL
 */
int post_check_DRV(void)
{
	/* Read All DRV parameters */
	DRV_writeAll();
	DRV_SPI_DEAD_TIME
	DRV_readAll();

	/* TODO: Confirm values are correct */
	return 0;
}


/* post_check_brushless()
 *
 *	@breif		Conduct the power on self test
 *				for brushless wiring and motor setup
 *	@param		None
 *	@retVal		0 if PASS, -1 if FAIL
 */
int post_check_brushless(void)
{
	return 0;
}


/* post_check_brushed()
 *
 *	@breif		Conduct the power on self test
 *				for brushed wiring and motor setup
 *	@param		None
 *	@retVal		0 if PASS, -1 if FAIL
 */
int post_check_brushed(void)
{
	return 0;
}
