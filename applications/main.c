#include "board.h"
#include "led.h"
#include "usart.h"

int main(void)
{
    board_init(); // Initialize the board and system clock
    led_init();   // Initialize the LED
    usart_init();
    while (1)
    {
        led_toggle(LED_RUN); // Toggle the LED state
        printf("Hello, World!\r\n"); // Print message to USART
        HAL_Delay(1000); // Delay for 1 second
    }
    
    return 0;
}

