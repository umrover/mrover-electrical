/*
 * mc_tasks.h
 *
 *  Created on: Dec 2, 2019
 *      Author: micha
 */

#ifndef INC_MC_TASKS_H_
#define INC_MC_TASKS_H_

#include "main.h"

void MC_DISABLED_TASK(void);
void MC_STARTUP_TASK(void);
void MC_DRIVE_TASK(void);
void MC_STOP_TASK(void);
void MC_FAULT_TASK(void);

#endif /* INC_MC_TASKS_H_ */
