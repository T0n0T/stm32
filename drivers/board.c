#include "board.h"
#include "cm_backtrace.h"

void board_init(void)
{
    HAL_Init();
    extern void SystemClock_Config(void);
    SystemClock_Config();
    cm_backtrace_init("STM32L4", "V1.0", "1.0.0");
}

void gpio_clk_init(GPIO_TypeDef* GPIOx)
{
    switch ((uint32_t)GPIOx) {
        case (uint32_t)GPIOA:
            __HAL_RCC_GPIOA_CLK_ENABLE();
            break;
        case (uint32_t)GPIOB:
            __HAL_RCC_GPIOB_CLK_ENABLE();
            break;
        case (uint32_t)GPIOC:
            __HAL_RCC_GPIOC_CLK_ENABLE();
            break;
        case (uint32_t)GPIOD:
            __HAL_RCC_GPIOD_CLK_ENABLE();
            break;      
        default:
            break;
    }
}