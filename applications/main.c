#include <stdio.h>

extern void HAL_UART_MspInit(void);

int main(void)
{
    HAL_UART_MspInit();
    return 0;
}

