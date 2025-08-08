#include "i2c_soft.h"
#include "stm32h723xx.h"
#include "stm32h7xx_hal_gpio.h"

// GPIO寄存器操作宏定义
#define GPIO_SET_PIN(port, pin)   ((port)->BSRR = (pin))
#define GPIO_RESET_PIN(port, pin) ((port)->BSRR = ((pin) << 16))
#define GPIO_READ_PIN(port, pin)  ((port)->IDR & (pin))

static i2c_soft_t i2c_soft_ins[I2C_SOFT_MAX] = {
    {
        .sda_port = GPIOF,
        .sda_pin  = GPIO_PIN_15,
        .scl_port = GPIOF,
        .scl_pin  = GPIO_PIN_14,
    },
};

// 延迟函数，用于控制I2C时序
static void i2c_soft_delay(void)
{
    // 根据系统时钟调整延时
    for (volatile int i = 0; i < 60; i++);
}

// 配置SDA为输出
static void i2c_soft_sda_out(i2c_soft_t* config)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    gpio_clk_init(config->sda_port);
    GPIO_InitStruct.Pin   = config->sda_pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD; // 开漏输出
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(config->sda_port, &GPIO_InitStruct);
}

// 配置SCL为输出
static void i2c_soft_scl_out(i2c_soft_t* config)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    gpio_clk_init(config->scl_port);
    GPIO_InitStruct.Pin   = config->scl_pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD; // 开漏输出
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(config->scl_port, &GPIO_InitStruct);
}

// 初始化I2C引脚
void i2c_soft_init(i2c_soft_index_t index)
{
    if (index >= I2C_SOFT_MAX) {
        return;
    }
    i2c_soft_t* config = &i2c_soft_ins[index];
    // 配置SDA和SCL为开漏输出
    i2c_soft_sda_out(config);
    i2c_soft_scl_out(config);

    // 设置初始状态为高电平
    GPIO_SET_PIN(config->sda_port, config->sda_pin);
    GPIO_SET_PIN(config->scl_port, config->scl_pin);
}

// 产生起始信号
static inline void _i2c_soft_start(i2c_soft_t* config)
{
    GPIO_SET_PIN(config->sda_port, config->sda_pin);
    GPIO_SET_PIN(config->scl_port, config->scl_pin);
    i2c_soft_delay();
    GPIO_RESET_PIN(config->sda_port, config->sda_pin);
    i2c_soft_delay();
    GPIO_RESET_PIN(config->scl_port, config->scl_pin);
    i2c_soft_delay();
}

// 产生停止信号
static inline void _i2c_soft_stop(i2c_soft_t* config)
{
    GPIO_RESET_PIN(config->sda_port, config->sda_pin);
    GPIO_SET_PIN(config->scl_port, config->scl_pin);
    i2c_soft_delay();
    GPIO_SET_PIN(config->sda_port, config->sda_pin);
    i2c_soft_delay();
}

// 写一个字节
static inline uint8_t _i2c_soft_write_byte(i2c_soft_t* config, uint8_t data)
{
    uint8_t i;

    for (i = 0; i < 8; i++) {
        if (data & 0x80) {
            GPIO_SET_PIN(config->sda_port, config->sda_pin);
        } else {
            GPIO_RESET_PIN(config->sda_port, config->sda_pin);
        }
        data <<= 1;
        i2c_soft_delay();
        GPIO_SET_PIN(config->scl_port, config->scl_pin);
        i2c_soft_delay();
        GPIO_RESET_PIN(config->scl_port, config->scl_pin);
        i2c_soft_delay();
    }

    // 释放SDA线，准备接收ACK
    GPIO_SET_PIN(config->sda_port, config->sda_pin);
    i2c_soft_delay();
    GPIO_SET_PIN(config->scl_port, config->scl_pin);
    i2c_soft_delay();

    // 读取ACK信号
    uint8_t ack = GPIO_READ_PIN(config->sda_port, config->sda_pin) ? 1 : 0;

    GPIO_RESET_PIN(config->scl_port, config->scl_pin);
    i2c_soft_delay();

    return ack;
}

// 读一个字节
static inline uint8_t _i2c_soft_read_byte(i2c_soft_t* config, uint8_t ack)
{
    uint8_t i, data = 0;

    // 循环读取8位数据
    for (i = 0; i < 8; i++) {
        // SCL拉高，准备读取数据
        GPIO_SET_PIN(config->scl_port, config->scl_pin);
        // 延时，确保信号稳定
        i2c_soft_delay();
        // 数据左移一位，为新数据腾出空间
        data <<= 1;
        // 读取SDA线上的数据位
        if (GPIO_READ_PIN(config->sda_port, config->sda_pin)) {
            // 如果SDA为高电平，将最低位设置为1
            data |= 0x01;
        }
        // SCL拉低，完成一个时钟周期
        GPIO_RESET_PIN(config->scl_port, config->scl_pin);
        // 延时，确保信号稳定
        i2c_soft_delay();
    }

    // 发送ACK或NACK
    i2c_soft_sda_out(config);
    if (ack) {
        GPIO_RESET_PIN(config->sda_port, config->sda_pin); // ACK
    } else {
        GPIO_SET_PIN(config->sda_port, config->sda_pin); // NACK
    }
    i2c_soft_delay();
    GPIO_SET_PIN(config->scl_port, config->scl_pin);
    i2c_soft_delay();
    GPIO_RESET_PIN(config->scl_port, config->scl_pin);
    i2c_soft_delay();
    GPIO_SET_PIN(config->sda_port, config->sda_pin);

    return data;
}

void i2c_soft_start(i2c_soft_index_t index)
{
    if (index >= I2C_SOFT_MAX) {
        return;
    }
    i2c_soft_t* config = &i2c_soft_ins[index];
    _i2c_soft_start(config);
}

void i2c_soft_stop(i2c_soft_index_t index)
{
    if (index >= I2C_SOFT_MAX) {
        return;
    }
    i2c_soft_t* config = &i2c_soft_ins[index];
    _i2c_soft_stop(config);
}

// 写多个字节
uint8_t i2c_soft_write_bytes(i2c_soft_index_t index, uint8_t* data, uint16_t len)
{
    if (index >= I2C_SOFT_MAX) {
        return I2C_SOFT_ERROR; // 索引越界
    }
    i2c_soft_t* config = &i2c_soft_ins[index];

    // 发送数据
    for (uint16_t i = 0; i < len; i++) {
        if (_i2c_soft_write_byte(config, data[i])) {
            _i2c_soft_stop(config);
            return I2C_SOFT_ERROR;
        }
    }
    return I2C_SOFT_OK;
}

// 读多个字节
uint8_t i2c_soft_read_bytes(i2c_soft_index_t index, uint8_t* data, uint16_t len)
{
    if (index >= I2C_SOFT_MAX) {
        return I2C_SOFT_ERROR; // 索引越界
    }
    i2c_soft_t* config = &i2c_soft_ins[index];

    // 读取数据
    for (uint16_t i = 0; i < len; i++) {
        // 对于最后一个字节，发送NACK，否则发送ACK
        data[i] = _i2c_soft_read_byte(config, i < (len - 1) ? 1 : 0);
    }

    return I2C_SOFT_OK;
}

// 写寄存器
uint8_t i2c_soft_write_reg(i2c_soft_index_t index, uint8_t dev_addr, uint8_t reg, uint8_t data)
{
    if (index >= I2C_SOFT_MAX) {
        return I2C_SOFT_ERROR; // 索引越界
    }
    i2c_soft_t* config = &i2c_soft_ins[index];
    _i2c_soft_start(config);

    // 发送设备地址和写标志
    if (_i2c_soft_write_byte(config, dev_addr << 1)) {
        _i2c_soft_stop(config);
        return I2C_SOFT_ERROR;
    }

    // 发送寄存器地址
    if (_i2c_soft_write_byte(config, reg)) {
        _i2c_soft_stop(config);
        return I2C_SOFT_ERROR;
    }

    // 发送数据
    if (_i2c_soft_write_byte(config, data)) {
        _i2c_soft_stop(config);
        return I2C_SOFT_ERROR;
    }

    _i2c_soft_stop(config);
    return I2C_SOFT_OK;
}

// 读寄存器
uint8_t i2c_soft_read_reg(i2c_soft_index_t index, uint8_t dev_addr, uint8_t reg, uint8_t* data)
{
    if (index >= I2C_SOFT_MAX) {
        return I2C_SOFT_ERROR; // 索引越界
    }
    i2c_soft_t* config = &i2c_soft_ins[index];
    _i2c_soft_start(config);

    // 发送设备地址和写标志
    if (_i2c_soft_write_byte(config, dev_addr << 1)) {
        _i2c_soft_stop(config);
        return I2C_SOFT_ERROR;
    }

    // 发送寄存器地址
    if (_i2c_soft_write_byte(config, reg)) {
        _i2c_soft_stop(config);
        return I2C_SOFT_ERROR;
    }

    // 重新启动
    _i2c_soft_start(config);

    // 发送设备地址和读标志
    if (_i2c_soft_write_byte(config, (dev_addr << 1) | 0x01)) {
        _i2c_soft_stop(config);
        return I2C_SOFT_ERROR;
    }

    // 读取数据并发送NACK
    *data = _i2c_soft_read_byte(config, 0);

    _i2c_soft_stop(config);
    return I2C_SOFT_OK;
}
