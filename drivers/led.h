#ifndef __LED_H__
#define __LED_H__

#include "board.h"

#define RUN_LED_PORT GPIOB
#define RUN_LED_PIN  GPIO_PIN_4

typedef enum led_index {
    LED_RUN = 0,
    LED_ERR,
    LED_MAX,
} led_index_t;

typedef struct led_struct {
    GPIO_TypeDef* port;
    uint16_t      pin;
    GPIO_PinState active_level;
} led_t;

void led_init(void);
void led_on(led_index_t index);
void led_off(led_index_t index);
void led_toggle(led_index_t index);

#endif /* __LED_H__ */