#include "usart.h"

static usart_t usarts_ins[USART_MAX] = {
    {
        .huart.Instance        = USART1,
        .huart.Init.BaudRate   = 115200,
        .huart.Init.WordLength = UART_WORDLENGTH_8B,
        .huart.Init.StopBits   = UART_STOPBITS_1,
        .huart.Init.Parity     = UART_PARITY_NONE,
        .huart.Init.Mode       = UART_MODE_TX_RX,
        .huart.Init.HwFlowCtl  = UART_HWCONTROL_NONE,
    },
};

void USART1_IRQHandler(void)
{
}

void usart_init(void)
{
    for (size_t i = 0; i < USART_MAX; i++) {
        HAL_UART_Init(&usarts_ins[i].huart);
    }
}

void usart_send(usart_index_t index, uint8_t* data, size_t len)
{
    HAL_UART_Transmit(&usarts_ins[index].huart, data, len, HAL_MAX_DELAY);
}