#ifndef __USART_H__
#define __USART_H__

#include "board.h"

typedef enum usart_index {
    USART_DEBUG = 0,
    USART_MAX,
} usart_index_t;

typedef struct usart_struct {
    UART_HandleTypeDef huart;
} usart_t;

void usart_init(void);
void usart_send(usart_index_t index, uint8_t* data, size_t len);
#endif /* __USART_H__ */