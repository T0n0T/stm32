#ifndef __BOARD_H__
#define __BOARD_H__

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "stm32l4xx_hal.h"

void gpio_clk_init(GPIO_TypeDef* GPIOx);
void board_init(void);

#endif /* __BOARD_H__ */