#ifndef __BOARD_H__
#define __BOARD_H__

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "stm32h7xx_hal.h"

#define GPIO_SET_PIN(port, pin)   ((port)->BSRR = (pin))
#define GPIO_RESET_PIN(port, pin) ((port)->BSRR = ((pin) << 16))
#define GPIO_READ_PIN(port, pin)  (((port)->IDR & (pin)) != 0)

typedef void (*wakeup_handle_func)(uint8_t);

void gpio_clk_init(GPIO_TypeDef* GPIOx);
void board_init(void);
void wakeup_init(wakeup_handle_func h);

#endif /* __BOARD_H__ */