#include "camera.h"
#include "i2c_soft.h"
#include <stdint.h>

#define OV5640_SCCB_ADDRESS 0x3c       // OV5640地址
#define OV5640_SCCB_BUS     I2C_SOFT_1 // 使用的I2C总线索引

static inline void ov5640_write_reg16_byte(uint16_t reg, uint8_t data)
{
    uint8_t buf[4] = {
        (uint8_t)OV5640_SCCB_ADDRESS,
        (uint8_t)((reg) >> 8),
        (uint8_t)(reg),
        (uint8_t)data,
    };
    i2c_soft_start(OV5640_SCCB_BUS);
    i2c_soft_write_bytes(OV5640_SCCB_BUS, buf, 4);
    i2c_soft_stop(OV5640_SCCB_BUS);
}

static inline uint8_t ov5640_read_reg16_byte(uint16_t reg)
{
    uint8_t data;
    uint8_t buf[3] = {
        (uint8_t)OV5640_SCCB_ADDRESS,
        (uint8_t)((reg) >> 8),
        (uint8_t)(reg),
    };
    uint8_t read_addr = OV5640_SCCB_ADDRESS << 1 | 0x01; // 7位地址转换为8位地址
    i2c_soft_start(OV5640_SCCB_BUS);
    i2c_soft_write_bytes(OV5640_SCCB_BUS, buf, 2);
    i2c_soft_stop(OV5640_SCCB_BUS);
    i2c_soft_start(OV5640_SCCB_BUS);
    i2c_soft_write_bytes(OV5640_SCCB_BUS, &read_addr, 1);
    i2c_soft_read_bytes(OV5640_SCCB_BUS, &data, 1);
    i2c_soft_stop(OV5640_SCCB_BUS);
    return data;
}

static inline void ov5640_write_reg16_bytes(uint16_t reg, uint8_t* data, uint8_t len)
{
    uint8_t buf[3] = {
        (uint8_t)OV5640_SCCB_ADDRESS,
        (uint8_t)((reg) >> 8),
        (uint8_t)(reg),
    };
    i2c_soft_start(OV5640_SCCB_BUS);
    i2c_soft_write_bytes(OV5640_SCCB_BUS, buf, 3);
    i2c_soft_write_bytes(OV5640_SCCB_BUS, data, len);
    i2c_soft_stop(OV5640_SCCB_BUS);
}

