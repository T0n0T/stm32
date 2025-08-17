#include "camera.h"
#include "board.h"
#include "i2c_soft.h"
#include <stdint.h>
#include <stdio.h>

#define OV5640_SCCB_ADDRESS 0x78       // OV5640地址
#define OV5640_SCCB_BUS     I2C_SOFT_1 // 使用的I2C总线索引
#define OV5640_DELAY_MS(x)  HAL_Delay(x)

extern DCMI_HandleTypeDef hdcmi;
extern DMA_HandleTypeDef  hdma_dcmi;

camera_ins_t camera_ins;

void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef* hdcmi)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hdcmi);

    camera_ins.capture_ok = 1;
}

static inline void ov5640_write_reg16_byte(uint16_t reg, uint8_t data)
{
    uint8_t buf[4] = {
        (uint8_t)OV5640_SCCB_ADDRESS,
        (uint8_t)((reg) >> 8),
        (uint8_t)(reg),
        (uint8_t)data,
    };
    i2c_soft_start(OV5640_SCCB_BUS);
    if (i2c_soft_write_bytes(OV5640_SCCB_BUS, buf, 4) != 0) {
        printf("ov5640 write reg 0x%04x failed\r\n", reg);
    }
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
    uint8_t read_addr = OV5640_SCCB_ADDRESS | 0x01;
    i2c_soft_start(OV5640_SCCB_BUS);
    if (i2c_soft_write_bytes(OV5640_SCCB_BUS, buf, 3) != 0) {
        printf("ov5640_read_reg16_byte failed 1\r\n");
    }
    i2c_soft_stop(OV5640_SCCB_BUS);
    i2c_soft_start(OV5640_SCCB_BUS);
    if (i2c_soft_write_bytes(OV5640_SCCB_BUS, &read_addr, 1) != 0) {
        printf("ov5640_read_reg16_byte failed 2\r\n");
    }
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

static uint16_t camera_read_id(void)
{
    uint16_t id = 0;

    id = ov5640_read_reg16_byte(OV5640_CHIP_ID_LOW_BYTE);
    id |= ov5640_read_reg16_byte(OV5640_CHIP_ID_HIGH_BYTE) << 8;
    return id;
}

void camera_start(uint32_t buffer_address, uint32_t buffer_size, uint8_t is_predictive)
{
    // HAL_DCMI_Resume(&hdcmi);
    // HAL_DMA_DeInit(&hdma_dcmi);
    // HAL_DMA_Init(&hdma_dcmi);

    if (is_predictive) {
        HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_CONTINUOUS, buffer_address,
                           buffer_size);
    } else {
        HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_SNAPSHOT, buffer_address,
                           buffer_size);
    }
    camera_ins.capture_ok = 0;
}

void camera_stop(void)
{
    HAL_DCMI_Suspend(&hdcmi);
    // HAL_DMA_DeInit(&hdma_dcmi);
}

void camera_crop(uint16_t want_x, uint16_t want_y)
{
    uint16_t x = 440;
    uint16_t y = 330;
    // switch (camera_ins.resolution) {
    //     case OV5640_R160x120: // 160x120
    //         x = 160;
    //         y = 120;
    //         break;
    //     case OV5640_R320x240: // 320x240
    //         x = 320;
    //         y = 240;
    //         break;
    //     case OV5640_R480x272: // 480x272
    //         x = 480;
    //         y = 272;
    //         break;
    //     case OV5640_R640x480: // 640x480
    //         x = 640;
    //         y = 480;
    //         break;
    //     default:
    //         return;
    // }
    HAL_DCMI_ConfigCrop(&hdcmi, x - want_x, (y - want_y) / 2 - 1, want_x * 2 - 1, want_y - 1);
    HAL_DCMI_EnableCrop(&hdcmi);
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

void camera_set_polarity(uint8_t pclk_polarity, uint8_t href_polarity,
                         uint8_t vsync_polarity)
{
    uint8_t tmp4740 =
        ((pclk_polarity << 5) | (href_polarity << 1) | vsync_polarity);
    ov5640_write_reg16_byte(OV5640_POLARITY_CTRL, tmp4740);
}

void camera_set_resolution(uint8_t resolution)
{
    camera_ins.resolution = resolution;
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
            // YUV VGA 30fps, night mode 5fps
            // Input Clock = 24Mhz, PCLK = 56MHz
            ov5640_write_reg16_byte(OV5640_TIMING_X_INC, 0x31);       // X INC
            ov5640_write_reg16_byte(OV5640_TIMING_Y_INC, 0x31);       // Y INC
            ov5640_write_reg16_byte(OV5640_TIMING_HS_HIGH, 0x00);     // HS
            ov5640_write_reg16_byte(OV5640_TIMING_HS_LOW, 0x00);      // HS
            ov5640_write_reg16_byte(OV5640_TIMING_VS_HIGH, 0x00);     // VS
            ov5640_write_reg16_byte(OV5640_TIMING_VS_LOW, 0x04);      // VS
            ov5640_write_reg16_byte(OV5640_TIMING_HW_HIGH, 0x0a);     // HW (HE)
            ov5640_write_reg16_byte(OV5640_TIMING_VH_HIGH, 0x07);     // VH (VE)
            ov5640_write_reg16_byte(OV5640_TIMING_VH_LOW, 0x9b);      // VH (VE)
            ov5640_write_reg16_byte(OV5640_TIMING_DVPHO_HIGH, 0x02);  // DVPHO
            ov5640_write_reg16_byte(OV5640_TIMING_DVPHO_LOW, 0x80);   // DVPHO
            ov5640_write_reg16_byte(OV5640_TIMING_DVPVO_HIGH, 0x01);  // DVPVO
            ov5640_write_reg16_byte(OV5640_TIMING_DVPVO_LOW, 0xe0);   // DVPVO
            ov5640_write_reg16_byte(OV5640_TIMING_HTS_HIGH, 0x07);    // HTS
            ov5640_write_reg16_byte(OV5640_TIMING_HTS_LOW, 0x68);     // HTS
            ov5640_write_reg16_byte(OV5640_TIMING_VTS_HIGH, 0x03);    // VTS
            ov5640_write_reg16_byte(OV5640_TIMING_VTS_LOW, 0xd8);     // VTS
            ov5640_write_reg16_byte(OV5640_TIMING_VOFFSET_LOW, 0x06); // Timing Voffset

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
    ov5640_write_reg16_byte(OV5640_SC_PLL_CONTRL1, 0x11);     // PLL
    ov5640_write_reg16_byte(OV5640_SC_PLL_CONTRL2, 0x46);     // PLL
    ov5640_write_reg16_byte(OV5640_LIGHTMETER1_TH_LOW, 0x08); // light meter 1 threshold [7:0]
    ov5640_write_reg16_byte(OV5640_TIMING_TC_REG20, 0x41);    // Sensor flip off, ISP flip on
    ov5640_write_reg16_byte(OV5640_TIMING_TC_REG21, 0x07);    // Sensor mirror on, ISP mirror on, H binning on

    ov5640_write_reg16_byte(0x3618, 0x00);
    ov5640_write_reg16_byte(0x3612, 0x29);
    ov5640_write_reg16_byte(0x3709, 0x52);
    ov5640_write_reg16_byte(0x370c, 0x03);
    ov5640_write_reg16_byte(OV5640_AEC_CTRL02, 0x17); // 60Hz max exposure, night mode 5fps
    ov5640_write_reg16_byte(OV5640_AEC_CTRL03, 0x10); // 60Hz max exposure
    // banding filters are calculated automatically in camera driver
    // ov5640_write_reg16_byte(OV5640_AEC_B50_STEP_HIGH, 0x01); // B50 step
    // ov5640_write_reg16_byte(OV5640_AEC_B50_STEP_LOW, 0x27); // B50 step
    // ov5640_write_reg16_byte(OV5640_AEC_B60_STEP_HIGH, 0x00); // B60 step
    // ov5640_write_reg16_byte(OV5640_AEC_B60_STEP_LOW, 0xf6); // B60 step
    // ov5640_write_reg16_byte(OV5640_AEC_CTRL0D, 0x04); // 60Hz max band
    // ov5640_write_reg16_byte(OV5640_AEC_CTRL0E, 0x03); // 50Hz max band
    ov5640_write_reg16_byte(OV5640_AEC_MAX_EXPO_HIGH, 0x17); // 50Hz max exposure, night mode 5fps
    ov5640_write_reg16_byte(OV5640_AEC_MAX_EXPO_LOW, 0x10);  // 50Hz max exposure
    ov5640_write_reg16_byte(OV5640_BLC_CTRL04, 0x02);        // BLC 2 lines
    ov5640_write_reg16_byte(OV5640_SYSREM_RESET02, 0x1c);    // reset JFIFO, SFIFO, JPEG
    ov5640_write_reg16_byte(OV5640_CLOCK_ENABLE02, 0xc3);    // disable clock of JPEG2x, JPEG
    ov5640_write_reg16_byte(OV5640_JPG_MODE_SELECT, 0x03);   // JPEG mode 3
    ov5640_write_reg16_byte(OV5640_JPEG_CTRL07, 0x04);       // Quantization scale
    ov5640_write_reg16_byte(0x460b, 0x35);
    ov5640_write_reg16_byte(OV5640_VFIFO_CTRL0C, 0x22);
    ov5640_write_reg16_byte(OV5640_PCLK_PERIOD, 0x22);   // DVP CLK divider
    ov5640_write_reg16_byte(0x3824, 0x02);               // DVP CLK divider
    ov5640_write_reg16_byte(OV5640_ISP_CONTROL01, 0xa3); // SDE on, scale on, UV average off, color matrix on, AWB on
    ov5640_write_reg16_byte(OV5640_AEC_PK_MANUAL, 0x00); // AEC/AGC on
}

void camera_set_format(uint8_t format)
{
    switch (format) {
        case OV5640_YUV422:
            ov5640_write_reg16_byte(OV5640_FORMAT_CTRL00, 0x30);
            OV5640_DELAY_MS(1);
            ov5640_write_reg16_byte(OV5640_FORMAT_MUX_CTRL, 0x00);
            OV5640_DELAY_MS(1);
            break;
        case OV5640_RGB888:
            ov5640_write_reg16_byte(OV5640_FORMAT_CTRL00, 0x23);
            OV5640_DELAY_MS(1);
            ov5640_write_reg16_byte(OV5640_FORMAT_MUX_CTRL, 0x01);
            OV5640_DELAY_MS(1);
            break;
        case OV5640_Y8:
            ov5640_write_reg16_byte(OV5640_FORMAT_CTRL00, 0x10);
            OV5640_DELAY_MS(1);
            ov5640_write_reg16_byte(OV5640_FORMAT_MUX_CTRL, 0x00);
            OV5640_DELAY_MS(1);
            break;
        case OV5640_JPEG:
            ov5640_write_reg16_byte(OV5640_FORMAT_CTRL00, 0x30);
            OV5640_DELAY_MS(1);
            ov5640_write_reg16_byte(OV5640_FORMAT_MUX_CTRL, 0x00);
            OV5640_DELAY_MS(1);
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
            OV5640_DELAY_MS(1);
            ov5640_write_reg16_byte(OV5640_FORMAT_MUX_CTRL, 0x01);
            OV5640_DELAY_MS(1);
            break;
        default:
            break;
    }
}

void camera_set_brightness(uint8_t level)
{
    static const uint16_t OV5640_BRIGHTNESS_REG_VAL[][2] = {
        {0x40, 0x09}, /* -4 */
        {0x30, 0x09}, /* -3 */
        {0x20, 0x09}, /* -2 */
        {0x10, 0x09}, /* -1 */
        {0x00, 0x01}, /* +0 */
        {0x10, 0x01}, /* +1 */
        {0x20, 0x01}, /* +2 */
        {0x30, 0x01}, /* +3 */
        {0x40, 0x01}, /* +4 */
    };
    ov5640_write_reg16_byte(OV5640_SRM_GROUP_ACCESS, 0X03);

    ov5640_write_reg16_byte(OV5640_SDE_CTRL7,
                            OV5640_BRIGHTNESS_REG_VAL[level][0]);
    ov5640_write_reg16_byte(OV5640_SDE_CTRL8,
                            OV5640_BRIGHTNESS_REG_VAL[level][1]);

    ov5640_write_reg16_byte(OV5640_SRM_GROUP_ACCESS, 0X13);
    ov5640_write_reg16_byte(OV5640_SRM_GROUP_ACCESS, 0Xa3);
}

void camera_set_contrast(uint8_t level)
{
    static const uint16_t OV5640_CONTRAST_REG_VAL[][2] = {
        {0x14, 0x14}, /* -3 */
        {0x18, 0x18}, /* -2 */
        {0x1c, 0x1c}, /* -1 */
        {0x00, 0x20}, /* +0 */
        {0x10, 0x24}, /* +1 */
        {0x18, 0x28}, /* +2 */
        {0x1c, 0x2c}, /* +3 */
    };
    ov5640_write_reg16_byte(OV5640_SRM_GROUP_ACCESS, 0X03);

    ov5640_write_reg16_byte(OV5640_SDE_CTRL5, OV5640_CONTRAST_REG_VAL[level][0]);
    ov5640_write_reg16_byte(OV5640_SDE_CTRL6, OV5640_CONTRAST_REG_VAL[level][1]);

    ov5640_write_reg16_byte(OV5640_SRM_GROUP_ACCESS, 0X13);
    ov5640_write_reg16_byte(OV5640_SRM_GROUP_ACCESS, 0Xa3);
}

void camera_set_saturation(uint8_t level)
{
    static const uint16_t OV5640_SATURATION_REG_VAL[][11] = {
        {0x1c, 0x5a, 0x06, 0x0c, 0x30, 0x3d, 0x3e, 0x3d, 0x01, 0x98,
         0x01}, /* -3 */
        {0x1c, 0x5a, 0x06, 0x10, 0x3d, 0x4d, 0x4e, 0x4d, 0x01, 0x98,
         0x01}, /* -2 */
        {0x1c, 0x5a, 0x06, 0x15, 0x52, 0x66, 0x68, 0x66, 0x02, 0x98,
         0x01}, /* -1 */
        {0x1c, 0x5a, 0x06, 0x1a, 0x66, 0x80, 0x82, 0x80, 0x02, 0x98,
         0x01}, /* +0 */
        {0x1c, 0x5a, 0x06, 0x1f, 0x7a, 0x9a, 0x9c, 0x9a, 0x02, 0x98,
         0x01}, /* +1 */
        {0x1c, 0x5a, 0x06, 0x24, 0x8f, 0xb3, 0xb6, 0xb3, 0x03, 0x98,
         0x01}, /* +2 */
        {0x1c, 0x5a, 0x06, 0x2b, 0xab, 0xd6, 0xda, 0xd6, 0x04, 0x98,
         0x01}, /* +3 */
    };
    ov5640_write_reg16_byte(OV5640_SRM_GROUP_ACCESS, 0X03);

    ov5640_write_reg16_byte(OV5640_CMX1, OV5640_SATURATION_REG_VAL[level][0]);
    ov5640_write_reg16_byte(OV5640_CMX2, OV5640_SATURATION_REG_VAL[level][1]);
    ov5640_write_reg16_byte(OV5640_CMX3, OV5640_SATURATION_REG_VAL[level][2]);
    ov5640_write_reg16_byte(OV5640_CMX4, OV5640_SATURATION_REG_VAL[level][3]);
    ov5640_write_reg16_byte(OV5640_CMX5, OV5640_SATURATION_REG_VAL[level][4]);
    ov5640_write_reg16_byte(OV5640_CMX6, OV5640_SATURATION_REG_VAL[level][5]);
    ov5640_write_reg16_byte(OV5640_CMX7, OV5640_SATURATION_REG_VAL[level][6]);
    ov5640_write_reg16_byte(OV5640_CMX8, OV5640_SATURATION_REG_VAL[level][7]);
    ov5640_write_reg16_byte(OV5640_CMX9, OV5640_SATURATION_REG_VAL[level][8]);
    ov5640_write_reg16_byte(OV5640_CMXSIGN_HIGH,
                            OV5640_SATURATION_REG_VAL[level][9]);
    ov5640_write_reg16_byte(OV5640_CMXSIGN_LOW,
                            OV5640_SATURATION_REG_VAL[level][10]);

    ov5640_write_reg16_byte(OV5640_SRM_GROUP_ACCESS, 0X13);
    ov5640_write_reg16_byte(OV5640_SRM_GROUP_ACCESS, 0Xa3);
}

void camera_set_exposure_compensation(uint8_t level)
{
    static const uint16_t OV5640_EXPOSURE_COMPENSATION_REG_VAL[][6] = {
        {0x10, 0x08, 0x10, 0x08, 0x20, 0x10}, /* -3 */
        {0x28, 0x18, 0x41, 0x20, 0x18, 0x10}, /* -2 */
        {0x30, 0x28, 0x61, 0x30, 0x28, 0x10}, /* -1 */
        {0x38, 0x30, 0x61, 0x38, 0x30, 0x10}, /* +0 */
        {0x40, 0x38, 0x71, 0x40, 0x38, 0x10}, /* +1 */
        {0x50, 0x48, 0x90, 0x50, 0x48, 0x20}, /* +2 */
        {0x60, 0x58, 0xa0, 0x60, 0x58, 0x10}, /* +3 */
    };
    ov5640_write_reg16_byte(OV5640_AEC_CTRL0F,
                            OV5640_EXPOSURE_COMPENSATION_REG_VAL[level][0]);
    ov5640_write_reg16_byte(OV5640_AEC_CTRL10,
                            OV5640_EXPOSURE_COMPENSATION_REG_VAL[level][1]);
    ov5640_write_reg16_byte(OV5640_AEC_CTRL11,
                            OV5640_EXPOSURE_COMPENSATION_REG_VAL[level][2]);
    ov5640_write_reg16_byte(OV5640_AEC_CTRL1B,
                            OV5640_EXPOSURE_COMPENSATION_REG_VAL[level][3]);
    ov5640_write_reg16_byte(OV5640_AEC_CTRL1E,
                            OV5640_EXPOSURE_COMPENSATION_REG_VAL[level][4]);
    ov5640_write_reg16_byte(OV5640_AEC_CTRL1F,
                            OV5640_EXPOSURE_COMPENSATION_REG_VAL[level][5]);
}

void camera_set_lightmode(uint8_t mode)
{
    static const uint16_t OV5640_LIGHT_MODE_REG_VAL[][7] = {
        {0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00}, /* Auto */
        {0x01, 0x06, 0x1c, 0x04, 0x00, 0x04, 0xf3}, /* Sunny */
        {0x01, 0x05, 0x48, 0x04, 0x00, 0x07, 0xcf}, /* Office */
        {0x01, 0x06, 0x48, 0x04, 0x00, 0x04, 0xd3}, /* Cloudy */
        {0x01, 0x04, 0x10, 0x04, 0x00, 0x08, 0x40}, /* Home */
    };
    ov5640_write_reg16_byte(OV5640_SRM_GROUP_ACCESS, 0X03);

    ov5640_write_reg16_byte(OV5640_AWB_MANUAL_CONTROL,
                            OV5640_LIGHT_MODE_REG_VAL[mode][0]);
    ov5640_write_reg16_byte(OV5640_AWB_R_GAIN_MSB,
                            OV5640_LIGHT_MODE_REG_VAL[mode][1]);
    ov5640_write_reg16_byte(OV5640_AWB_R_GAIN_LSB,
                            OV5640_LIGHT_MODE_REG_VAL[mode][2]);
    ov5640_write_reg16_byte(OV5640_AWB_G_GAIN_MSB,
                            OV5640_LIGHT_MODE_REG_VAL[mode][3]);
    ov5640_write_reg16_byte(OV5640_AWB_G_GAIN_LSB,
                            OV5640_LIGHT_MODE_REG_VAL[mode][4]);
    ov5640_write_reg16_byte(OV5640_AWB_B_GAIN_MSB,
                            OV5640_LIGHT_MODE_REG_VAL[mode][5]);
    ov5640_write_reg16_byte(OV5640_AWB_B_GAIN_LSB,
                            OV5640_LIGHT_MODE_REG_VAL[mode][6]);

    ov5640_write_reg16_byte(OV5640_SRM_GROUP_ACCESS, 0X13);
    ov5640_write_reg16_byte(OV5640_SRM_GROUP_ACCESS, 0Xa3);
}

void camera_set_nightmode(uint8_t mode)
{
    uint8_t tmp3a00 = ov5640_read_reg16_byte(OV5640_AEC_CTRL00);
    if (mode == NIGHT_MODE_ENABLE) {
        tmp3a00 |= 0x04; // Enable night mode
    } else {
        tmp3a00 &= ~0x04; // Disable night mode
    }
    ov5640_write_reg16_byte(OV5640_AEC_CTRL00, tmp3a00);
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
    extern void MX_DMA_Init(void);
    MX_DMA_Init();

    extern void MX_DCMI_Init(void);
    MX_DCMI_Init();

    // __HAL_DCMI_ENABLE_IT(&hdcmi, DCMI_IT_FRAME);
#define OV5640_PWDN_PORT GPIOF
#define OV5640_PWDN_PIN  GPIO_PIN_13
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    gpio_clk_init(OV5640_PWDN_PORT);
    GPIO_InitStruct.Pin   = OV5640_PWDN_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP; // 开漏输出
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(OV5640_PWDN_PORT, &GPIO_InitStruct);

    GPIO_RESET_PIN(OV5640_PWDN_PORT, OV5640_PWDN_PIN);
    OV5640_DELAY_MS(20);

    ov5640_write_reg16_byte(OV5640_SCCB_SYSTEM_CTRL1, 0x11); // system clock from pad, bit[1]
    ov5640_write_reg16_byte(OV5640_SYSTEM_CTROL0, 0x82);     // software reset, bit[7]
    OV5640_DELAY_MS(10);                                     // 等待10ms

    uint16_t id = camera_read_id();
    if (id != OV5640_ID) {
        printf("Camera ID error! Expected: 0x%04x, Got: 0x%04x\n", OV5640_ID, id);
        return;
    }
    printf("Camera ID: 0x%04x\n", id);

    /* Initialization sequence for OV5640 */
    static const uint16_t OV5640_INIT_SEQ[][2] = {
        // maybe need delay 5ms
        {OV5640_SYSTEM_CTROL0, 0x42},     // software power down, bit[6]
        {OV5640_SCCB_SYSTEM_CTRL1, 0x03}, // system clock from PLL, bit[1]
        {OV5640_PAD_OUTPUT_ENABLE01,
         0xff},                             // FREX, Vsync, HREF, PCLK, D[9:6] output enable
        {OV5640_PAD_OUTPUT_ENABLE02, 0xff}, // D[5:0], GPIO[1:0] output enable
        {OV5640_SC_PLL_CONTRL0, 0x1a},      // MIPI 10-bit
        {OV5640_SC_PLL_CONTRL3,
         0x13}, // PLL root divider, bit[4], PLL pre-divider, bit[3:0]
        {OV5640_SYSTEM_ROOT_DIVIDER,
         0x01}, // PCLK root divider, bit[5:4], SCLK2x root divider, bit[3:2]

        // SCLK root divider, bit[1:0]
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
        {0x3600, 0x08}, // VCM control
        {0x3601, 0x33}, // VCM control
        {0x302d, 0x60}, // system control
        {0x3620, 0x52},
        {0x371b, 0x20},
        {0x471c, 0x50},
        {OV5640_AEC_CTRL13, 0x43},            // pre-gain = 1.047x
        {OV5640_AEC_GAIN_CEILING_HIGH, 0x00}, // gain ceiling
        {OV5640_AEC_GAIN_CEILING_LOW, 0xf8},  // gain ceiling = 15.5x
        {0x3635, 0x13},
        {0x3636, 0x03},
        {0x3634, 0x40},
        {0x3622, 0x01},
        // 50/60Hz detection 50/60Hz 灯光条纹过滤
        {OV5640_5060HZ_CTRL01, 0x34},       // Band auto, bit[7]
        {OV5640_5060HZ_CTRL04, 0x28},       // threshold low sum
        {OV5640_5060HZ_CTRL05, 0x98},       // threshold high sum
        {OV5640_LIGHTMETER1_TH_HIGH, 0x00}, // light meter 1 threshold[15:8]
        {OV5640_LIGHTMETER1_TH_LOW, 0x08},  // light meter 1 threshold[7:0]
        {OV5640_LIGHTMETER2_TH_HIGH, 0x00}, // light meter 2 threshold[15:8]
        {OV5640_LIGHTMETER2_TH_LOW, 0x1c},  // light meter 2 threshold[7:0]
        {OV5640_SAMPLE_NUMBER_HIGH, 0x9c},  // sample number[15:8]
        {OV5640_SAMPLE_NUMBER_LOW, 0x40},   // sample number[7:0]
        {OV5640_TIMING_HOFFSET_HIGH, 0x00}, // Timing Hoffset[11:8]
        {OV5640_TIMING_HOFFSET_LOW, 0x10},  // Timing Hoffset[7:0]
        {OV5640_TIMING_VOFFSET_HIGH, 0x00}, // Timing Voffset[10:8]
        {0x3708, 0x64},
        {OV5640_BLC_CTRL01, 0x02},     // BLC start from line 2
        {OV5640_BLC_CTRL05, 0x1a},     // BLC always update
        {OV5640_SYSREM_RESET00, 0x00}, // enable blocks
        {OV5640_CLOCK_ENABLE00, 0xff}, // enable clocks
        {OV5640_MIPI_CONTROL00, 0x58}, // MIPI power down, DVP enable
        {0x302e, 0x00},
        {OV5640_FORMAT_CTRL00, 0x30},   // YUV 422, YUYV
        {OV5640_FORMAT_MUX_CTRL, 0x00}, // YUV 422
        {0x440e, 0x00},
        {OV5640_ISP_CONTROL00,
         0xa7}, // Lenc on, raw gamma on, BPC on, WPC on, CIP on
        // AEC target 自动曝光控制
        {OV5640_AEC_CTRL0F, 0x30}, // stable range in high
        {OV5640_AEC_CTRL10, 0x28}, // stable range in low
        {OV5640_AEC_CTRL1B, 0x30}, // stable range out high
        {OV5640_AEC_CTRL1E, 0x26}, // stable range out low
        {OV5640_AEC_CTRL11, 0x60}, // fast zone high
        {OV5640_AEC_CTRL1F, 0x14}, // fast zone low
        // Lens correction for ? 镜头补偿
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
        {OV5640_LENC_BR_OFFSET, 0xce}, // lenc BR offset
        // AWB 自动白平衡
        {OV5640_AWB_CTRL00, 0xff}, // AWB B block
        {OV5640_AWB_CTRL01, 0xf2}, // AWB control
        {OV5640_AWB_CTRL02,
         0x00},                    // [7:4] max local counter, [3:0] max fast counter
        {OV5640_AWB_CTRL03, 0x14}, // AWB advanced
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
        {OV5640_AWB_CTRL17, 0xf8}, // AWB top limit
        {OV5640_AWB_CTRL18, 0x04}, // AWB bottom limit
        {OV5640_AWB_CTRL19, 0x70}, // red limit
        {OV5640_AWB_CTRL20, 0xf0}, // green limit
        {OV5640_AWB_CTRL21, 0xf0}, // blue limit
        {OV5640_AWB_CTRL22, 0x03}, // AWB control
        {OV5640_AWB_CTRL23, 0x01}, // local limit
        {OV5640_AWB_CTRL24, 0x04},
        {OV5640_AWB_CTRL25, 0x12},
        {OV5640_AWB_CTRL26, 0x04},
        {OV5640_AWB_CTRL27, 0x00},
        {OV5640_AWB_CTRL28, 0x06},
        {OV5640_AWB_CTRL29, 0x82},
        {OV5640_AWB_CTRL30, 0x38}, // AWB control
        // Gamma 伽玛曲线
        {OV5640_GAMMA_CTRL00, 0x01}, // Gamma bias plus on, bit[0]
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
        // color matrix 色彩矩阵
        {OV5640_CMX1, 0x1e},         // CMX1 for Y
        {OV5640_CMX2, 0x5b},         // CMX2 for Y
        {OV5640_CMX3, 0x08},         // CMX3 for Y
        {OV5640_CMX4, 0x0a},         // CMX4 for U
        {OV5640_CMX5, 0x7e},         // CMX5 for U
        {OV5640_CMX6, 0x88},         // CMX6 for U
        {OV5640_CMX7, 0x7c},         // CMX7 for V
        {OV5640_CMX8, 0x6c},         // CMX8 for V
        {OV5640_CMX9, 0x10},         // CMX9 for V
        {OV5640_CMXSIGN_HIGH, 0x01}, // sign[9]
        {OV5640_CMXSIGN_LOW, 0x98},  // sign[8:1]
        // UV adjust UV 色彩饱和度调整
        {OV5640_SDE_CTRL0, 0x06}, // saturation on, bit[1]
        {OV5640_SDE_CTRL3, 0x40},
        {OV5640_SDE_CTRL4, 0x10},
        {OV5640_SDE_CTRL9, 0x10},
        {OV5640_SDE_CTRL10, 0x00},
        {OV5640_SDE_CTRL11, 0xf8},
        {OV5640_ISP_MISC0, 0x40}, // enable manual offset of contrast
        // CIP 锐化和降噪
        {OV5640_CIP_SHARPENMT_TH1, 0x08},     // CIP sharpen MT threshold 1
        {OV5640_CIP_SHARPENMT_TH2, 0x30},     // CIP sharpen MT threshold 2
        {OV5640_CIP_SHARPENMT_OFFSET1, 0x10}, // CIP sharpen MT offset 1
        {OV5640_CIP_SHARPENMT_OFFSET2, 0x00}, // CIP sharpen MT offset 2
        {OV5640_CIP_DNS_TH1, 0x08},           // CIP DNS threshold 1
        {OV5640_CIP_DNS_TH2, 0x30},           // CIP DNS threshold 2
        {OV5640_CIP_DNS_OFFSET1, 0x08},       // CIP DNS offset 1
        {OV5640_CIP_DNS_OFFSET2, 0x16},
        {OV5640_CIP_SHARPENTH_TH1, 0x08},     // CIP sharpen TH threshold 1
        {OV5640_CIP_SHARPENTH_TH2, 0x30},     // CIP sharpen TH threshold 2
        {OV5640_CIP_SHARPENTH_OFFSET1, 0x04}, // CIP sharpen TH offset 1
        {OV5640_CIP_SHARPENTH_OFFSET2, 0x06}, // CIP sharpen TH offset 2
        {0x5025, 0x00},
        {OV5640_SYSTEM_CTROL0, 0x02}, // wake up from standby, bit[6]
    };

    for (uint32_t i = 0; i < sizeof(OV5640_INIT_Config) / sizeof(OV5640_INIT_Config[0]);
         i++) {
        ov5640_write_reg16_byte(OV5640_INIT_Config[i][0], OV5640_INIT_Config[i][1]);
        OV5640_DELAY_MS(1);
    }

    for (uint32_t i = 0; i < sizeof(OV5640_INIT_SEQ) / sizeof(OV5640_INIT_SEQ[0]);
         i++) {
        ov5640_write_reg16_byte(OV5640_INIT_SEQ[i][0], OV5640_INIT_SEQ[i][1]);
    }
    camera_enable_mode(OV5640_DVP_MODE);
    camera_set_resolution(OV5640_R640x480);
    camera_set_format(OV5640_RGB565);
    camera_set_polarity(OV5640_POLARITY_PCLK_HIGH, OV5640_POLARITY_HREF_LOW,
                        OV5640_POLARITY_VSYNC_LOW); // PCLK, HREF, VSYNC polarity
    camera_crop(240, 320);
}

void camera_pattern_test(void)
{
    ov5640_write_reg16_byte(OV5640_PRE_ISP_TEST_SETTING1, 0x80);
    ov5640_write_reg16_byte(OV5640_TEST_PATTERN, 0x00); // test pattern
}
