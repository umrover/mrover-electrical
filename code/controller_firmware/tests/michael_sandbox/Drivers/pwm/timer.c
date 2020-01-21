#include "timer.h"
#include "gpio.h"
#include "stm32f303xe.h"


void TIMxInit(TIM_TypeDef *TIMx)
{
	// enable TIM1 clock
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

	/* Use A7 as TIM1 CH1N */
	GPIO_init(GPIOA);
	GPIO_PinCfg_TypeDef pinCfg;
	pinCfg.MODE = GPIO_MODE_ALTERNATE;
	pinCfg.OTYPE = GPIO_TYPE_PP;
	pinCfg.OSPEED = GPIO_SPEED_MED;
	pinCfg.PUPD = GPIO_PUPD_NONE;
	pinCfg.AF = 6;
	GPIO_config(GPIOA, 7, &pinCfg);

	/* Use A8 as CH1 */
	pinCfg.MODE = GPIO_MODE_ALTERNATE;
	pinCfg.OTYPE = GPIO_TYPE_PP;
	pinCfg.OSPEED = GPIO_SPEED_MED;
	pinCfg.PUPD = GPIO_PUPD_NONE;
	pinCfg.AF = 6;
	GPIO_config(GPIOA, 8, &pinCfg);

	/* Disable the timer for configuration */
	TIMx->CR1 &= ~TIM_CR1_CEN;

	TIMx->CR1 =	/* TIMx control register 1 */
					TIM_CR1_CMS_1 |												// set the timer in center align mode (with compare events on upcount)
					TIM_CR1_ARPE |												// enable preload of the auto-reload register
					TIM_CR1_CKD_0; 												// set dead-time clock as 2 times sample clock

	TIMx->CR2 =	/* TIMx control register 2 */
					TIM_CR2_OIS1N;											// set output channel 1N idle state to high

	TIMx->CCMR1 =	/* TIMx output compare mode register 1 */
					TIM_CCMR1_OC1PE |											// enable preload for channel 1 output compare register
					TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2; 						// set channel 1 to PWM mode 1 (active when TIMx_CNT < TIMx_CCR1)

	TIMx->CCER = 	/* TIMx channel enable register */
					TIM_CCER_CC1E |												// enable output compare channel 1
					TIM_CCER_CC1NE;											    // enable output compare channel 1N (complement of 1)


	TIMx->PSC = 	/* TIMx prescaler register */
					0;															// don't use a prescaler to enable maximum resolution (168 MHz clock)

	TIMx->ARR = 	/* TIMx auto-reload register (period) */
					8000;														// set timer frequency to 168 MHz / 14000 = 12 kHz

	TIMx->CCR1 = 	/* TIMx channel 4 compare register */
					4000;														// set channel 4 to timing of ADC sample

	TIMx->BDTR =	/* TIMx break and dead-time register */
					((84 << TIM_BDTR_DTG_Pos) & TIM_BDTR_DTG_Msk) |				// set dead-time to 84/168MHz * 2 = 1us
					TIM_BDTR_OSSI |
					TIM_BDTR_BKE |												// enable break signal
					TIM_BDTR_BKP |												// set break signal as active high TODO: check if this is true and add filter (potentially)
					TIM_BDTR_MOE;												// main output enable


	// load registers from shadow registers by triggering the first update event
	//TIMx->EGR |= TIM_EGR_UG;

	// enable timer
	TIMx->CR1 |= TIM_CR1_CEN;

	//    HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
	//	HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
}

/* Takes in frequency in HZ and duty cycle from 0 to 1 */
int TIMxSet(TIM_TypeDef* TIMx, uint32_t freq, float duty)
{
	/* Error Checking */
	if (freq > TIMER_CLK_FREQ || duty < 0 || duty > 1 )
	{
		return 0;
	}

	/* Convert frequency to a auto reload value */
	uint32_t freq_cnts = TIMER_CLK_FREQ / freq;

	/* Calculate Duty Cycle */
	uint32_t duty_cnts = freq_cnts * duty;

	/* Update Frequency */
	TIMx->ARR = freq_cnts;

	/* Update Duty Cycle for channel 1 */
	TIMx->CCR1 = duty_cnts;

	return 1;
}


float getDutyCycle(TIM_TypeDef* TIMx)
{
	return TIMx->CCR1 / TIMx->ARR;
}
