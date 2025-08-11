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

static inline void ov5640_write_reg16_bytes(uint16_t reg, uint8_t* data,
                                            uint8_t len)
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

static inline void ov5640_delay(uint32_t ms)
{
    HAL_Delay(ms);
}

uint16_t camera_read_id(void)
{
    uint16_t id = 0;

    id = ov5640_read_reg16_byte(OV5640_CHIP_ID_LOW_BYTE);
    id |= ov5640_read_reg16_byte(OV5640_CHIP_ID_HIGH_BYTE) << 8;
    return id;
}

void camera_start(void)
{
    ov5640_write_reg16_byte(OV5640_SYSTEM_CTROL0, 0x02);
}

void camera_stop(void)
{
    ov5640_write_reg16_byte(OV5640_SYSTEM_CTROL0, 0x42);
}

static void camera_enable_mode(uint8_t mode)
{
    if (mode == OV5640_MIPI_MODE) {
        ov5640_write_reg16_byte(OV5640_PAD_OUTPUT_ENABLE01, 0x00);
        ov5640_write_reg16_byte(OV5640_PAD_OUTPUT_ENABLE02, 0x00);
        ov5640_write_reg16_byte(0x302e, 0x08);
        ov5640_write_reg16_byte(OV5640_PCLK_PERIOD, 0x23);
        ov5640_write_reg16_byte(OV5640_SC_PLL_CONTRL0, 0x18);
        ov5640_write_reg16_byte(OV5640_SC_PLL_CONTRL1, 0x12);
        ov5640_write_reg16_byte(OV5640_SC_PLL_CONTRL2, 0x1c);
        ov5640_write_reg16_byte(OV5640_SC_PLL_CONTRL3, 0x13);
        ov5640_write_reg16_byte(OV5640_SYSTEM_ROOT_DIVIDER, 0x01);
        ov5640_write_reg16_byte(0x4814, 0x2a);
        ov5640_write_reg16_byte(OV5640_MIPI_CTRL00, 0x24);
        ov5640_write_reg16_byte(OV5640_PAD_OUTPUT_VALUE00, 0x70);
        ov5640_write_reg16_byte(OV5640_MIPI_CONTROL00, 0x45);
        ov5640_write_reg16_byte(OV5640_FRAME_CTRL02, 0x00);
    } else if (mode == OV5640_DVP_MODE) {
        ov5640_write_reg16_byte(OV5640_PAD_OUTPUT_ENABLE01, 0xff);
        ov5640_write_reg16_byte(OV5640_PAD_OUTPUT_ENABLE02, 0xf3);
        ov5640_write_reg16_byte(0x302e, 0x00);
        ov5640_write_reg16_byte(0x471c, 0x50);
        ov5640_write_reg16_byte(OV5640_MIPI_CTRL00, 0x58);
        ov5640_write_reg16_byte(OV5640_SC_PLL_CONTRL0, 0x18);
        ov5640_write_reg16_byte(OV5640_SC_PLL_CONTRL1, 0x41);
        ov5640_write_reg16_byte(OV5640_SC_PLL_CONTRL2, 0x60);
        ov5640_write_reg16_byte(OV5640_SC_PLL_CONTRL3, 0x13);
        ov5640_write_reg16_byte(OV5640_SYSTEM_ROOT_DIVIDER, 0x01);
    }
}

void camera_set_resolution(uint8_t resolution)
{
    switch (resolution) {
        case OV5640_R160x120: // 160x120
            ov5640_write_reg16_byte(OV5640_TIMING_DVPHO_HIGH, 0x00);
            ov5640_write_reg16_byte(OV5640_TIMING_DVPHO_LOW, 0xa0);
            ov5640_write_reg16_byte(OV5640_TIMING_DVPVO_HIGH, 0x00);
            ov5640_write_reg16_byte(OV5640_TIMING_DVPVO_LOW, 0x78);
            break;
        case OV5640_R320x240: // 640x480
            ov5640_write_reg16_byte(OV5640_TIMING_DVPHO_HIGH, 0x01);
            ov5640_write_reg16_byte(OV5640_TIMING_DVPHO_LOW, 0x40);
            ov5640_write_reg16_byte(OV5640_TIMING_DVPVO_HIGH, 0x00);
            ov5640_write_reg16_byte(OV5640_TIMING_DVPVO_LOW, 0xf0);
            break;
        case OV5640_R480x272: // 480x272
            ov5640_write_reg16_byte(OV5640_TIMING_DVPHO_HIGH, 0x01);
            ov5640_write_reg16_byte(OV5640_TIMING_DVPHO_LOW, 0xe0);
            ov5640_write_reg16_byte(OV5640_TIMING_DVPVO_HIGH, 0x01);
            ov5640_write_reg16_byte(OV5640_TIMING_DVPVO_LOW, 0x10);
            break;
        case OV5640_R640x480: // 640x480
            ov5640_write_reg16_byte(OV5640_TIMING_DVPHO_HIGH, 0x02);
            ov5640_write_reg16_byte(OV5640_TIMING_DVPHO_LOW, 0x80);
            ov5640_write_reg16_byte(OV5640_TIMING_DVPVO_HIGH, 0x01);
            ov5640_write_reg16_byte(OV5640_TIMING_DVPVO_LOW, 0xe0);
            break;
        case OV5640_R800x480: // 800x480
            ov5640_write_reg16_byte(OV5640_TIMING_DVPHO_HIGH, 0x03);
            ov5640_write_reg16_byte(OV5640_TIMING_DVPHO_LOW, 0x20);
            ov5640_write_reg16_byte(OV5640_TIMING_DVPVO_HIGH, 0x01);
            ov5640_write_reg16_byte(OV5640_TIMING_DVPVO_LOW, 0xe0);
            break;
        default:
            break;
    }
}

void camera_set_format(uint8_t format)
{
    switch (format) {
        case OV5640_YUV422:
            ov5640_write_reg16_byte(OV5640_FORMAT_CTRL00, 0x30);
            ov5640_delay(1);
            ov5640_write_reg16_byte(OV5640_FORMAT_MUX_CTRL, 0x00);
            ov5640_delay(1);
            break;
        case OV5640_RGB888:
            ov5640_write_reg16_byte(OV5640_FORMAT_CTRL00, 0x23);
            ov5640_delay(1);
            ov5640_write_reg16_byte(OV5640_FORMAT_MUX_CTRL, 0x01);
            ov5640_delay(1);
            break;
        case OV5640_Y8:
            ov5640_write_reg16_byte(OV5640_FORMAT_CTRL00, 0x10);
            ov5640_delay(1);
            ov5640_write_reg16_byte(OV5640_FORMAT_MUX_CTRL, 0x00);
            ov5640_delay(1);
            break;
        case OV5640_JPEG:
            ov5640_write_reg16_byte(OV5640_FORMAT_CTRL00, 0x30);
            ov5640_delay(1);
            ov5640_write_reg16_byte(OV5640_FORMAT_MUX_CTRL, 0x00);
            ov5640_delay(1);
            // make sure jpg enbale
            uint8_t tmp3821 = ov5640_read_reg16_byte(OV5640_TIMING_TC_REG21);
            tmp3821 |= (1 << 5);
            ov5640_write_reg16_byte(OV5640_TIMING_TC_REG21, tmp3821);
            // reset jfifo,sfifo,jpg
            uint8_t tmp3002 = ov5640_read_reg16_byte(OV5640_SYSREM_RESET02);
            tmp3002 &= ~((1 << 2) | (1 << 3) | (1 << 4));
            ov5640_write_reg16_byte(OV5640_SYSREM_RESET02, tmp3002);
            // enable jpeg clock
            uint8_t tmp3006 = ov5640_read_reg16_byte(OV5640_CLOCK_ENABLE02);
            tmp3006 |= ((1 << 3) | (1 << 5));
            ov5640_write_reg16_byte(OV5640_CLOCK_ENABLE02, tmp3006);
            break;
        case OV5640_RGB565:
            ov5640_write_reg16_byte(OV5640_FORMAT_CTRL00, 0x6f);
            ov5640_delay(1);
            ov5640_write_reg16_byte(OV5640_FORMAT_MUX_CTRL, 0x01);
            ov5640_delay(1);
            break;
        default:
            break;
    }
}

void camera_set_polarity(uint8_t pclk_polarity, uint8_t href_polarity,
                         uint8_t vsync_polarity)
{
    uint8_t tmp4740 = ((pclk_polarity << 5) | (href_polarity << 1) | vsync_polarity);
    ov5640_write_reg16_byte(OV5640_POLARITY_CTRL, tmp4740);
}

void camera_set_mirror_flip(uint8_t mode)
{
    uint8_t tmp1 = ov5640_read_reg16_byte(OV5640_TIMING_TC_REG20) & 0xf9;
    uint8_t tmp2 = ov5640_read_reg16_byte(OV5640_TIMING_TC_REG21) & 0xf9;
    switch (mode) {
        case 0: // Normal
            tmp1 |= 0x00;
            tmp2 |= 0x00;
            break;
        case 1: // Mirror
            tmp1 |= 0x00;
            tmp2 |= 0x06;
            break;
        case 2: // Flip
            tmp1 |= 0x06;
            tmp2 |= 0x00;
            break;
        case 3: // Mirror + Flip
            tmp1 |= 0x06;
            tmp2 |= 0x06;
            break;
        default:
            break;
    }
    ov5640_write_reg16_byte(OV5640_TIMING_TC_REG20, tmp1);
    ov5640_write_reg16_byte(OV5640_TIMING_TC_REG21, tmp2);
}

void camera_init(void)
{
    extern void MX_DCMI_Init(void);
    MX_DCMI_Init();

    /* Initialization sequence for OV5640 */
    static const uint16_t OV5640_INIT_SEQ[][2] = {
        {OV5640_SCCB_SYSTEM_CTRL1, 0x11},
        {OV5640_SYSTEM_CTROL0, 0x82},
        {OV5640_SCCB_SYSTEM_CTRL1, 0x03},
        {0x3630, 0x36},
        {0x3631, 0x0e},
        {0x3632, 0xe2},
        {0x3633, 0x12},
        {0x3621, 0xe0},
        {0x3704, 0xa0},
        {0x3703, 0x5a},
        {0x3715, 0x78},
        {0x3717, 0x01},
        {0x370b, 0x60},
        {0x3705, 0x1a},
        {0x3905, 0x02},
        {0x3906, 0x10},
        {0x3901, 0x0a},
        {0x3731, 0x12},
        {0x3600, 0x08},
        {0x3601, 0x33},
        {0x302d, 0x60},
        {0x3620, 0x52},
        {0x371b, 0x20},
        {0x471c, 0x50},
        {OV5640_AEC_CTRL13, 0x43},
        {OV5640_AEC_GAIN_CEILING_HIGH, 0x00},
        {OV5640_AEC_GAIN_CEILING_LOW, 0xf8},
        {0x3635, 0x13},
        {0x3636, 0x03},
        {0x3634, 0x40},
        {0x3622, 0x01},
        {OV5640_5060HZ_CTRL01, 0x34},
        {OV5640_5060HZ_CTRL04, 0x28},
        {OV5640_5060HZ_CTRL05, 0x98},
        {OV5640_LIGHTMETER1_TH_HIGH, 0x00},
        {OV5640_LIGHTMETER1_TH_LOW, 0x00},
        {OV5640_LIGHTMETER2_TH_HIGH, 0x01},
        {OV5640_LIGHTMETER2_TH_LOW, 0x2c},
        {OV5640_SAMPLE_NUMBER_HIGH, 0x9c},
        {OV5640_SAMPLE_NUMBER_LOW, 0x40},
        {OV5640_TIMING_TC_REG20, 0x06},
        {OV5640_TIMING_TC_REG21, 0x00},
        {OV5640_TIMING_X_INC, 0x31},
        {OV5640_TIMING_Y_INC, 0x31},
        {OV5640_TIMING_HS_HIGH, 0x00},
        {OV5640_TIMING_HS_LOW, 0x00},
        {OV5640_TIMING_VS_HIGH, 0x00},
        {OV5640_TIMING_VS_LOW, 0x04},
        // hts * vts = 2623 * 1947(default)
        {OV5640_TIMING_HW_HIGH, 0x0a},
        {OV5640_TIMING_HW_LOW, 0x3f},
        {OV5640_TIMING_VH_HIGH, 0x07},
        {OV5640_TIMING_VH_LOW, 0x9b},
        // h * v = 800*600
        {OV5640_TIMING_DVPHO_HIGH, 0x03},
        {OV5640_TIMING_DVPHO_LOW, 0x20},
        {OV5640_TIMING_DVPVO_HIGH, 0x02},
        {OV5640_TIMING_DVPVO_LOW, 0x58},
        /* For 800x480 resolution: OV5640_TIMING_HTS=0x790,
           OV5640_TIMING_VTS=0x440 */
        {OV5640_TIMING_HTS_HIGH, 0x07},
        {OV5640_TIMING_HTS_LOW, 0x90},
        {OV5640_TIMING_VTS_HIGH, 0x04},
        {OV5640_TIMING_VTS_LOW, 0x40},
        {OV5640_TIMING_HOFFSET_HIGH, 0x00},
        {OV5640_TIMING_HOFFSET_LOW, 0x10},
        {OV5640_TIMING_VOFFSET_HIGH, 0x00},
        {OV5640_TIMING_VOFFSET_LOW, 0x06},
        {0x3618, 0x00},
        {0x3612, 0x29},
        {0x3708, 0x64},
        {0x3709, 0x52},
        {0x370c, 0x03},
        {OV5640_AEC_CTRL02, 0x03},
        {OV5640_AEC_CTRL03, 0xd8},
        {OV5640_AEC_B50_STEP_HIGH, 0x01},
        {OV5640_AEC_B50_STEP_LOW, 0x27},
        {OV5640_AEC_B60_STEP_HIGH, 0x00},
        {OV5640_AEC_B60_STEP_LOW, 0xf6},
        {OV5640_AEC_CTRL0E, 0x03},
        {OV5640_AEC_CTRL0D, 0x04},
        {OV5640_AEC_MAX_EXPO_HIGH, 0x03},
        {OV5640_AEC_MAX_EXPO_LOW, 0xd8},
        {OV5640_BLC_CTRL01, 0x02},
        {OV5640_BLC_CTRL04, 0x02},
        {OV5640_SYSREM_RESET00, 0x00},
        {OV5640_SYSREM_RESET02, 0x1c},
        {OV5640_CLOCK_ENABLE00, 0xff},
        {OV5640_CLOCK_ENABLE02, 0xc3},
        {OV5640_MIPI_CONTROL00, 0x58},
        {0x302e, 0x00},
        {OV5640_POLARITY_CTRL, 0x22},
        {OV5640_FORMAT_CTRL00, 0x6F},
        {OV5640_FORMAT_MUX_CTRL, 0x01},
        {OV5640_JPG_MODE_SELECT, 0x03},
        {OV5640_JPEG_CTRL07, 0x04},
        {0x440e, 0x00},
        {0x460b, 0x35},
        {0x460c, 0x23},
        {OV5640_PCLK_PERIOD, 0x22},
        {0x3824, 0x02},
        {OV5640_ISP_CONTROL00, 0xa7},
        {OV5640_ISP_CONTROL01, 0xa3},
        {OV5640_AWB_CTRL00, 0xff},
        {OV5640_AWB_CTRL01, 0xf2},
        {OV5640_AWB_CTRL02, 0x00},
        {OV5640_AWB_CTRL03, 0x14},
        {OV5640_AWB_CTRL04, 0x25},
        {OV5640_AWB_CTRL05, 0x24},
        {OV5640_AWB_CTRL06, 0x09},
        {OV5640_AWB_CTRL07, 0x09},
        {OV5640_AWB_CTRL08, 0x09},
        {OV5640_AWB_CTRL09, 0x75},
        {OV5640_AWB_CTRL10, 0x54},
        {OV5640_AWB_CTRL11, 0xe0},
        {OV5640_AWB_CTRL12, 0xb2},
        {OV5640_AWB_CTRL13, 0x42},
        {OV5640_AWB_CTRL14, 0x3d},
        {OV5640_AWB_CTRL15, 0x56},
        {OV5640_AWB_CTRL16, 0x46},
        {OV5640_AWB_CTRL17, 0xf8},
        {OV5640_AWB_CTRL18, 0x04},
        {OV5640_AWB_CTRL19, 0x70},
        {OV5640_AWB_CTRL20, 0xf0},
        {OV5640_AWB_CTRL21, 0xf0},
        {OV5640_AWB_CTRL22, 0x03},
        {OV5640_AWB_CTRL23, 0x01},
        {OV5640_AWB_CTRL24, 0x04},
        {OV5640_AWB_CTRL25, 0x12},
        {OV5640_AWB_CTRL26, 0x04},
        {OV5640_AWB_CTRL27, 0x00},
        {OV5640_AWB_CTRL28, 0x06},
        {OV5640_AWB_CTRL29, 0x82},
        {OV5640_AWB_CTRL30, 0x38},
        {OV5640_CMX1, 0x1e},
        {OV5640_CMX2, 0x5b},
        {OV5640_CMX3, 0x08},
        {OV5640_CMX4, 0x0a},
        {OV5640_CMX5, 0x7e},
        {OV5640_CMX6, 0x88},
        {OV5640_CMX7, 0x7c},
        {OV5640_CMX8, 0x6c},
        {OV5640_CMX9, 0x10},
        {OV5640_CMXSIGN_HIGH, 0x01},
        {OV5640_CMXSIGN_LOW, 0x98},
        {OV5640_CIP_SHARPENMT_TH1, 0x08},
        {OV5640_CIP_SHARPENMT_TH2, 0x30},
        {OV5640_CIP_SHARPENMT_OFFSET1, 0x10},
        {OV5640_CIP_SHARPENMT_OFFSET2, 0x00},
        {OV5640_CIP_DNS_TH1, 0x08},
        {OV5640_CIP_DNS_TH2, 0x30},
        {OV5640_CIP_DNS_OFFSET1, 0x08},
        {OV5640_CIP_DNS_OFFSET2, 0x16},
        {OV5640_CIP_CTRL, 0x08},
        {OV5640_CIP_SHARPENTH_TH1, 0x30},
        {OV5640_CIP_SHARPENTH_TH2, 0x04},
        {OV5640_CIP_SHARPENTH_OFFSET1, 0x06},
        {OV5640_GAMMA_CTRL00, 0x01},
        {OV5640_GAMMA_YST00, 0x08},
        {OV5640_GAMMA_YST01, 0x14},
        {OV5640_GAMMA_YST02, 0x28},
        {OV5640_GAMMA_YST03, 0x51},
        {OV5640_GAMMA_YST04, 0x65},
        {OV5640_GAMMA_YST05, 0x71},
        {OV5640_GAMMA_YST06, 0x7d},
        {OV5640_GAMMA_YST07, 0x87},
        {OV5640_GAMMA_YST08, 0x91},
        {OV5640_GAMMA_YST09, 0x9a},
        {OV5640_GAMMA_YST0A, 0xaa},
        {OV5640_GAMMA_YST0B, 0xb8},
        {OV5640_GAMMA_YST0C, 0xcd},
        {OV5640_GAMMA_YST0D, 0xdd},
        {OV5640_GAMMA_YST0E, 0xea},
        {OV5640_GAMMA_YST0F, 0x1d},
        {OV5640_SDE_CTRL0, 0x02},
        {OV5640_SDE_CTRL3, 0x40},
        {OV5640_SDE_CTRL4, 0x10},
        {OV5640_SDE_CTRL9, 0x10},
        {OV5640_SDE_CTRL10, 0x00},
        {OV5640_SDE_CTRL11, 0xf8},
        {OV5640_GMTRX00, 0x23},
        {OV5640_GMTRX01, 0x14},
        {OV5640_GMTRX02, 0x0f},
        {OV5640_GMTRX03, 0x0f},
        {OV5640_GMTRX04, 0x12},
        {OV5640_GMTRX05, 0x26},
        {OV5640_GMTRX10, 0x0c},
        {OV5640_GMTRX11, 0x08},
        {OV5640_GMTRX12, 0x05},
        {OV5640_GMTRX13, 0x05},
        {OV5640_GMTRX14, 0x08},
        {OV5640_GMTRX15, 0x0d},
        {OV5640_GMTRX20, 0x08},
        {OV5640_GMTRX21, 0x03},
        {OV5640_GMTRX22, 0x00},
        {OV5640_GMTRX23, 0x00},
        {OV5640_GMTRX24, 0x03},
        {OV5640_GMTRX25, 0x09},
        {OV5640_GMTRX30, 0x07},
        {OV5640_GMTRX31, 0x03},
        {OV5640_GMTRX32, 0x00},
        {OV5640_GMTRX33, 0x01},
        {OV5640_GMTRX34, 0x03},
        {OV5640_GMTRX35, 0x08},
        {OV5640_GMTRX40, 0x0d},
        {OV5640_GMTRX41, 0x08},
        {OV5640_GMTRX42, 0x05},
        {OV5640_GMTRX43, 0x06},
        {OV5640_GMTRX44, 0x08},
        {OV5640_GMTRX45, 0x0e},
        {OV5640_GMTRX50, 0x29},
        {OV5640_GMTRX51, 0x17},
        {OV5640_GMTRX52, 0x11},
        {OV5640_GMTRX53, 0x11},
        {OV5640_GMTRX54, 0x15},
        {OV5640_GMTRX55, 0x28},
        {OV5640_BRMATRX00, 0x46},
        {OV5640_BRMATRX01, 0x26},
        {OV5640_BRMATRX02, 0x08},
        {OV5640_BRMATRX03, 0x26},
        {OV5640_BRMATRX04, 0x64},
        {OV5640_BRMATRX05, 0x26},
        {OV5640_BRMATRX06, 0x24},
        {OV5640_BRMATRX07, 0x22},
        {OV5640_BRMATRX08, 0x24},
        {OV5640_BRMATRX09, 0x24},
        {OV5640_BRMATRX20, 0x06},
        {OV5640_BRMATRX21, 0x22},
        {OV5640_BRMATRX22, 0x40},
        {OV5640_BRMATRX23, 0x42},
        {OV5640_BRMATRX24, 0x24},
        {OV5640_BRMATRX30, 0x26},
        {OV5640_BRMATRX31, 0x24},
        {OV5640_BRMATRX32, 0x22},
        {OV5640_BRMATRX33, 0x22},
        {OV5640_BRMATRX34, 0x26},
        {OV5640_BRMATRX40, 0x44},
        {OV5640_BRMATRX41, 0x24},
        {OV5640_BRMATRX42, 0x26},
        {OV5640_BRMATRX43, 0x28},
        {OV5640_BRMATRX44, 0x42},
        {OV5640_LENC_BR_OFFSET, 0xce},
        {0x5025, 0x00},
        {OV5640_AEC_CTRL0F, 0x30},
        {OV5640_AEC_CTRL10, 0x28},
        {OV5640_AEC_CTRL1B, 0x30},
        {OV5640_AEC_CTRL1E, 0x26},
        {OV5640_AEC_CTRL11, 0x60},
        {OV5640_AEC_CTRL1F, 0x14},
        {OV5640_SYSTEM_CTROL0, 0x02},
    };
    for (uint32_t i = 0; i < sizeof(OV5640_INIT_SEQ) / sizeof(OV5640_INIT_SEQ[0]); i++) {
        ov5640_write_reg16_byte(OV5640_INIT_SEQ[i][0], OV5640_INIT_SEQ[i][1]);
    }
    camera_enable_mode(OV5640_DVP_MODE);
    // camera_set_resolution(OV5640_R640x480);
    camera_set_format(OV5640_RGB565);
    camera_set_polarity(OV5640_POLARITY_PCLK_HIGH,
                        OV5640_POLARITY_HREF_LOW,
                        OV5640_POLARITY_VSYNC_LOW); // PCLK, HREF, VSYNC polarity
}
