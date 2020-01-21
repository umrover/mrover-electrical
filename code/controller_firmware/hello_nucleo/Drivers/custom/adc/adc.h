#ifndef _ADC_H_
#define _ADC_H_

#include <stdint.h>

/* ADC CONFIG TYPE */
typedef struct {
	// Defined in Reference Manual 15.5

    volatile uint32_t ADC_ISR;   	// Interrupt and Status Register
    volatile uint32_t ADC_IER;   	// Interrupt Enable Register
    volatile uint32_t ADC_CR;    	// Control Register
    volatile uint32_t ADC_CFGR;  	// Configuration Register
	volatile uint32_t RESERVED_1;
    volatile uint32_t ADC_SMPR1; 	// Sample Time Register
    volatile uint32_t ADC_SMPR2; 	// Sample Time Register
	volatile uint32_t RESERVED_2;
    volatile uint32_t ADC_TR1;   	// Watchdog 1 Threshold Register
    volatile uint32_t ADC_TR2;   	// Watchdog 2 Threshold Register
    volatile uint32_t ADC_TR3;   	// Watchdog 3 Threshold Register
	volatile uint32_t RESERVED_3;
    volatile uint32_t ADC_SQR1;  	// Regular Sequence Register 1
    volatile uint32_t ADC_SQR2;  	// Regular Sequence Register 2
    volatile uint32_t ADC_SQR3;  	// Regular Sequence Register 3
    volatile uint32_t ADC_SQR4;  	// Regular Sequence Register 4
    volatile uint32_t ADC_DR;    	// Regular Data Register
	volatile uint32_t RESERVED_4[2];
    volatile uint32_t ADC_JSQR;  	// Injected Sequence Register
	volatile uint32_t RESERVED_5[4];
    volatile uint32_t ADC_OFR1;  	// Offset Register 1
    volatile uint32_t ADC_OFR2;  	// Offset Register 2
    volatile uint32_t ADC_OFR3;  	// Offset Register 3
    volatile uint32_t ADC_OFR4;  	// Offset Register 4
	volatile uint32_t RESERVED_6[4];
    volatile uint32_t ADC_JDR1;  	// Injected Data Register 1
    volatile uint32_t ADC_JDR2;  	// Injected Data Register 2
    volatile uint32_t ADC_JDR3;  	// Injected Data Register 3
    volatile uint32_t ADC_JDR4;  	// Injected Data Register 4
	volatile uint32_t RESERVED_7[4];
    volatile uint32_t ADC_AWD2CR;	// Analog Watchdog 2 Configuration Register
    volatile uint32_t ADC_AWD3CR;	// Analog Watchdog 3 Configuration Register
	volatile uint32_t RESERVED_8[2];
	volatile uint32_t ADC_DIFSEL;	// Differential Mode Selection Register
	volatile uint32_t ADC_CALFACT;	// Calibration Factors Register
} ADCSINGLE_TypeDef;

typedef struct {
	uint32_t ADCPAIR_CSR;	// Pair Common Status Register
	uint32_t RESERVED_9;
	uint32_t ADCPAIR_CCR;	// Pair Common Control Register
	uint32_t ADCPAIR_CDR;	// Pair Common Data Register
} ADCPAIR_TypeDef;

#define ADC1_BASE_ADDR		(ADCSINGLE_TypeDef*)(0x50000000)
#define ADC2_BASE_ADDR		(ADCSINGLE_TypeDef*)(0x50000000 + 0x100)
#define ADC12_COMMON_ADDR	(ADCPAIR_TypeDef*)(0x50000000 + 0x300)	// If ADCs are in master-slave mode
#define ADC3_BASE_ADDR		(ADCSINGLE_TypeDef*)(0x50000400)
#define ADC4_BASE_ADDR		(ADCSINGLE_TypeDef*)(0x50000400 + 0x100)
#define ADC34_COMMON_ADDR	(ADCPAIR_TypeDef*)(0x50000400 + 0x300)	// If ADCs are in master-slave mode

#define ADC_RES_12b     0b00
#define ADC_RES_10b     0b01
#define ADC_RES_8b      0b10
#define ADC_RES_6b      0b11

#define RCC_BASE_ADDR	0x40021000

/* INITIALIZE ADC */
void ADC_init(ADCSINGLE_TypeDef* adc);

/* CONFIG ADC */
void ADC_cfg(ADCSINGLE_TypeDef* adc, uint8_t res, uint8_t ch);

/* ADC READY */
uint8_t ADC_ready(ADCSINGLE_TypeDef* adc);

/* START CONVERSION */
void ADC_startConversion(ADCSINGLE_TypeDef* adc);

/* READ CONVERTED DATA */
uint32_t ADC_readData(ADCSINGLE_TypeDef* adc);


/**/

/**/

/**/


#endif
