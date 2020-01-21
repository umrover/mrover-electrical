#ifndef _NVIC_H_
#define _NVIC_H_

#include <stdint.h>

#define NVIC_BASE_ADDR  0xE000E000
#define NVIC_ISER_OFST  0x100
#define NVIC_ICER_OFST  0x180
#define NVIC_ISPR_OFST  0x200
#define NVIC_ICPR_OFST  0x280
#define NVIC_IABR_OFST  0x300
#define NVIC_IPR_OFST   0x400
#define STIR_OFST       0xF00

void NVIC_setEnable(uint8_t irq_num);

void NVIC_clrEnable(uint8_t irq_num);

uint8_t NVIC_readEnable(uint8_t irq_num);

void NVIC_setPending(uint8_t irq_num);

void NVIC_clrPending(uint8_t irq_num);

uint8_t NVIC_readPending(uint8_t irq_num);

uint8_t NVIC_readActive(uint8_t irq_num);

#endif
