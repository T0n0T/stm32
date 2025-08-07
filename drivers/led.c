#include "led.h"

static led_t leds[LED_MAX] = {
    {
        .port         = GPIOG,
        .pin          = GPIO_PIN_7,
        .active_level = GPIO_PIN_RESET,
    },
};

void led_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    for (size_t i = 0; i < LED_MAX; i++) {
        gpio_clk_init(leds[i].port);
        GPIO_InitStruct.Pin   = leds[i].pin;
        GPIO_InitStruct.Pull  = GPIO_NOPULL;
        GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(leds[i].port, &GPIO_InitStruct);
        HAL_GPIO_WritePin(leds[i].port, leds[i].pin, !leds[i].active_level);
    }
}

void led_on(led_index_t index)
{
    HAL_GPIO_WritePin(leds[index].port, leds[index].pin, leds[index].active_level);
}

void led_off(led_index_t index)
{
    HAL_GPIO_WritePin(leds[index].port, leds[index].pin, !leds[index].active_level);
}

void led_toggle(led_index_t index)
{
    HAL_GPIO_TogglePin(leds[index].port, leds[index].pin);
}