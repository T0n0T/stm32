#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "board.h"

typedef enum button_index {
    BUTTON_1 = 0,
    BUTTON_2,
    BUTTON_MAX,
} button_index_t;

typedef struct button_struct {
    GPIO_TypeDef* port;
    uint16_t      pin;
    GPIO_PinState active_level;
} button_t;

void btn_init(void);
GPIO_PinState btn_read(button_index_t index);
uint8_t btn_is_pressed(button_index_t index);

#endif /* __BUTTON_H__ */
