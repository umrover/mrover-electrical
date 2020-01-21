#ifndef _EXTI_H_
#define _EXTI_H_

#include <stdint.h>

#define EXTI_BASE_ADDR  0x40010400

typedef struct {
    volatile uint32_t EXTI_IMR1;    // Interrupt mask register
    volatile uint32_t EXTI_EMR1;    // Event mask register
    volatile uint32_t EXTI_RTSR1;   // Rising trigger selection register
    volatile uint32_t EXTI_FTSR1;   // Falling trigger selection register
    volatile uint32_t EXTI_SWIER1;  // Software interrupt event register
    volatile uint32_t EXTI_PR1;     // Pending register
    volatile uint32_t RESERVED;
    volatile uint32_t EXTI_IMR2;    // Interrupt mask register
    volatile uint32_t EXTI_EMR2;    // Event mask register
    volatile uint32_t EXTI_RTSR2;   // Rising trigger selection register
    volatile uint32_t EXTI_FTSR2;   // Falling trigger selection register
    volatile uint32_t EXTI_SWIER2;  // Software interrupt event register
    volatile uint32_t EXTI_PR2;     // Pending register
    
} EXTI_CFG_TypeDef;


#endif
