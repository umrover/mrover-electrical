#include "adc.h"

/* INITIALIZE ADC */
void ADC_init(ADCSINGLE_TypeDef* adc) {
	// Enable ADC clock source from RCC
	*( (uint32_t*)(RCC_BASE_ADDR + 0x14) ) |= (0b11 << 28);
    // Enable voltage regulator
	adc->ADC_CR = (0b01 << 28);
	// Wait at least 10 us
	for(int i = 0; i < 1000000; ++i) {};
	// Calibrate ADC
	adc->ADC_CR |= (0b1 << 31);

	// Wait for calibration to finish
	while( (adc->ADC_CR >> 31) & 0b1 ) {};

	// Enable ADC
    adc->ADC_CR |= 0b1;
}

/* CONFIG ADC */
void ADC_cfg(ADCSINGLE_TypeDef* adc, uint8_t res, uint8_t ch) {
    // Discontinuous, resolution
    adc->ADC_CFGR |= (0b1 << 26) | (0b1 << 16) | (res << 3);

    adc->ADC_SQR1 |= (ch << 6);
}

/* ADC READY */
uint8_t ADC_ready(ADCSINGLE_TypeDef* adc) {
    return (adc->ADC_ISR & 0b1);
}

/* START CONVERSION */
void ADC_startConversion(ADCSINGLE_TypeDef* adc) {
    adc->ADC_CR |= (0b1 << 2);
}

/* READ CONVERTED DATA */
uint32_t ADC_readData(ADCSINGLE_TypeDef* adc) {
    return (adc->ADC_DR & 0x0000FFFF);
}
