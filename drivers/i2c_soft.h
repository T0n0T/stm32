#ifndef __I2C_SOFT_H__
#define __I2C_SOFT_H__

#include "board.h"

// I2C状态定义
#define I2C_SOFT_OK    0
#define I2C_SOFT_ERROR 1

// I2C速度设置
#define I2C_SOFT_SPEED 100000 // 100kHz

typedef enum i2c_soft_index {
    I2C_SOFT_1 = 0,
    I2C_SOFT_MAX,
} i2c_soft_index_t;

// I2C引脚信息结构体
typedef struct i2c_soft_struct {
    GPIO_TypeDef* sda_port;
    uint16_t      sda_pin;
    GPIO_TypeDef* scl_port;
    uint16_t      scl_pin;
} i2c_soft_t;

// 函数声明
void    i2c_soft_init(i2c_soft_index_t index);
void    i2c_soft_start(i2c_soft_index_t index);
void    i2c_soft_stop(i2c_soft_index_t index);
uint8_t i2c_soft_write_reg(i2c_soft_index_t index, uint8_t dev_addr, uint8_t reg, uint8_t data);
uint8_t i2c_soft_read_reg(i2c_soft_index_t index, uint8_t dev_addr, uint8_t reg, uint8_t* data);
uint8_t i2c_soft_write_bytes(i2c_soft_index_t index, uint8_t* data, uint16_t len);
uint8_t i2c_soft_read_bytes(i2c_soft_index_t index, uint8_t* data, uint16_t len);

#endif /* __I2C_SOFT_H__ */