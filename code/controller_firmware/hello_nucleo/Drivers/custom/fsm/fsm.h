#ifndef FSM_H
#define FSM_H

#include <stdbool.h>
#include "main.h"


/* VM BUS Voltage Monitoring */
#define FAULT_VM_VOLT_OVER          (1<<0)
#define FAULT_VM_VOLT_UNDER         (1<<1)
#define FAULT_VM                    (FAULT_VM_VOLT_OVER | FAULT_VM_VOLT_UNDER)

/* Phase current monitoring */
#define FAULT_PHASEA_CURR_OVER      (1<<2)
#define FAULT_PHASEB_CURR_OVER      (1<<3)
#define FAULT_PHASEC_CURR_OVER      (1<<4)
#define FAULT_CURR_OVER             (FAULT_PHASEA_CURR_OVER | FAULT_PHASEB_CURR_OVER | FAULT_PHASEC_CURR_OVER)

/* Temperature monitoring */
#define FAULT_DCLINK_TEMP_OVER      (1<<5)
#define FAULT_MOTOR_TEMP_OVER       (1<<6)
#define FAULT_MOSFET_TEMP_OVER      (1<<7)
#define FAULT_GATEDRIVER_TEMP_OVER  (1<<8)
#define FAULT_TEMP_OVER             (FAULT_DCLINK_TEMP_OVER | FAULT_MOTOR_TEMP_OVER | FAULT_MOSFET_TEMP_OVER | FAULT_GATEDRIVER_TEMP_OVER)

/* Position Sensing */
#define FAULT_RESOLVER_OPEN         (1<<9)
#define FAULT_RESOLVER_SHORT        (1<<10)
#define FAULT_RESOLVER              (FAULT_RESOLVER_OPEN | FAULT_RESOLVER_SHORT)

/* Gate Driver faults DRV8353 */
#define FAULT_DRV_nFault            (1<<11)
#define FAULT_GATEDRIVER            (FAULT_DRV_nFault)

/* UI Faults */
#define FAULT_I2C_HEARTBEAT         (1<<12)
#define FAULT_I2C_BUSFAULT          (1<<13)
#define FAULT_I2C                   (FAULT_I2C_HEARTBEAT | FAULT_I2C_BUSFAULT)

/* Controller Performance */
#define FAULT_FOC_DEADLINE          (1<<14)
#define FAULT_SPEED_OVER            (1<<15)
#define FAULT_WATCHDOG_FAILURE      (1<<16)
#define FAULT_FLASH_SELFTEST        (1<<17)

/* Current Sensing */
#define FAULT_ICS_RANGE_OVER        (1<<18)
#define FAULT_ICS_RANGE_UNDER       (1<<19)
#define FAULT_ICS                   (FAULT_ICS_RANGE_OVER | FAULT_ICS_RANGE_UNDER)

/* Fault Classes */
#define DEF_DISABLE_FAULT_MASK      (FAULT_VM | FAULT_CURR_OVER | FAULT_TEMP_OVER | FAULT_RESOLVER | FAULT_GATEDRIVER | FAULT_I2C | FAULT_SPEED_OVER | FAULT_WATCHDOG_FAILURE | FAULT_FLASH_SELFTEST | FAULT_ICS | FAULT_FOC_DEADLINE)
#define DEF_LATCH_FALT_MASK         (FAULT_TEMP_OVER | FAULT_RESOLVER | FAULT_GATEDRIVER | FAULT_CAN | FAULT_WATCHDOG_FAILURE | FAULT_FLASH_SELFTEST | FAULT_ICS)
#define DEF_HARD_FALT_MASK          (FAULT_VM_VOLT_UNDER)


/* MOTOR Controller Moore State */
typedef enum {
	DISABLED, 			/* Controller is disabled until enabled command */
	STARTUP,			/* Controller has been requested to enable, performs startup checks */
	ENABLED,			/* Controller is enabled and responding to position commands */
	FAULT,				/* Controller has thrown fault and is waiting for power down */
	STOP				/* Controller coasts and awaits valid command or state change */
} fsm_state_t;

/* Global FSM structure */
typedef struct {
	fsm_state_t state;
	uint32_t active_faults;
	uint32_t enabled_faults;
} fsm_t;

/* Initialize faults and fsm upon initialization */
void FSM_INIT(fsm_t* fsm);

/* Validate transition when called in fsm handlers */
int FSM_Transition(fsm_t* fsm, fsm_state_t next_state);

/* Retrieve active status of fault */
int getFaultStatus(fsm_t* fsm, uint32_t fault);

/* Enabling or disabling */
void enableFault(fsm_t* fsm, uint32_t fault);
void disableFault(fsm_t* fsm, uint32_t fault);

/* Set fault status to active, call fault handler */
int throwFault(fsm_t* fsm, uint32_t fault);

/* Clear active status of fault */
void clearFault(fsm_t* fsm, uint32_t fault);

void FSM_RUN(fsm_t* fsm);

#endif /* FSM_H */
