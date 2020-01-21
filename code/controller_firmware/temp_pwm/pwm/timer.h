#ifndef TIMER_H
#define TIMER_H

#include <stm32f303xe.h>

#define TIMER_CLK_FREQ 64000000U

void TIMxInit(TIM_TypeDef *TIMx);

int TIMxSet(TIM_TypeDef* TIMx, uint32_t freq, float duty);

float getDutyCycle(TIM_TypeDef* TIMx);

#endif /* TIMER_H */
