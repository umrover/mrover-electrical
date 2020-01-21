#include "gpio.h"

/* Initialize GPIO port */
void GPIO_init(GPIO_Port_TypeDef* port) {
    // Activate AHB clock for this port
    uint32_t* RCC_AHBENR = (uint32_t*)0x40021014;
    *RCC_AHBENR |= 1 << GPIO_portClockOffset(((uint32_t)port & 0xFF00) >> 10);
}

/* Config GPIO pin */
void GPIO_config(GPIO_Port_TypeDef* port, uint8_t pin, GPIO_PinCfg_TypeDef* cfg) {
    port->MODER   |= (cfg->MODE) << (2*pin);
    port->OTYPER  |= (cfg->OTYPE) << (pin);
    port->OSPEEDR |= (cfg->OSPEED) << (2*pin);
    port->PUPDR   |= (cfg->PUPD) << (2*pin);
    if(pin < 8) {
        port->AFRL |= (cfg->AF) << (4*pin);
    }
    else {
        port->AFRH |= (cfg->AF) << (4*(pin - 8));
    }
}

/* Read state of GPIO pin */
GPIO_PinLevel GPIO_readPin(GPIO_Port_TypeDef* port, uint8_t pin) {
    return (port->IDR >> pin) & 0x0001;
}

/* Write output to GPIO pin */
void GPIO_writePin(GPIO_Port_TypeDef* port, uint8_t pin, GPIO_PinLevel state) {
    if(state == GPIO_PIN_HIGH) {
        port->BSRR = 1 << pin;
    }
    else {
        port->BSRR = 1 << (pin + 16);
    }
}

/* Toggle state of GPIO pin */
void GPIO_togglePin(GPIO_Port_TypeDef* port, uint8_t pin) {
    GPIO_PinLevel state = GPIO_readPin(port, pin);
    state = state ? 0 : 1;
    GPIO_writePin(port, pin, state);
}

/* Helper to find bitfield for GPIO port clock enable */
uint8_t GPIO_portClockOffset(uint8_t id) {
    if(id == 0x7)
        return 16;
    else
        return id + 17;
}
