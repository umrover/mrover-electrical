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

extern __IO uint16_t aTxBuffer[CCDSize];
extern __IO uint8_t aRxBuffer[RxDataSize];
extern __IO	uint8_t nRxBuffer[RxDataSize];


/* USART2 is connected on PA2 / PA3 - DMA1 Ch4 Stream 5/6 */
void USART2_conf()
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef    	GPIO_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;

	/* Enable USART & DMA clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	/* Use PA2 and PA3 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	/* Configure USART Tx and Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz; //100 MHz?
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USART2 - 115.2 kbps */
	//USART_OverSampling8Cmd(USARTx, ENABLE); 
	USART_InitStructure.USART_BaudRate = BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	/* When using Parity the word length must be configured to 9 bits */
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);


	/* Configure DMA - common for Rx/Tx */
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable ;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull ;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t) (&(USART2->DR)) ;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;

	/* Configure RX DMA */
	DMA_InitStructure.DMA_BufferSize = RxDataSize;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	//Listen continously
	DMA_InitStructure.DMA_Channel = DMA_Channel_4 ;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;
	DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)aRxBuffer ; 
	DMA_Init(DMA1_Stream5,&DMA_InitStructure);

	/*	DMA1 Rx interrupt configuration */
	DMA_ITConfig(DMA1_Stream5, DMA_IT_TC, ENABLE);

	/* Configure TX DMA */
	DMA_InitStructure.DMA_BufferSize = 2*CCDSize; //The data is 16-bit, but UART is 8-bit
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Channel = DMA_Channel_4 ;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral ;
	DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)aTxBuffer ;
	DMA_Init(DMA1_Stream6,&DMA_InitStructure);


	/* Enable USART */
	USART_Cmd(USART2, ENABLE);

	/* Enable Rx DMA stream */
	DMA_Cmd(DMA1_Stream5,ENABLE);	
	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
}



void UART2_Tx_DMA(void)
{
	/* Notice that UART is 8-bit. The 12-bit data from the ADC
	is stored in a 16-bit array and sent 8-bit at a time. This
	causes an endianness-like problem on the receiving side eg:
		the uint16 0x0102 is received as 0x02 0x01 
	This must be handled on the receiving side with 
		proper_data[i] = (unsigned int)Rx_data[i+1] << 8 | Rx_data[i]; */

	/* Transmit 1x */

	/* Enable DMA stream */
	DMA_Cmd(DMA1_Stream6,ENABLE);
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);

  	/* Wait for transmission to complete */
  	while (USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
    while (DMA_GetFlagStatus(DMA1_Stream6,DMA_FLAG_TCIF6)==RESET);

	/* Clear DMA Transfer Complete Flags */
	DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);
	/* Clear USART Transfer Complete Flags */
	USART_ClearFlag(USART2,USART_FLAG_TC);
//	GPIOA->ODR ^= GPIO_Pin_5;
}



void sort_aRxBuffer()
{
	int shift = -1;	
	int i = 0;

	/* Examine for byte-shifts in aRxBuffer */
	for (i = 0; i < RxDataSize; i++)
	{
		if ((aRxBuffer[i]==69)&&(aRxBuffer[i+1]==82))
			shift = i;
	}
	if ((aRxBuffer[RxDataSize-1]==69)&&(aRxBuffer[0]==82))
			shift = RxDataSize-1;

	/* If necessary permutate data */
	if (shift != -1){
		for (i = 0; i < RxDataSize - shift; i++) 
			nRxBuffer[i] = aRxBuffer[shift+i]; 
		for (i = 0; i < shift; i++)
			nRxBuffer[RxDataSize-shift+i] = aRxBuffer[i];
	}

	/* Clear aRxBuffer */
	for (i = 0; i < RxDataSize; i++)
	{
		aRxBuffer[i] = 0;
	}

}
