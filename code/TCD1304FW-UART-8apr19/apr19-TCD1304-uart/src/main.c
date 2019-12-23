/*-
 * Copyright (c) 2017 Esben Rossel
 * All rights reserved.
 *
 * Author: Esben Rossel <esbenrossel@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */


#include "main.h"

void virtual_GND(void);
void flush_CCD(void);
void NVIC_conf(void);


__IO uint32_t SH_period = 20;
__IO uint32_t ICG_period = 500000;
__IO uint16_t aTxBuffer[CCDSize];
__IO uint16_t avgBuffer[CCDSize] = {0};
__IO uint8_t aRxBuffer[RxDataSize] = {0};
__IO uint8_t nRxBuffer[RxDataSize] = {0};

__IO uint8_t change_exposure_flag = 0;
__IO uint8_t data_flag = 0;
__IO uint8_t pulse_counter = 0;
__IO uint8_t CCD_flushed = 0;
__IO uint8_t avg_exps = 0;
__IO uint8_t exps_left = 0;
__IO uint8_t coll_mode = 0;



/* TIM Configuration
	TIM2/5 are 32 bit and will serve the ICG and SH pulses that may require very long periods.
	TIM3/4 are 16 bit and will serve the fM (master clock) and ADC clock. 

	fM (TIM3)	PB0	(Ch3)
	SH (TIM2)	PA1	(Ch2)
	ICG (TIM5)	PA0	(Ch1)
	CCD-output	PC0	(ADC-in10) */

/* UART Configuration
	Tx on PA3
	Rx on PA2 */

/* Other GPIOs
	PA0, PB2 and PC2 are driven low
	PA5 (LED) is enabled (but driven low by default) */


int main(void)
{
	int i = 0;

	/* virtual_GND() enables GPIOA, GPIOB and GPIOC clocks */
	virtual_GND();
	NVIC_conf();

	/* Setup CCD_fM (TIM3) and ADC-timer (TIM4) */
	get_Timer_clocks();
	TIM_CCD_fM_conf();
	TIM_ADC_conf();

	/* Setup UART */
	USART2_conf();

	/* Setup ADC + ADC-DMA */
	ADC1_conf();

	/* Setup ICG (TIM5) and SH (TIM2) */
	TIM_ICG_SH_conf();


	//flush_CCD();




	while(1)
	{
		if (change_exposure_flag == 1)
		{
			/* reset flag */
			change_exposure_flag = 0;
			
			flush_CCD();

			/* set new integration time */
			ICG_period = nRxBuffer[6]<<24|nRxBuffer[7]<<16|nRxBuffer[8]<<8|nRxBuffer[9];
			SH_period = nRxBuffer[2]<<24|nRxBuffer[3]<<16|nRxBuffer[4]<<8|nRxBuffer[5];

			/*	Disable ICG (TIM5) and SH (TIM2) before reconfiguring*/
			TIM_Cmd(TIM2, DISABLE);
			TIM_Cmd(TIM5, DISABLE);

			/* 	Reconfigure TIM2 and TIM5 */
			TIM_ICG_SH_conf();
		}

		switch (data_flag){
		case 1:
			/* reset flags */
			data_flag = 0;
            if (coll_mode == 1)
				pulse_counter=6;

			/* Transmit data in aTxBuffer */
			UART2_Tx_DMA();
			break;		

		case 2:
			/* reset flags */
			data_flag = 0;

			/* This is the first integration of several so overwrite avgBuffer */
			for (i=0; i<CCDSize; i++)
				avgBuffer[i] = aTxBuffer[i];	
			break;

		case 3:
			/* reset flags */
			data_flag = 0;

			/* Add new to previous integrations.
			   This loop takes 3-4ms to complete. */		
			for (i=0; i<CCDSize; i++)
				avgBuffer[i] = avgBuffer[i] + aTxBuffer[i];		
			break;

		case 4:
			/* reset flags */
			data_flag = 0;

			/* Add new to previous integrations.
			   This loop takes 3-4ms to complete. */		
			for (i=0; i<CCDSize; i++)
				avgBuffer[i] = avgBuffer[i] + aTxBuffer[i];		

			/* Store average values in aTxBuffer */
			for (i=0; i<CCDSize; i++)
				aTxBuffer[i] = avgBuffer[i]/avg_exps;

            if (coll_mode == 1){
				exps_left = avg_exps;
				pulse_counter=6;	
			}			

			/* Transmit data in aTxBuffer */
			UART2_Tx_DMA();
			break;
		}
	}
}



/* 	To keep noise-level on ADC-in down, the following GPIO's are
	set as output, driven low and physically connected to GND:
		PC1 and PC4 which are physically close to PC0 (ADC-in)
		PB1 and PC5 which are physically close to PB0 - the most busy GPIO (fM) */
void virtual_GND(void)
{
	GPIO_InitTypeDef    	GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

	/* 	Clock the GPIOs */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  

	/* PC2 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* PB1 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Setup LED (PA5) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}


/* Run this function prior to datacollection */
void flush_CCD()
{
	/* Set exposure very low */
	ICG_period = 15000;
	SH_period = 20;

	/*	Disable ICG (TIM5) and SH (TIM2) before reconfiguring*/
	TIM_Cmd(TIM2, DISABLE);
	TIM_Cmd(TIM5, DISABLE);

	/*	Reset flags and counters */
	CCD_flushed = 0;
	pulse_counter = 0;

	/* 	Reconfigure TIM2 and TIM5 */
	TIM_ICG_SH_conf();

	/*	Block until CCD is properly flushed */
	while(CCD_flushed == 0);
}


/* Configure interrupts */
void NVIC_conf(void)
{
	NVIC_InitTypeDef		NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* ICG (TIM5) IRQ */
	/* The TIM5 update interrupts starts TIM4 and ADC */
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* ADC-DMA IRQ */
	/* DMA1 Transfer complete interrupt stops TIM4 and ADC */
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* USART2-DMA-Rx IRQ */
	/* DMA1 Transfer complete interrupt checks incoming data */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
