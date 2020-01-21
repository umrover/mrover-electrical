#ifndef _SPI_H_
#define _SPI_H_

#include <stdint.h>
//#include "gpio.h"

/* SPI Module Type */
typedef struct {
	volatile uint32_t CR1;   /* SPI control register 1 */
	volatile uint32_t CR2;   /* SPI control register 2 */
	volatile uint32_t SR;    /* SPI status register    */
	volatile uint32_t DR;    /* SPI data register      */
} SPI_Module_TypeDef;

/* SPI Modules */
#define SPI1    (SPI_Module_TypeDef*)(0x40013000)
#define SPI2    (SPI_Module_TypeDef*)(0x40003800)
#define SPI3    (SPI_Module_TypeDef*)(0x40003C00)
#define SPI4    (SPI_Module_TypeDef*)(0x40013C00)

#define SPI_CLKDIV_2    0b000
#define SPI_CLKDIV_4    0b001
#define SPI_CLKDIV_8    0b010
#define SPI_CLKDIV_16   0b011
#define SPI_CLKDIV_32   0b100
#define SPI_CLKDIV_64   0b101
#define SPI_CLKDIV_128  0b110
#define SPI_CLKDIV_256  0b111

#define SPI_DS_04   0b0011
#define SPI_DS_05   0b0100
#define SPI_DS_06   0b0101
#define SPI_DS_07   0b0110
#define SPI_DS_08   0b0111
#define SPI_DS_09   0b1000
#define SPI_DS_10   0b1001
#define SPI_DS_11   0b1010
#define SPI_DS_12   0b1011
#define SPI_DS_13   0b1100
#define SPI_DS_14   0b1101
#define SPI_DS_15   0b1110
#define SPI_DS_16   0b1111

/* SPI Config Type */
typedef struct {
	uint8_t lsbFirst;
	uint8_t dataSize;
	uint8_t baud;
	uint8_t cpol;
	uint8_t cpha;
} SPI_Config_TypeDef;

/* Initialize SPI Module */
void SPI_init(SPI_Module_TypeDef* module);

/* Configure SPI Module for default master, full-duplex communication */
void SPI_config(SPI_Module_TypeDef* module, SPI_Config_TypeDef* cfg);

/* Enable SPI Module */
void SPI_enable(SPI_Module_TypeDef* module);

/* Disable SPI Module */
void SPI_disable(SPI_Module_TypeDef* module);

/* Send 8 bits over SPI */
void SPI_send8(SPI_Module_TypeDef* module, uint8_t data);

/* Send 16 bits over SPI */
void SPI_send16(SPI_Module_TypeDef* module, uint16_t data);

/* Read 8 bits from SPI */
uint8_t SPI_read8(SPI_Module_TypeDef* module);

/* Read 16 bits from SPI */
uint16_t SPI_read16(SPI_Module_TypeDef* module);

#endif
