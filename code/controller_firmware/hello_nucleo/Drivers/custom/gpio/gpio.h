#ifndef _GPIO_H_
#define _GPIO_H_

#include <stdint.h>

/* GPIO Pin State Enum */
typedef enum {
    GPIO_PIN_LOW = 0U,
    GPIO_PIN_HIGH = 1U
} GPIO_PinLevel;

/* GPIO Port Type */
typedef struct {
    uint32_t MODER;     /* GPIO port mode register                  */
    uint32_t OTYPER;    /* GPIO port output type register           */
    uint32_t OSPEEDR;   /* GPIO port output speed register          */
    uint32_t PUPDR;     /* GPIO port pull-up/pull-down register     */
    uint32_t IDR;       /* GPIO port input data register            */
    uint32_t ODR;       /* GPIO port output data register           */
    uint32_t BSRR;      /* GPIO port bit set/reset register         */
    uint32_t LCKR;      /* GPIO port configuration lock register    */
    uint32_t AFRL;      /* GPIO alternate function low register     */
    uint32_t AFRH;      /* GPIO alternate function high register    */
    uint32_t BRR;       /* GPIO port bit reset register             */
} GPIO_Port_TypeDef;

/* GPIO Ports */
#define GPIOA   (GPIO_Port_TypeDef*)(0x48000000)
#define GPIOB   (GPIO_Port_TypeDef*)(0x48000400)
#define GPIOC   (GPIO_Port_TypeDef*)(0x48000800)
#define GPIOD   (GPIO_Port_TypeDef*)(0x48000C00)
#define GPIOE   (GPIO_Port_TypeDef*)(0x48001000)
#define GPIOF   (GPIO_Port_TypeDef*)(0x48001400)
#define GPIOG   (GPIO_Port_TypeDef*)(0x48001800)
#define GPIOH   (GPIO_Port_TypeDef*)(0x48001C00)

/* GPIO Pin Numbers */
#define GPIO_Pin0   0
#define GPIO_Pin1   1
#define GPIO_Pin2   2
#define GPIO_Pin3   3
#define GPIO_Pin4   4
#define GPIO_Pin5   5
#define GPIO_Pin6   6
#define GPIO_Pin7   7
#define GPIO_Pin8   8
#define GPIO_Pin9   9
#define GPIO_Pin10  10
#define GPIO_Pin11  11
#define GPIO_Pin12  12
#define GPIO_Pin13  13
#define GPIO_Pin14  14
#define GPIO_Pin15  15

/* GPIO Configuration Values */
#define GPIO_MODE_INPUT     0b00
#define GPIO_MODE_OUTPUT    0b01
#define GPIO_MODE_ALTERNATE 0b10
#define GPIO_MODE_ANALOG    0b11

#define GPIO_TYPE_PP        0b0
#define GPIO_TYPE_OD        0b1

#define GPIO_SPEED_LOW      0b00
#define GPIO_SPEED_MED      0b01
#define GPIO_SPEED_HIGH     0b11

#define GPIO_PUPD_NONE      0b00
#define GPIO_PUPD_PULLUP    0b01
#define GPIO_PUPD_PULLDOWN  0b10

/* GPIO Config Type */
typedef struct {
    uint32_t MODE;
    uint32_t OTYPE;
    uint32_t OSPEED;
    uint32_t PUPD;
    uint32_t AF;
} GPIO_PinCfg_TypeDef;

/* Initialize GPIO port */
void GPIO_init(GPIO_Port_TypeDef* port);

/* Config GPIO pin */
void GPIO_config(GPIO_Port_TypeDef* port, uint8_t pin, GPIO_PinCfg_TypeDef* cfg);

/* Read state of GPIO pin */
GPIO_PinLevel GPIO_readPin(GPIO_Port_TypeDef* port, uint8_t pin);

/* Write output to GPIO pin */
void GPIO_writePin(GPIO_Port_TypeDef* port, uint8_t pin, GPIO_PinLevel state);

/* Toggle state of GPIO pin */
void GPIO_togglePin(GPIO_Port_TypeDef* port, uint8_t pin);

/* Helper to find bitfield for GPIO port clock enable */
uint8_t GPIO_portClockOffset(uint8_t id);

#endif
