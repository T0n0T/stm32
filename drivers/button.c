#include "button.h"

static button_t buttons[BUTTON_MAX] = {
    {
        .port         = GPIOA,
        .pin          = GPIO_PIN_0,
        .active_level = GPIO_PIN_SET,
    },
    {
        .port         = GPIOC,
        .pin          = GPIO_PIN_13,
        .active_level = GPIO_PIN_SET,
    },
};

void btn_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    for (size_t i = 0; i < BUTTON_MAX; i++) {
        gpio_clk_init(buttons[i].port);
        GPIO_InitStruct.Pin   = buttons[i].pin;
        GPIO_InitStruct.Pull  = GPIO_PULLUP;
        GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(buttons[i].port, &GPIO_InitStruct);
    }
}

GPIO_PinState btn_read(button_index_t index)
{
    return HAL_GPIO_ReadPin(buttons[index].port, buttons[index].pin);
}

uint8_t btn_is_pressed(button_index_t index)
{
    return (btn_read(index) == buttons[index].active_level);
}