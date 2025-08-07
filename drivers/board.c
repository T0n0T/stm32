#include "board.h"

void board_init(void)
{
    HAL_Init();
    extern void SystemClock_Config(void);
    SystemClock_Config();
    SystemCoreClockUpdate();
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
        case (uint32_t)GPIOE:
            __HAL_RCC_GPIOE_CLK_ENABLE();
            break;
        case (uint32_t)GPIOF:
            __HAL_RCC_GPIOF_CLK_ENABLE();
            break;
        case (uint32_t)GPIOG:
            __HAL_RCC_GPIOG_CLK_ENABLE();
            break;
        default:
            break;
    }
}

static wakeup_handle_func wakeup_handler;

void wakeup_init(wakeup_handle_func h)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Configure PA.0 as input with External interrupt */
    GPIO_InitStructure.Pin  = GPIO_PIN_0;
    GPIO_InitStructure.Pull = GPIO_PULLDOWN;
    GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING_FALLING;

    /* Enable GPIOA clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
    /* Enable and set PA.0 EXTI Interrupt to the lowest priority */
    NVIC_SetPriority((IRQn_Type)(EXTI0_IRQn), 0x03);
    HAL_NVIC_EnableIRQ((IRQn_Type)(EXTI0_IRQn));

    /* Enable Power Clock */
    __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_HSI);

    wakeup_handler = h;
}

void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_0) {
        uint8_t bit = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
        if (wakeup_handler != NULL) {
            wakeup_handler(bit);
        }
    }
}
