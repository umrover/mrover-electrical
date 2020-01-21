#include "spi.h"

/* Initialize SPI Module */
void SPI_init(SPI_Module_TypeDef* module) {
	uint8_t ahb2 = ((uint32_t)module >> 16) & 1;
	uint32_t* RCC_APBxENR = (ahb2) ? (uint32_t*)0x40021018 : (uint32_t*)0x4002101C;

	if(ahb2) {
		if(((uint32_t)module >> 10) & 0x3 == 3)
			// SPI 4
			*RCC_APBxENR |= 1 << 15;
		else
			// SPI 1
			*RCC_APBxENR |= 1 << 12;
	}
	else {
		if(((uint32_t)module >> 10) & 0x3 == 3)
			// SPI 3
			*RCC_APBxENR |= 1 << 15;
		else
			// SPI 2
			*RCC_APBxENR |= 1 << 14;
	}
}

/* Configure SPI Module for default master, full-duplex communication */
void SPI_config(SPI_Module_TypeDef* module, SPI_Config_TypeDef* cfg) {
    // configure SPI CLK baudrate
	module->CR1 |= (cfg->baud) << 3;
	// configure CPOL/CPHA
	module->CR1 |= (cfg->cpol) << 1;
	module->CR1 |= (cfg->cpha) << 0;
	// configure LSBFIRST
	module->CR1 |= (cfg->lsbFirst) << 7;
	// configure SSM/SSI
	// Both are set to prevent mode errors and allow external slave management
	module->CR1 |= 1 << 9; // SSM = 1
	module->CR1 |= 1 << 8; // SSI = 1
	// set MSTR bit
	module->CR1 |= 1 << 2;
	// configure frame size
	module->CR2 |= (cfg->dataSize) << 8;
}

/* Enable SPI Module */
void SPI_enable(SPI_Module_TypeDef* module) {
	module->CR1 |= 1 << 6;
}

/* Disable SPI Module */
void SPI_disable(SPI_Module_TypeDef* module) {
	module->CR1 &= ~(1 << 6);
}

/* Send 8 bits over SPI */
void SPI_send8(SPI_Module_TypeDef* module, uint8_t data) {
	uint8_t* txfifo = (uint8_t*)&(module->DR);
	while(!((module->SR >> 1) & 0x1)); // while txfifo is not empty
	while((module->SR >> 7) & 0x1); // while busy
	*txfifo = data;
}

/* Send 16 bits over SPI */
void SPI_send16(SPI_Module_TypeDef* module, uint16_t data) {
	uint16_t* txfifo = (uint16_t*)&(module->DR);
	while(!((module->SR >> 1) & 0x1)); // while txfifo is not empty
	*txfifo = data;
	while((module->SR >> 7) & 0x1); // while busy
}

/* Read 8 bits from SPI */
uint8_t SPI_read8(SPI_Module_TypeDef* module) {
	uint8_t* rxfifo = (uint8_t*)&(module->DR);
	while(!(module->SR & 0x1)); // while rxfifo is empty
	return *rxfifo;
}

/* Read 16 bits from SPI */
uint16_t SPI_read16(SPI_Module_TypeDef* module) {
	uint16_t* rxfifo = (uint16_t*)&(module->DR);
	while(!(module->SR & 0x1)); // while rxfifo is empty
	return *rxfifo;
}
