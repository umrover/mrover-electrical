#include "nvic.h"

void NVIC_setEnable(uint8_t irq_num) {
    uint8_t set_num = irq_num >> 5;
    *( (uint32_t*)(NVIC_BASE_ADDR + NVIC_ISER_OFST + 0x4 * set_num) ) |= 0b1;
}

void NVIC_clrEnable(uint8_t irq_num) {
    uint8_t set_num = irq_num >> 5;
    *( (uint32_t*)(NVIC_BASE_ADDR + NVIC_ICER_OFST + 0x4 * set_num) ) |= 0b1;
}

uint8_t NVIC_readEnable(uint8_t irq_num) {
    uint8_t set_num = irq_num >> 5;
    return *( (uint32_t*)(NVIC_BASE_ADDR + NVIC_ISER_OFST + 0x4 * set_num) ) & 0b1;
}

void NVIC_setPending(uint8_t irq_num) {
    uint8_t set_num = irq_num >> 5;
    *( (uint32_t*)(NVIC_BASE_ADDR + NVIC_ISPR_OFST + 0x4 * set_num) ) |= 0b1;
}

void NVIC_clrPending(uint8_t irq_num) {
    uint8_t set_num = irq_num >> 5;
    *( (uint32_t*)(NVIC_BASE_ADDR + NVIC_ICPR_OFST + 0x4 * set_num) ) |= 0b1;
}

uint8_t NVIC_readPending(uint8_t irq_num) {
    uint8_t set_num = irq_num >> 5;
    return *( (uint32_t*)(NVIC_BASE_ADDR + NVIC_ISPR_OFST + 0x4 * set_num) ) & 0b1;
}

uint8_t NVIC_readActive(uint8_t irq_num) {
    uint8_t set_num = irq_num >> 5;
    return *( (uint32_t*)(NVIC_BASE_ADDR + NVIC_IABR_OFST + 0x4 * set_num) ) & 0b1;
}
