#ifndef CAMERA_H
#define CAMERA_H

#include "i2c_soft.h"
#include "board.h"
#include <stdint.h>

/** @defgroup OV5640_Exported_Constants OV5640 Exported Constants
 * @{
 */

/**
 * @brief  OV5640 ID
 */
#define OV5640_ID                           0x5640U
/**
 * @brief  OV5640 Registers
 */
/* system and IO pad control [0x3000 ~ 0x3052]        */
#define OV5640_SYSREM_RESET00               0x3000U
#define OV5640_SYSREM_RESET01               0x3001U
#define OV5640_SYSREM_RESET02               0x3002U
#define OV5640_SYSREM_RESET03               0x3003U
#define OV5640_CLOCK_ENABLE00               0x3004U
#define OV5640_CLOCK_ENABLE01               0x3005U
#define OV5640_CLOCK_ENABLE02               0x3006U
#define OV5640_CLOCK_ENABLE03               0x3007U
#define OV5640_SYSTEM_CTROL0                0x3008U
#define OV5640_CHIP_ID_HIGH_BYTE            0x300AU
#define OV5640_CHIP_ID_LOW_BYTE             0x300BU
#define OV5640_MIPI_CONTROL00               0x300EU
#define OV5640_PAD_OUTPUT_ENABLE00          0x3016U
#define OV5640_PAD_OUTPUT_ENABLE01          0x3017U
#define OV5640_PAD_OUTPUT_ENABLE02          0x3018U
#define OV5640_PAD_OUTPUT_VALUE00           0x3019U
#define OV5640_PAD_OUTPUT_VALUE01           0x301AU
#define OV5640_PAD_OUTPUT_VALUE02           0x301BU
#define OV5640_PAD_SELECT00                 0x301CU
#define OV5640_PAD_SELECT01                 0x301DU
#define OV5640_PAD_SELECT02                 0x301EU
#define OV5640_CHIP_REVISION                0x302AU
#define OV5640_PAD_CONTROL00                0x301CU
#define OV5640_SC_PWC                       0x3031U
#define OV5640_SC_PLL_CONTRL0               0x3034U
#define OV5640_SC_PLL_CONTRL1               0x3035U
#define OV5640_SC_PLL_CONTRL2               0x3036U
#define OV5640_SC_PLL_CONTRL3               0x3037U
#define OV5640_SC_PLL_CONTRL4               0x3038U
#define OV5640_SC_PLL_CONTRL5               0x3039U
#define OV5640_SC_PLLS_CTRL0                0x303AU
#define OV5640_SC_PLLS_CTRL1                0x303BU
#define OV5640_SC_PLLS_CTRL2                0x303CU
#define OV5640_SC_PLLS_CTRL3                0x303DU
#define OV5640_IO_PAD_VALUE00               0x3050U
#define OV5640_IO_PAD_VALUE01               0x3051U
#define OV5640_IO_PAD_VALUE02               0x3052U

/* SCCB control [0x3100 ~ 0x3108]                       */
#define OV5640_SCCB_ID                      0x3100U
#define OV5640_SCCB_SYSTEM_CTRL0            0x3102U
#define OV5640_SCCB_SYSTEM_CTRL1            0x3103U
#define OV5640_SYSTEM_ROOT_DIVIDER          0x3108U

/* SRB control [0x3200 ~ 0x3213]                        */
#define OV5640_GROUP_ADDR0                  0x3200U
#define OV5640_GROUP_ADDR1                  0x3201U
#define OV5640_GROUP_ADDR2                  0x3202U
#define OV5640_GROUP_ADDR3                  0x3203U
#define OV5640_SRM_GROUP_ACCESS             0x3212U
#define OV5640_SRM_GROUP_STATUS             0x3213U

/* AWB gain control [0x3400 ~ 0x3406]                   */
#define OV5640_AWB_R_GAIN_MSB               0x3400U
#define OV5640_AWB_R_GAIN_LSB               0x3401U
#define OV5640_AWB_G_GAIN_MSB               0x3402U
#define OV5640_AWB_G_GAIN_LSB               0x3403U
#define OV5640_AWB_B_GAIN_MSB               0x3404U
#define OV5640_AWB_B_GAIN_LSB               0x3405U
#define OV5640_AWB_MANUAL_CONTROL           0x3406U

/* AEC/AGC control [0x3500 ~ 0x350D]                    */
#define OV5640_AEC_PK_EXPOSURE_19_16        0x3500U
#define OV5640_AEC_PK_EXPOSURE_HIGH         0x3501U
#define OV5640_AEC_PK_EXPOSURE_LOW          0x3502U
#define OV5640_AEC_PK_MANUAL                0x3503U
#define OV5640_AEC_PK_REAL_GAIN_9_8         0x350AU
#define OV5640_AEC_PK_REAL_GAIN_LOW         0x350BU
#define OV5640_AEC_PK_VTS_HIGH              0x350CU
#define OV5640_AEC_PK_VTS_LOW               0x350DU

/* VCM control [0x3600 ~ 0x3606]                        */
#define OV5640_VCM_CONTROL_0                0x3602U
#define OV5640_VCM_CONTROL_1                0x3603U
#define OV5640_VCM_CONTROL_2                0x3604U
#define OV5640_VCM_CONTROL_3                0x3605U
#define OV5640_VCM_CONTROL_4                0x3606U

/* timing control [0x3800 ~ 0x3821]                    */
#define OV5640_TIMING_HS_HIGH               0x3800U
#define OV5640_TIMING_HS_LOW                0x3801U
#define OV5640_TIMING_VS_HIGH               0x3802U
#define OV5640_TIMING_VS_LOW                0x3803U
#define OV5640_TIMING_HW_HIGH               0x3804U
#define OV5640_TIMING_HW_LOW                0x3805U
#define OV5640_TIMING_VH_HIGH               0x3806U
#define OV5640_TIMING_VH_LOW                0x3807U
#define OV5640_TIMING_DVPHO_HIGH            0x3808U
#define OV5640_TIMING_DVPHO_LOW             0x3809U
#define OV5640_TIMING_DVPVO_HIGH            0x380AU
#define OV5640_TIMING_DVPVO_LOW             0x380BU
#define OV5640_TIMING_HTS_HIGH              0x380CU
#define OV5640_TIMING_HTS_LOW               0x380DU
#define OV5640_TIMING_VTS_HIGH              0x380EU
#define OV5640_TIMING_VTS_LOW               0x380FU
#define OV5640_TIMING_HOFFSET_HIGH          0x3810U
#define OV5640_TIMING_HOFFSET_LOW           0x3811U
#define OV5640_TIMING_VOFFSET_HIGH          0x3812U
#define OV5640_TIMING_VOFFSET_LOW           0x3813U
#define OV5640_TIMING_X_INC                 0x3814U
#define OV5640_TIMING_Y_INC                 0x3815U
#define OV5640_HSYNC_START_HIGH             0x3816U
#define OV5640_HSYNC_START_LOW              0x3817U
#define OV5640_HSYNC_WIDTH_HIGH             0x3818U
#define OV5640_HSYNC_WIDTH_LOW              0x3819U
#define OV5640_TIMING_TC_REG20              0x3820U
#define OV5640_TIMING_TC_REG21              0x3821U

/* AEC/AGC power down domain control [0x3A00 ~ 0x3A25] */
#define OV5640_AEC_CTRL00                   0x3A00U
#define OV5640_AEC_CTRL01                   0x3A01U
#define OV5640_AEC_CTRL02                   0x3A02U
#define OV5640_AEC_CTRL03                   0x3A03U
#define OV5640_AEC_CTRL04                   0x3A04U
#define OV5640_AEC_CTRL05                   0x3A05U
#define OV5640_AEC_CTRL06                   0x3A06U
#define OV5640_AEC_CTRL07                   0x3A07U
#define OV5640_AEC_B50_STEP_HIGH            0x3A08U
#define OV5640_AEC_B50_STEP_LOW             0x3A09U
#define OV5640_AEC_B60_STEP_HIGH            0x3A0AU
#define OV5640_AEC_B60_STEP_LOW             0x3A0BU
#define OV5640_AEC_AEC_CTRL0C               0x3A0CU
#define OV5640_AEC_CTRL0D                   0x3A0DU
#define OV5640_AEC_CTRL0E                   0x3A0EU
#define OV5640_AEC_CTRL0F                   0x3A0FU
#define OV5640_AEC_CTRL10                   0x3A10U
#define OV5640_AEC_CTRL11                   0x3A11U
#define OV5640_AEC_CTRL13                   0x3A13U
#define OV5640_AEC_MAX_EXPO_HIGH            0x3A14U
#define OV5640_AEC_MAX_EXPO_LOW             0x3A15U
#define OV5640_AEC_CTRL17                   0x3A17U
#define OV5640_AEC_GAIN_CEILING_HIGH        0x3A18U
#define OV5640_AEC_GAIN_CEILING_LOW         0x3A19U
#define OV5640_AEC_DIFF_MIN                 0x3A1AU
#define OV5640_AEC_CTRL1B                   0x3A1BU
#define OV5640_LED_ADD_ROW_HIGH             0x3A1CU
#define OV5640_LED_ADD_ROW_LOW              0x3A1DU
#define OV5640_AEC_CTRL1E                   0x3A1EU
#define OV5640_AEC_CTRL1F                   0x3A1FU
#define OV5640_AEC_CTRL20                   0x3A20U
#define OV5640_AEC_CTRL21                   0x3A21U
#define OV5640_AEC_CTRL25                   0x3A25U

/* strobe control [0x3B00 ~ 0x3B0C]                      */
#define OV5640_STROBE_CTRL                  0x3B00U
#define OV5640_FREX_EXPOSURE02              0x3B01U
#define OV5640_FREX_SHUTTER_DLY01           0x3B02U
#define OV5640_FREX_SHUTTER_DLY00           0x3B03U
#define OV5640_FREX_EXPOSURE01              0x3B04U
#define OV5640_FREX_EXPOSURE00              0x3B05U
#define OV5640_FREX_CTRL07                  0x3B06U
#define OV5640_FREX_MODE                    0x3B07U
#define OV5640_FREX_RQST                    0x3B08U
#define OV5640_FREX_HREF_DLY                0x3B09U
#define OV5640_FREX_RST_LENGTH              0x3B0AU
#define OV5640_STROBE_WIDTH_HIGH            0x3B0BU
#define OV5640_STROBE_WIDTH_LOW             0x3B0CU

/* 50/60Hz detector control [0x3C00 ~ 0x3C1E]            */
#define OV5640_5060HZ_CTRL00                0x3C00U
#define OV5640_5060HZ_CTRL01                0x3C01U
#define OV5640_5060HZ_CTRL02                0x3C02U
#define OV5640_5060HZ_CTRL03                0x3C03U
#define OV5640_5060HZ_CTRL04                0x3C04U
#define OV5640_5060HZ_CTRL05                0x3C05U
#define OV5640_LIGHTMETER1_TH_HIGH          0x3C06U
#define OV5640_LIGHTMETER1_TH_LOW           0x3C07U
#define OV5640_LIGHTMETER2_TH_HIGH          0x3C08U
#define OV5640_LIGHTMETER2_TH_LOW           0x3C09U
#define OV5640_SAMPLE_NUMBER_HIGH           0x3C0AU
#define OV5640_SAMPLE_NUMBER_LOW            0x3C0BU
#define OV5640_SIGMA_DELTA_CTRL0C           0x3C0CU
#define OV5640_SUM50_BYTE4                  0x3C0DU
#define OV5640_SUM50_BYTE3                  0x3C0EU
#define OV5640_SUM50_BYTE2                  0x3C0FU
#define OV5640_SUM50_BYTE1                  0x3C10U
#define OV5640_SUM60_BYTE4                  0x3C11U
#define OV5640_SUM60_BYTE3                  0x3C12U
#define OV5640_SUM60_BYTE2                  0x3C13U
#define OV5640_SUM60_BYTE1                  0x3C14U
#define OV5640_SUM5060_HIGH                 0x3C15U
#define OV5640_SUM5060_LOW                  0x3C16U
#define OV5640_BLOCK_CNTR_HIGH              0x3C17U
#define OV5640_BLOCK_CNTR_LOW               0x3C18U
#define OV5640_B6_HIGH                      0x3C19U
#define OV5640_B6_LOW                       0x3C1AU
#define OV5640_LIGHTMETER_OUTPUT_BYTE3      0x3C1BU
#define OV5640_LIGHTMETER_OUTPUT_BYTE2      0x3C1CU
#define OV5640_LIGHTMETER_OUTPUT_BYTE1      0x3C1DU
#define OV5640_SUM_THRESHOLD                0x3C1EU

/* OTP control [0x3D00 ~ 0x3D21]                         */
/* MC control [0x3F00 ~ 0x3F0D]                          */
/* BLC control [0x4000 ~ 0x4033]                         */
#define OV5640_BLC_CTRL00                   0x4000U
#define OV5640_BLC_CTRL01                   0x4001U
#define OV5640_BLC_CTRL02                   0x4002U
#define OV5640_BLC_CTRL03                   0x4003U
#define OV5640_BLC_CTRL04                   0x4004U
#define OV5640_BLC_CTRL05                   0x4005U

/* frame control [0x4201 ~ 0x4202]                       */
#define OV5640_FRAME_CTRL01                 0x4201U
#define OV5640_FRAME_CTRL02                 0x4202U

/* format control [0x4300 ~ 0x430D]                      */
#define OV5640_FORMAT_CTRL00                0x4300U
#define OV5640_FORMAT_CTRL01                0x4301U
#define OV5640_YMAX_VAL_HIGH                0x4302U
#define OV5640_YMAX_VAL_LOW                 0x4303U
#define OV5640_YMIN_VAL_HIGH                0x4304U
#define OV5640_YMIN_VAL_LOW                 0x4305U
#define OV5640_UMAX_VAL_HIGH                0x4306U
#define OV5640_UMAX_VAL_LOW                 0x4307U
#define OV5640_UMIN_VAL_HIGH                0x4308U
#define OV5640_UMIN_VAL_LOW                 0x4309U
#define OV5640_VMAX_VAL_HIGH                0x430AU
#define OV5640_VMAX_VAL_LOW                 0x430BU
#define OV5640_VMIN_VAL_HIGH                0x430CU
#define OV5640_VMIN_VAL_LOW                 0x430DU

/* JPEG control [0x4400 ~ 0x4431]                        */
#define OV5640_JPEG_CTRL00                  0x4400U
#define OV5640_JPEG_CTRL01                  0x4401U
#define OV5640_JPEG_CTRL02                  0x4402U
#define OV5640_JPEG_CTRL03                  0x4403U
#define OV5640_JPEG_CTRL04                  0x4404U
#define OV5640_JPEG_CTRL05                  0x4405U
#define OV5640_JPEG_CTRL06                  0x4406U
#define OV5640_JPEG_CTRL07                  0x4407U
#define OV5640_JPEG_ISI_CTRL1               0x4408U
#define OV5640_JPEG_CTRL09                  0x4409U
#define OV5640_JPEG_CTRL0A                  0x440AU
#define OV5640_JPEG_CTRL0B                  0x440BU
#define OV5640_JPEG_CTRL0C                  0x440CU
#define OV5640_JPEG_QT_DATA                 0x4410U
#define OV5640_JPEG_QT_ADDR                 0x4411U
#define OV5640_JPEG_ISI_DATA                0x4412U
#define OV5640_JPEG_ISI_CTRL2               0x4413U
#define OV5640_JPEG_LENGTH_BYTE3            0x4414U
#define OV5640_JPEG_LENGTH_BYTE2            0x4415U
#define OV5640_JPEG_LENGTH_BYTE1            0x4416U
#define OV5640_JFIFO_OVERFLOW               0x4417U

/* VFIFO control [0x4600 ~ 0x460D]                       */
#define OV5640_VFIFO_CTRL00                 0x4600U
#define OV5640_VFIFO_HSIZE_HIGH             0x4602U
#define OV5640_VFIFO_HSIZE_LOW              0x4603U
#define OV5640_VFIFO_VSIZE_HIGH             0x4604U
#define OV5640_VFIFO_VSIZE_LOW              0x4605U
#define OV5640_VFIFO_CTRL0C                 0x460CU
#define OV5640_VFIFO_CTRL0D                 0x460DU

/* DVP control [0x4709 ~ 0x4745]                         */
#define OV5640_DVP_VSYNC_WIDTH0             0x4709U
#define OV5640_DVP_VSYNC_WIDTH1             0x470AU
#define OV5640_DVP_VSYNC_WIDTH2             0x470BU
#define OV5640_PAD_LEFT_CTRL                0x4711U
#define OV5640_PAD_RIGHT_CTRL               0x4712U
#define OV5640_JPG_MODE_SELECT              0x4713U
#define OV5640_656_DUMMY_LINE               0x4715U
#define OV5640_CCIR656_CTRL                 0x4719U
#define OV5640_HSYNC_CTRL00                 0x471BU
#define OV5640_DVP_VSYN_CTRL                0x471DU
#define OV5640_DVP_HREF_CTRL                0x471FU
#define OV5640_VSTART_OFFSET                0x4721U
#define OV5640_VEND_OFFSET                  0x4722U
#define OV5640_DVP_CTRL23                   0x4723U
#define OV5640_CCIR656_CTRL00               0x4730U
#define OV5640_CCIR656_CTRL01               0x4731U
#define OV5640_CCIR656_FS                   0x4732U
#define OV5640_CCIR656_FE                   0x4733U
#define OV5640_CCIR656_LS                   0x4734U
#define OV5640_CCIR656_LE                   0x4735U
#define OV5640_CCIR656_CTRL06               0x4736U
#define OV5640_CCIR656_CTRL07               0x4737U
#define OV5640_CCIR656_CTRL08               0x4738U
#define OV5640_POLARITY_CTRL                0x4740U
#define OV5640_TEST_PATTERN                 0x4741U
#define OV5640_DATA_ORDER                   0x4745U

/* MIPI control [0x4800 ~ 0x4837]                        */
#define OV5640_MIPI_CTRL00                  0x4800U
#define OV5640_MIPI_CTRL01                  0x4801U
#define OV5640_MIPI_CTRL05                  0x4805U
#define OV5640_MIPI_DATA_ORDER              0x480AU
#define OV5640_MIN_HS_ZERO_HIGH             0x4818U
#define OV5640_MIN_HS_ZERO_LOW              0x4819U
#define OV5640_MIN_MIPI_HS_TRAIL_HIGH       0x481AU
#define OV5640_MIN_MIPI_HS_TRAIL_LOW        0x481BU
#define OV5640_MIN_MIPI_CLK_ZERO_HIGH       0x481CU
#define OV5640_MIN_MIPI_CLK_ZERO_LOW        0x481DU
#define OV5640_MIN_MIPI_CLK_PREPARE_HIGH    0x481EU
#define OV5640_MIN_MIPI_CLK_PREPARE_LOW     0x481FU
#define OV5640_MIN_CLK_POST_HIGH            0x4820U
#define OV5640_MIN_CLK_POST_LOW             0x4821U
#define OV5640_MIN_CLK_TRAIL_HIGH           0x4822U
#define OV5640_MIN_CLK_TRAIL_LOW            0x4823U
#define OV5640_MIN_LPX_PCLK_HIGH            0x4824U
#define OV5640_MIN_LPX_PCLK_LOW             0x4825U
#define OV5640_MIN_HS_PREPARE_HIGH          0x4826U
#define OV5640_MIN_HS_PREPARE_LOW           0x4827U
#define OV5640_MIN_HS_EXIT_HIGH             0x4828U
#define OV5640_MIN_HS_EXIT_LOW              0x4829U
#define OV5640_MIN_HS_ZERO_UI               0x482AU
#define OV5640_MIN_HS_TRAIL_UI              0x482BU
#define OV5640_MIN_CLK_ZERO_UI              0x482CU
#define OV5640_MIN_CLK_PREPARE_UI           0x482DU
#define OV5640_MIN_CLK_POST_UI              0x482EU
#define OV5640_MIN_CLK_TRAIL_UI             0x482FU
#define OV5640_MIN_LPX_PCLK_UI              0x4830U
#define OV5640_MIN_HS_PREPARE_UI            0x4831U
#define OV5640_MIN_HS_EXIT_UI               0x4832U
#define OV5640_PCLK_PERIOD                  0x4837U

/* ISP frame control [0x4901 ~ 0x4902]                   */
#define OV5640_ISP_FRAME_CTRL01             0x4901U
#define OV5640_ISP_FRAME_CTRL02             0x4902U

/* ISP top control [0x5000 ~ 0x5063]                     */
#define OV5640_ISP_CONTROL00                0x5000U
#define OV5640_ISP_CONTROL01                0x5001U
#define OV5640_ISP_CONTROL03                0x5003U
#define OV5640_ISP_CONTROL05                0x5005U
#define OV5640_ISP_MISC0                    0x501DU
#define OV5640_ISP_MISC1                    0x501EU
#define OV5640_FORMAT_MUX_CTRL              0x501FU
#define OV5640_DITHER_CTRL0                 0x5020U
#define OV5640_DRAW_WINDOW_CTRL00           0x5027U
#define OV5640_DRAW_WINDOW_LEFT_CTRL_HIGH   0x5028U
#define OV5640_DRAW_WINDOW_LEFT_CTRL_LOW    0x5029U
#define OV5640_DRAW_WINDOW_RIGHT_CTRL_HIGH  0x502AU
#define OV5640_DRAW_WINDOW_RIGHT_CTRL_LOW   0x502BU
#define OV5640_DRAW_WINDOW_TOP_CTRL_HIGH    0x502CU
#define OV5640_DRAW_WINDOW_TOP_CTRL_LOW     0x502DU
#define OV5640_DRAW_WINDOW_BOTTOM_CTRL_HIGH 0x502EU
#define OV5640_DRAW_WINDOW_BOTTOM_CTRL_LOW  0x502FU
#define OV5640_DRAW_WINDOW_HBW_CTRL_HIGH    0x5030U /* HBW: Horizontal Boundary Width */
#define OV5640_DRAW_WINDOW_HBW_CTRL_LOW     0x5031U
#define OV5640_DRAW_WINDOW_VBW_CTRL_HIGH    0x5032U /* VBW: Vertical Boundary Width */
#define OV5640_DRAW_WINDOW_VBW_CTRL_LOW     0x5033U
#define OV5640_DRAW_WINDOW_Y_CTRL           0x5034U
#define OV5640_DRAW_WINDOW_U_CTRL           0x5035U
#define OV5640_DRAW_WINDOW_V_CTRL           0x5036U
#define OV5640_PRE_ISP_TEST_SETTING1        0x503DU
#define OV5640_ISP_SENSOR_BIAS_I            0x5061U
#define OV5640_ISP_SENSOR_GAIN1_I           0x5062U
#define OV5640_ISP_SENSOR_GAIN2_I           0x5063U

/* AWB control [0x5180 ~ 0x51D0]                         */
#define OV5640_AWB_CTRL00                   0x5180U
#define OV5640_AWB_CTRL01                   0x5181U
#define OV5640_AWB_CTRL02                   0x5182U
#define OV5640_AWB_CTRL03                   0x5183U
#define OV5640_AWB_CTRL04                   0x5184U
#define OV5640_AWB_CTRL05                   0x5185U
#define OV5640_AWB_CTRL06                   0x5186U /* Advanced AWB control registers: 0x5186 ~ 0x5190 */
#define OV5640_AWB_CTRL07                   0x5187U
#define OV5640_AWB_CTRL08                   0x5188U
#define OV5640_AWB_CTRL09                   0x5189U
#define OV5640_AWB_CTRL10                   0x518AU
#define OV5640_AWB_CTRL11                   0x518BU
#define OV5640_AWB_CTRL12                   0x518CU
#define OV5640_AWB_CTRL13                   0x518DU
#define OV5640_AWB_CTRL14                   0x518EU
#define OV5640_AWB_CTRL15                   0x518FU
#define OV5640_AWB_CTRL16                   0x5190U
#define OV5640_AWB_CTRL17                   0x5191U
#define OV5640_AWB_CTRL18                   0x5192U
#define OV5640_AWB_CTRL19                   0x5193U
#define OV5640_AWB_CTRL20                   0x5194U
#define OV5640_AWB_CTRL21                   0x5195U
#define OV5640_AWB_CTRL22                   0x5196U
#define OV5640_AWB_CTRL23                   0x5197U
#define OV5640_AWB_CTRL24                   0x5198U
#define OV5640_AWB_CTRL25                   0x5199U
#define OV5640_AWB_CTRL26                   0x519AU
#define OV5640_AWB_CTRL27                   0x519BU
#define OV5640_AWB_CTRL28                   0x519CU
#define OV5640_AWB_CTRL29                   0x519DU
#define OV5640_AWB_CTRL30                   0x519EU
#define OV5640_AWB_CURRENT_R_GAIN_HIGH      0x519FU
#define OV5640_AWB_CURRENT_R_GAIN_LOW       0x51A0U
#define OV5640_AWB_CURRENT_G_GAIN_HIGH      0x51A1U
#define OV5640_AWB_CURRENT_G_GAIN_LOW       0x51A2U
#define OV5640_AWB_CURRENT_B_GAIN_HIGH      0x51A3U
#define OV5640_AWB_CURRENT_B_GAIN_LOW       0x51A4U
#define OV5640_AWB_AVERAGE_R                0x51A5U
#define OV5640_AWB_AVERAGE_G                0x51A6U
#define OV5640_AWB_AVERAGE_B                0x51A7U
#define OV5640_AWB_CTRL74                   0x5180U

/* CIP control [0x5300 ~ 0x530F]                         */
#define OV5640_CIP_SHARPENMT_TH1            0x5300U
#define OV5640_CIP_SHARPENMT_TH2            0x5301U
#define OV5640_CIP_SHARPENMT_OFFSET1        0x5302U
#define OV5640_CIP_SHARPENMT_OFFSET2        0x5303U
#define OV5640_CIP_DNS_TH1                  0x5304U
#define OV5640_CIP_DNS_TH2                  0x5305U
#define OV5640_CIP_DNS_OFFSET1              0x5306U
#define OV5640_CIP_DNS_OFFSET2              0x5307U
#define OV5640_CIP_CTRL                     0x5308U
#define OV5640_CIP_SHARPENTH_TH1            0x5309U
#define OV5640_CIP_SHARPENTH_TH2            0x530AU
#define OV5640_CIP_SHARPENTH_OFFSET1        0x530BU
#define OV5640_CIP_SHARPENTH_OFFSET2        0x530CU
#define OV5640_CIP_EDGE_MT_AUTO             0x530DU
#define OV5640_CIP_DNS_TH_AUTO              0x530EU
#define OV5640_CIP_SHARPEN_TH_AUTO          0x530FU

/* CMX control [0x5380 ~ 0x538B]                         */
#define OV5640_CMX_CTRL                     0x5380U
#define OV5640_CMX1                         0x5381U
#define OV5640_CMX2                         0x5382U
#define OV5640_CMX3                         0x5383U
#define OV5640_CMX4                         0x5384U
#define OV5640_CMX5                         0x5385U
#define OV5640_CMX6                         0x5386U
#define OV5640_CMX7                         0x5387U
#define OV5640_CMX8                         0x5388U
#define OV5640_CMX9                         0x5389U
#define OV5640_CMXSIGN_HIGH                 0x538AU
#define OV5640_CMXSIGN_LOW                  0x538BU

/* gamma control [0x5480 ~ 0x5490]                       */
#define OV5640_GAMMA_CTRL00                 0x5480U
#define OV5640_GAMMA_YST00                  0x5481U
#define OV5640_GAMMA_YST01                  0x5482U
#define OV5640_GAMMA_YST02                  0x5483U
#define OV5640_GAMMA_YST03                  0x5484U
#define OV5640_GAMMA_YST04                  0x5485U
#define OV5640_GAMMA_YST05                  0x5486U
#define OV5640_GAMMA_YST06                  0x5487U
#define OV5640_GAMMA_YST07                  0x5488U
#define OV5640_GAMMA_YST08                  0x5489U
#define OV5640_GAMMA_YST09                  0x548AU
#define OV5640_GAMMA_YST0A                  0x548BU
#define OV5640_GAMMA_YST0B                  0x548CU
#define OV5640_GAMMA_YST0C                  0x548DU
#define OV5640_GAMMA_YST0D                  0x548EU
#define OV5640_GAMMA_YST0E                  0x548FU
#define OV5640_GAMMA_YST0F                  0x5490U

/* SDE control [0x5580 ~ 0x558C]                         */
#define OV5640_SDE_CTRL0                    0x5580U
#define OV5640_SDE_CTRL1                    0x5581U
#define OV5640_SDE_CTRL2                    0x5582U
#define OV5640_SDE_CTRL3                    0x5583U
#define OV5640_SDE_CTRL4                    0x5584U
#define OV5640_SDE_CTRL5                    0x5585U
#define OV5640_SDE_CTRL6                    0x5586U
#define OV5640_SDE_CTRL7                    0x5587U
#define OV5640_SDE_CTRL8                    0x5588U
#define OV5640_SDE_CTRL9                    0x5589U
#define OV5640_SDE_CTRL10                   0x558AU
#define OV5640_SDE_CTRL11                   0x558BU
#define OV5640_SDE_CTRL12                   0x558CU

/* scale control [0x5600 ~ 0x5606]                       */
#define OV5640_SCALE_CTRL0                  0x5600U
#define OV5640_SCALE_CTRL1                  0x5601U
#define OV5640_SCALE_CTRL2                  0x5602U
#define OV5640_SCALE_CTRL3                  0x5603U
#define OV5640_SCALE_CTRL4                  0x5604U
#define OV5640_SCALE_CTRL5                  0x5605U
#define OV5640_SCALE_CTRL6                  0x5606U

/* AVG control [0x5680 ~ 0x56A2]                         */
#define OV5640_X_START_HIGH                 0x5680U
#define OV5640_X_START_LOW                  0x5681U
#define OV5640_Y_START_HIGH                 0x5682U
#define OV5640_Y_START_LOW                  0x5683U
#define OV5640_X_WINDOW_HIGH                0x5684U
#define OV5640_X_WINDOW_LOW                 0x5685U
#define OV5640_Y_WINDOW_HIGH                0x5686U
#define OV5640_Y_WINDOW_LOW                 0x5687U
#define OV5640_WEIGHT00                     0x5688U
#define OV5640_WEIGHT01                     0x5689U
#define OV5640_WEIGHT02                     0x568AU
#define OV5640_WEIGHT03                     0x568BU
#define OV5640_WEIGHT04                     0x568CU
#define OV5640_WEIGHT05                     0x568DU
#define OV5640_WEIGHT06                     0x568EU
#define OV5640_WEIGHT07                     0x568FU
#define OV5640_AVG_CTRL10                   0x5690U
#define OV5640_AVG_WIN_00                   0x5691U
#define OV5640_AVG_WIN_01                   0x5692U
#define OV5640_AVG_WIN_02                   0x5693U
#define OV5640_AVG_WIN_03                   0x5694U
#define OV5640_AVG_WIN_10                   0x5695U
#define OV5640_AVG_WIN_11                   0x5696U
#define OV5640_AVG_WIN_12                   0x5697U
#define OV5640_AVG_WIN_13                   0x5698U
#define OV5640_AVG_WIN_20                   0x5699U
#define OV5640_AVG_WIN_21                   0x569AU
#define OV5640_AVG_WIN_22                   0x569BU
#define OV5640_AVG_WIN_23                   0x569CU
#define OV5640_AVG_WIN_30                   0x569DU
#define OV5640_AVG_WIN_31                   0x569EU
#define OV5640_AVG_WIN_32                   0x569FU
#define OV5640_AVG_WIN_33                   0x56A0U
#define OV5640_AVG_READOUT                  0x56A1U
#define OV5640_AVG_WEIGHT_SUM               0x56A2U

/* LENC control [0x5800 ~ 0x5849]                        */
#define OV5640_GMTRX00                      0x5800U
#define OV5640_GMTRX01                      0x5801U
#define OV5640_GMTRX02                      0x5802U
#define OV5640_GMTRX03                      0x5803U
#define OV5640_GMTRX04                      0x5804U
#define OV5640_GMTRX05                      0x5805U
#define OV5640_GMTRX10                      0x5806U
#define OV5640_GMTRX11                      0x5807U
#define OV5640_GMTRX12                      0x5808U
#define OV5640_GMTRX13                      0x5809U
#define OV5640_GMTRX14                      0x580AU
#define OV5640_GMTRX15                      0x580BU
#define OV5640_GMTRX20                      0x580CU
#define OV5640_GMTRX21                      0x580DU
#define OV5640_GMTRX22                      0x580EU
#define OV5640_GMTRX23                      0x580FU
#define OV5640_GMTRX24                      0x5810U
#define OV5640_GMTRX25                      0x5811U
#define OV5640_GMTRX30                      0x5812U
#define OV5640_GMTRX31                      0x5813U
#define OV5640_GMTRX32                      0x5814U
#define OV5640_GMTRX33                      0x5815U
#define OV5640_GMTRX34                      0x5816U
#define OV5640_GMTRX35                      0x5817U
#define OV5640_GMTRX40                      0x5818U
#define OV5640_GMTRX41                      0x5819U
#define OV5640_GMTRX42                      0x581AU
#define OV5640_GMTRX43                      0x581BU
#define OV5640_GMTRX44                      0x581CU
#define OV5640_GMTRX45                      0x581DU
#define OV5640_GMTRX50                      0x581EU
#define OV5640_GMTRX51                      0x581FU
#define OV5640_GMTRX52                      0x5820U
#define OV5640_GMTRX53                      0x5821U
#define OV5640_GMTRX54                      0x5822U
#define OV5640_GMTRX55                      0x5823U
#define OV5640_BRMATRX00                    0x5824U
#define OV5640_BRMATRX01                    0x5825U
#define OV5640_BRMATRX02                    0x5826U
#define OV5640_BRMATRX03                    0x5827U
#define OV5640_BRMATRX04                    0x5828U
#define OV5640_BRMATRX05                    0x5829U
#define OV5640_BRMATRX06                    0x582AU
#define OV5640_BRMATRX07                    0x582BU
#define OV5640_BRMATRX08                    0x582CU
#define OV5640_BRMATRX09                    0x582DU
#define OV5640_BRMATRX20                    0x582EU
#define OV5640_BRMATRX21                    0x582FU
#define OV5640_BRMATRX22                    0x5830U
#define OV5640_BRMATRX23                    0x5831U
#define OV5640_BRMATRX24                    0x5832U
#define OV5640_BRMATRX30                    0x5833U
#define OV5640_BRMATRX31                    0x5834U
#define OV5640_BRMATRX32                    0x5835U
#define OV5640_BRMATRX33                    0x5836U
#define OV5640_BRMATRX34                    0x5837U
#define OV5640_BRMATRX40                    0x5838U
#define OV5640_BRMATRX41                    0x5839U
#define OV5640_BRMATRX42                    0x583AU
#define OV5640_BRMATRX43                    0x583BU
#define OV5640_BRMATRX44                    0x583CU
#define OV5640_LENC_BR_OFFSET               0x583DU
#define OV5640_MAX_GAIN                     0x583EU
#define OV5640_MIN_GAIN                     0x583FU
#define OV5640_MIN_Q                        0x5840U
#define OV5640_LENC_CTRL59                  0x5841U
#define OV5640_BR_HSCALE_HIGH               0x5842U
#define OV5640_BR_HSCALE_LOW                0x5843U
#define OV5640_BR_VSCALE_HIGH               0x5844U
#define OV5640_BR_VSCALE_LOW                0x5845U
#define OV5640_G_HSCALE_HIGH                0x5846U
#define OV5640_G_HSCALE_LOW                 0x5847U
#define OV5640_G_VSCALE_HIGH                0x5848U
#define OV5640_G_VSCALE_LOW                 0x5849U

/* AFC control [0x6000 ~ 0x603F]                         */
#define OV5640_AFC_CTRL00                   0x6000U
#define OV5640_AFC_CTRL01                   0x6001U
#define OV5640_AFC_CTRL02                   0x6002U
#define OV5640_AFC_CTRL03                   0x6003U
#define OV5640_AFC_CTRL04                   0x6004U
#define OV5640_AFC_CTRL05                   0x6005U
#define OV5640_AFC_CTRL06                   0x6006U
#define OV5640_AFC_CTRL07                   0x6007U
#define OV5640_AFC_CTRL08                   0x6008U
#define OV5640_AFC_CTRL09                   0x6009U
#define OV5640_AFC_CTRL10                   0x600AU
#define OV5640_AFC_CTRL11                   0x600BU
#define OV5640_AFC_CTRL12                   0x600CU
#define OV5640_AFC_CTRL13                   0x600DU
#define OV5640_AFC_CTRL14                   0x600EU
#define OV5640_AFC_CTRL15                   0x600FU
#define OV5640_AFC_CTRL16                   0x6010U
#define OV5640_AFC_CTRL17                   0x6011U
#define OV5640_AFC_CTRL18                   0x6012U
#define OV5640_AFC_CTRL19                   0x6013U
#define OV5640_AFC_CTRL20                   0x6014U
#define OV5640_AFC_CTRL21                   0x6015U
#define OV5640_AFC_CTRL22                   0x6016U
#define OV5640_AFC_CTRL23                   0x6017U
#define OV5640_AFC_CTRL24                   0x6018U
#define OV5640_AFC_CTRL25                   0x6019U
#define OV5640_AFC_CTRL26                   0x601AU
#define OV5640_AFC_CTRL27                   0x601BU
#define OV5640_AFC_CTRL28                   0x601CU
#define OV5640_AFC_CTRL29                   0x601DU
#define OV5640_AFC_CTRL30                   0x601EU
#define OV5640_AFC_CTRL31                   0x601FU
#define OV5640_AFC_CTRL32                   0x6020U
#define OV5640_AFC_CTRL33                   0x6021U
#define OV5640_AFC_CTRL34                   0x6022U
#define OV5640_AFC_CTRL35                   0x6023U
#define OV5640_AFC_CTRL36                   0x6024U
#define OV5640_AFC_CTRL37                   0x6025U
#define OV5640_AFC_CTRL38                   0x6026U
#define OV5640_AFC_CTRL39                   0x6027U
#define OV5640_AFC_CTRL40                   0x6028U
#define OV5640_AFC_CTRL41                   0x6029U
#define OV5640_AFC_CTRL42                   0x602AU
#define OV5640_AFC_CTRL43                   0x602BU
#define OV5640_AFC_CTRL44                   0x602CU
#define OV5640_AFC_CTRL45                   0x602DU
#define OV5640_AFC_CTRL46                   0x602EU
#define OV5640_AFC_CTRL47                   0x602FU
#define OV5640_AFC_CTRL48                   0x6030U
#define OV5640_AFC_CTRL49                   0x6031U
#define OV5640_AFC_CTRL50                   0x6032U
#define OV5640_AFC_CTRL51                   0x6033U
#define OV5640_AFC_CTRL52                   0x6034U
#define OV5640_AFC_CTRL53                   0x6035U
#define OV5640_AFC_CTRL54                   0x6036U
#define OV5640_AFC_CTRL55                   0x6037U
#define OV5640_AFC_CTRL56                   0x6038U
#define OV5640_AFC_CTRL57                   0x6039U
#define OV5640_AFC_CTRL58                   0x603AU
#define OV5640_AFC_CTRL59                   0x603BU
#define OV5640_AFC_CTRL60                   0x603CU
#define OV5640_AFC_READ58                   0x603DU
#define OV5640_AFC_READ59                   0x603EU
#define OV5640_AFC_READ60                   0x603FU

/**
 * @}
 */

/** @defgroup OV5640_Exported_Constants
 * @{
 */
#define OV5640_OK                           (0)
#define OV5640_ERROR                        (-1)
/**
 * @brief  OV5640 Features Parameters
 */
/* Camera resolutions */
#define OV5640_R160x120                     0x00U /* QQVGA Resolution           */
#define OV5640_R320x240                     0x01U /* QVGA Resolution            */
#define OV5640_R480x272                     0x02U /* 480x272 Resolution         */
#define OV5640_R640x480                     0x03U /* VGA Resolution             */
#define OV5640_R800x480                     0x04U /* WVGA Resolution            */

/* Camera Pixel Format */
#define OV5640_RGB565                       0x00U /* Pixel Format RGB565        */
#define OV5640_RGB888                       0x01U /* Pixel Format RGB888        */
#define OV5640_YUV422                       0x02U /* Pixel Format YUV422        */
#define OV5640_Y8                           0x08U /* Pixel Format Y8            */
#define OV5640_JPEG                         0x10U /* Compressed format JPEG     */

/* Polarity */
#define OV5640_POLARITY_PCLK_LOW            0x00U /* Signal Active Low          */
#define OV5640_POLARITY_PCLK_HIGH           0x01U /* Signal Active High         */
#define OV5640_POLARITY_HREF_LOW            0x00U /* Signal Active Low          */
#define OV5640_POLARITY_HREF_HIGH           0x01U /* Signal Active High         */
#define OV5640_POLARITY_VSYNC_LOW           0x01U /* Signal Active Low          */
#define OV5640_POLARITY_VSYNC_HIGH          0x00U /* Signal Active High         */

/* Mirror/Flip */
#define OV5640_MIRROR_FLIP_NONE             0x00U /* Set camera normal mode     */
#define OV5640_FLIP                         0x01U /* Set camera flip config     */
#define OV5640_MIRROR                       0x02U /* Set camera mirror config   */
#define OV5640_MIRROR_FLIP                  0x03U /* Set camera mirror and flip */

/* Zoom */
#define OV5640_ZOOM_x8                      0x00U /* Set zoom to x8             */
#define OV5640_ZOOM_x4                      0x11U /* Set zoom to x4             */
#define OV5640_ZOOM_x2                      0x22U /* Set zoom to x2             */
#define OV5640_ZOOM_x1                      0x44U /* Set zoom to x1             */

/* Special Effect */
#define OV5640_COLOR_EFFECT_NONE            0x00U /* No effect                  */
#define OV5640_COLOR_EFFECT_BLUE            0x01U /* Blue effect                */
#define OV5640_COLOR_EFFECT_RED             0x02U /* Red effect                 */
#define OV5640_COLOR_EFFECT_GREEN           0x04U /* Green effect               */
#define OV5640_COLOR_EFFECT_BW              0x08U /* Black and White effect     */
#define OV5640_COLOR_EFFECT_SEPIA           0x10U /* Sepia effect               */
#define OV5640_COLOR_EFFECT_NEGATIVE        0x20U /* Negative effect            */

/* Light Mode */
#define OV5640_LIGHT_AUTO                   0x00U /* Light Mode Auto            */
#define OV5640_LIGHT_SUNNY                  0x01U /* Light Mode Sunny           */
#define OV5640_LIGHT_OFFICE                 0x02U /* Light Mode Office          */
#define OV5640_LIGHT_CLOUDY                 0x03U /* Light Mode Claudy          */
#define OV5640_LIGHT_HOME                   0x04U /* Light Mode Home            */

/* Night Mode */
#define NIGHT_MODE_DISABLE                  0x00U /* Disable night mode         */
#define NIGHT_MODE_ENABLE                   0x01U /* Enable night mode          */

/* Brightness */
#define OV5640_BRIGHTNESS_UP_4              0x00U /* Brightness +4               */
#define OV5640_BRIGHTNESS_UP_3              0x01U /* Brightness +3               */
#define OV5640_BRIGHTNESS_UP_2              0x02U /* Brightness +2               */
#define OV5640_BRIGHTNESS_UP_1              0x03U /* Brightness +1               */
#define OV5640_BRIGHTNESS_0                 0x04U /* Brightness 0 (default)      */
#define OV5640_BRIGHTNESS_DOWN_1            0x05U /* Brightness -1               */
#define OV5640_BRIGHTNESS_DOWN_2            0x06U /* Brightness -2               */
#define OV5640_BRIGHTNESS_DOWN_3            0x07U /* Brightness -3               */
#define OV5640_BRIGHTNESS_DOWN_4            0x08U /* Brightness -4               */

/* Contrast */
#define OV5640_CONTRAST_UP_3                0x00U /* Contrast +3               */
#define OV5640_CONTRAST_UP_2                0x01U /* Contrast +2               */
#define OV5640_CONTRAST_UP_1                0x02U /* Contrast +1               */
#define OV5640_CONTRAST_0                   0x03U /* Contrast 0 (default)      */
#define OV5640_CONTRAST_DOWN_1              0x04U /* Contrast -1               */
#define OV5640_CONTRAST_DOWN_2              0x05U /* Contrast -2               */
#define OV5640_CONTRAST_DOWN_3              0x06U /* Contrast -3               */

/* Saturation */
#define OV5640_SATURATION_UP_3              0x00U /* Saturation +3               */
#define OV5640_SATURATION_UP_2              0x01U /* Saturation +2               */
#define OV5640_SATURATION_UP_1              0x02U /* Saturation +1               */
#define OV5640_SATURATION_0                 0x03U /* Saturation 0 (default)      */
#define OV5640_SATURATION_DOWN_1            0x04U /* Saturation -1               */
#define OV5640_SATURATION_DOWN_2            0x05U /* Saturation -2               */
#define OV5640_SATURATION_DOWN_3            0x06U /* Saturation -3               */

/* exposure compensation */
#define OV5640_EXPOSURE_COMPENSATION_UP_3   0x00U /* Exposure compensation +3           */
#define OV5640_EXPOSURE_COMPENSATION_UP_2   0x01U /* Exposure compensation +2           */
#define OV5640_EXPOSURE_COMPENSATION_UP_1   0x02U /* Exposure compensation +1           */
#define OV5640_EXPOSURE_COMPENSATION_0      0x03U /* Exposure compensation 0 (default)  */
#define OV5640_EXPOSURE_COMPENSATION_DOWN_1 0x04U /* Exposure compensation -1           */
#define OV5640_EXPOSURE_COMPENSATION_DOWN_2 0x05U /* Exposure compensation -2           */
#define OV5640_EXPOSURE_COMPENSATION_DOWN_3 0x06U /* Exposure compensation -3           */

/* Colorbar Mode */
#define COLORBAR_MODE_DISABLE               0x00U /* Disable colorbar mode      */
#define COLORBAR_MODE_ENABLE                0x01U /* 8 bars W/Y/C/G/M/R/B/Bl    */
#define COLORBAR_MODE_GRADUALV              0x02U /* Gradual vertical colorbar  */

/* Pixel Clock */
#define OV5640_PCLK_7M                      0x00U /* Pixel Clock set to 7Mhz    */
#define OV5640_PCLK_8M                      0x01U /* Pixel Clock set to 8Mhz    */
#define OV5640_PCLK_9M                      0x02U /* Pixel Clock set to 9Mhz    */
#define OV5640_PCLK_12M                     0x04U /* Pixel Clock set to 12Mhz   */
#define OV5640_PCLK_24M                     0x08U /* Pixel Clock set to 24Mhz   */
#define OV5640_PCLK_48M                     0x09U /* Pixel Clock set to 48MHz   */

/* Mode */
#define PARALLEL_MODE                       0x00U /* Parallel Interface Mode */
#define SERIAL_MODE                         0x01U /* Serial Interface Mode   */

/**
 * @}
 */

#define OV5640_TRUE                         1
#define OV5640_FALSE                        0

#define OV5640_DVP_MODE                     0x01U
#define OV5640_MIPI_MODE                    0x02U

typedef struct camera_ins_s {
    uint8_t  resolution; // 分辨率
    uint8_t  mode;       // 模式
    uint32_t capture_ok;
} camera_ins_t;

extern camera_ins_t camera_ins;

void camera_init(void);
void camera_start(uint32_t buffer_address, uint32_t buffer_size, uint8_t is_predictive);
void camera_stop(void);

// 镜像翻转
void camera_set_mirror_flip(uint8_t mode);
void camera_set_format(uint8_t format);
void camera_set_brightness(uint8_t level);
void camera_set_contrast(uint8_t level);
void camera_set_saturation(uint8_t level);
void camera_set_exposure_compensation(uint8_t level);
void camera_set_lightmode(uint8_t mode);
void camera_set_nightmode(uint8_t mode);
void camera_set_resolution(uint8_t resolution);

static const uint16_t OV5640_INIT_Config[][2] =
    {
        {0x3008, 0x42}, // 系统电源控制，Bit[6]设置为1进入掉电模式，Bit[5:0]手册里没有说明作用
        {0x3103, 0x03}, // 系统时钟选择，设置使用PLL生成后的时钟，根据手册的配置，设置为0x03
        {0x3017, 0xff}, // PCLK、VS、HS以及数据引脚 D9~D6 使能输出
        {0x4740, 0X21}, // 设置PCLK、VS和HS的信号极性
        {0x3018, 0xff}, // 数据引脚 D5~D0 使能输出

        /*------- 时钟配置，可结合资料里的《时钟图》进行参考 -------------------------------------*/

        //	以下所有关于时钟的配置，都是在 XVCLK=24MHz 的前提下进行设置

        //	PLL预分频,bit[7:5]没有说明作用
        //	bit[4], PLL R divider，用来设置是否将经过0x3035寄存器配置之后的时钟进行分频，设置为0不分频，为1则是2分频
        // Bit[3:0]，PLL pre-divider，预分频，此处设置为3，即将 XVCLK 3分频后得到8M的时钟
        {0x3037, 0x13}, // 分频

        // Bit[7:0] 用于设置倍频参数，倍频值可以是4~127的任何数值，大于127时，只能设置为偶数
        // 此处设置为 100，即将上面分频得到的8M时钟进行100倍频得到800M时钟
        {0x3036, 0x64}, // 倍频

        // Bit[3:0] 用于设置MIPI时钟，这里用不到，保留默认值1
        // Bit[7:4]	分频系数，此处设置为1，即不分频，此时主时钟还是800M，
        // 如果需要降低帧率，可以设置为0x21，帧率会减半
        {0x3035, 0x11}, // 分频

        // 接着回到 0x3037 的 bit[4] 设置，上面设置为1，即2分频，此时时钟为400M
        // 0x3034的 Bit[3:0] 的取值会影响分频系数，设置为 8 是2分频，A是2.5分频，其它值则不分频
        // 此处设置是2.5分频，此时主时钟为 160M
        {0x3034, 0x1A}, // 手册的默认值，0x1A

        // 接下来这个寄存器是设置 PCLK（也就是DCMI接口里的像素时钟）	和 SCLK（应该是OV5640内部的各个处理单元的时钟）
        // Bit[5:4]: PCLK 分频，此处为0，即不分频
        // Bit[3:0] 都是和SCLK有关，直接使用手册给的参考值即可
        {0x3108, 0x01}, // 分频

        // 0x460c寄存器的Bit[7:4]	用于设置JPEG空数据速度，默认值0x20
        // 0x460c寄存器的Bit[1]		用于设置PCLK的分频值，为0则是自动控制模式，设置为1时，PCLK的分频值由0x3824寄存器控制
        // 自动模式下，生成的时钟非常高，通常都是80M，如果用户对EMC、 EMI有需求，可以手动调整
        {0x460c, 0x20},

        // Bit[4:0]有效，只能设置为 1、2、4、8、16 这几个分频值
        // 在上面的配置中，最终生成了160M的时钟，然后再根据0x3824寄存器的分频值得到最终的PCLK输出时钟
        // STM32H750/743的DCMI接口所允许的最高速度为80M，所以不管是自动配置还是手动配置，都不能超过这个值
        {0x3824, 0x02}, // PCLK分频系数 ， 0x460c寄存器的Bit[1]	设置为1时才有效												fanke

        /*------------------------------------------------------------------  时钟配置结束 -----*/

        // 手册里没有说明这些寄存器的作用,这里直接保留手册给的设置参数
        {0x3630, 0x36},
        {0x3631, 0x0e},
        {0x3632, 0xe2},
        {0x3633, 0x12},
        {0x3621, 0xe0},
        {0x3704, 0xa0}, // FanKe
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
        {0x3635, 0x13},
        {0x3636, 0x03},
        {0x3634, 0x40},
        {0x3622, 0x01},
        {0x440e, 0x00},
        {0x5025, 0x00},
        {0x3618, 0x00},
        {0x3612, 0x29},
        {0x3708, 0x64},
        {0x3709, 0x52},
        {0x370c, 0x03},
        {0x302e, 0x00},
        {0x460b, 0x37},

        {0x3000, 0x00}, // 使能所有系统单元，包括 BIST、MCU、OTP等
        {0x3002, 0x1c}, // 复位 JFIFO, SFIFO, JPG
        {0x3004, 0xff}, // 使能所有系统单元，包括 BIST、MCU、OTP等
        {0x3006, 0xc3}, // 禁止 JPEG2x, JPEG	的时钟
        {0x300e, 0x58}, // 禁止 MIPI，使用DVP接口（也就是STM32的DCMI接口）

        // 设置数据接口输出的格式，Bit[7:4]用于设置 RGB或者YUV等 格式，Bit[3:0]设置输出的序列，例如是BGR还是RGB
        // 此处设置为RGB565格式，序列为 G[2:0]B[4:0], R[4:0]G[5:3]
        {0x4300, 0x6F},
        // 除了要设置接口的输出格式，ISP（image sensor processor）图像处理单元也要设置成对应的格式
        {0X501F, 0x01}, // ISP格式设置在Bit[2:0] ，此处使用RGB格式
        {0x5000, 0xa7}, // ISP设置，使能 LENC补偿、黑色像素、白色像素以及颜色插值（CIP）
        {0x5001, 0xA3}, // ISP设置，使能 SDE特效、图像缩放、Color Matrix 色彩矩阵	、AWB 自动白平衡

        {0x3820, 0x47}, // Bit[2:1]用于设置是否垂直翻转，其余位的作用手册里没有说明
        {0x3821, 0x01}, // Bit[2:1]用于设置是否水平镜像，Bit[0]用于使能水平像素合并

        /*------- 窗口配置，参考OV5640数据手册 4.2 小节 image windowing ------------------------*/

        // OV5640有好几个窗口的概念
        // 摄像头的物理像素窗口：2624*1954 （包含黑电平矫正线和空像素，有效分辨率为2592*1944）
        // ISP（image sensor processor）输入窗口：需要进行处理的像素窗口
        // 预缩放窗口：ISP窗口的基础上，调整用于缩放输出的窗口
        // 输出窗口： 根据预缩放窗口和要输出的分辨率，进行缩放，得到最终的图像

        // 因为摄像头的默认像素比例是 2592/1944 = 4/3 （15帧），而我们实际使用的屏幕往往不是这个比例或者不需要这么高像素，
        // 因此需要做一定的调整，最终再配合DCMI的窗口裁剪以匹配屏幕。
        // （注：用户也可以直接使用OV5640裁剪 物理窗口 得到对应比例的 ISP窗口，然后缩放偏移按照默认设置即可，
        // 例如可以直接将ISP窗口设置为 240/280等对应实际屏幕的比例，而无需DCMI去裁剪。不过为了例程的通用性，我们选择
        // 使用 4:3固定比例+DCMI裁剪的方式	）

        // 以下配置为 4:3(1280*960) 43帧 的配置，
        // 可以设置 0x3035寄存器 为0x21，帧率会减半
        // JPG模式2、3情况下，帧率也会减半
        {0x3800, 0x00}, // HS，ISP窗口水平起始坐标 0
        {0x3801, 0x00}, // HS
        {0x3802, 0x00}, // VS，ISP窗口垂直起始坐标 4
        {0x3803, 0x04}, // VS
        {0x3804, 0x0a}, // HW (HE)，ISP窗口水平终点坐标2623 ,实际尺寸=2624
        {0x3805, 0x3f}, // HW (HE)
        {0x3806, 0x07}, // VH (VE)，ISP窗口垂直终点坐标1947 ,实际尺寸=1947-4+1=1944
        {0x3807, 0x9b}, // VH (VE)
        {0x380c, 0x07}, // HTS，输出的水平总尺寸，不懂要如何取值，手册没有说，但这里是影响帧率的最重要配置，这里直接使用手册给的参考值
        {0x380d, 0x68}, // HTS
        {0x380e, 0x03}, // VTS，输出的垂直总尺寸，不懂要如何取值，手册没有说，但这里是影响帧率的最重要配置，这里直接使用手册给的参考值
        {0x380f, 0xd8}, // VTS

        // 预缩放窗口的设置，用于设置 在ISP窗口的基础上，根据偏移量的多少，得到预缩放窗口，
        // 这里直接使用OV5640的默认配置，水平偏移16，垂直偏移4，因为包含了一些空像素和无效行，不能取0
        {0x3810, 0x00}, // 水平偏移高字节
        {0x3811, 0x10}, // 水平偏移低字节
        {0x3812, 0x00}, // 垂直偏移高字节
        {0x3813, 0x04}, // 垂直偏移低字节

        // 以下两个寄存器，手册的描述是 水平和垂直二次采样时，奇数和偶数的增量，
        // 笔者猜测的理解是：因为OV5640的全画幅分辨率很大，当实际输出的图像尺寸小于最大分辨率时，
        // 需要通过裁剪或者2次采样，这两个寄存器用于控制每多少个像素去合并成一个像素，但是能找到
        //	的信息有限，因此只能根据手册的给出的参考代码去配置
        {0x3814, 0x31}, // timing X inc
        {0x3815, 0x31}, // timing Y inc

        /*------------------------------------------------------------------  窗口配置结束 -----*/

        // BLC（Black Level Calibration ）黑电平校正
        // OV5640的像素阵列包含几条光学屏蔽(黑色)线，这些线被用作黑电平校准的参考，
        // 不清楚要如何去配置，这里直接使用 OV5640应用指南 里的设置
        {0x4001, 0x02},
        {0x4004, 0x02},
        {0x4005, 0x1a},

        // 曝光时间相关
        // 不清楚要如何去配置，这里使用的是 OpenMV 的配置
        {0x3a02, 0x05},
        {0x3a03, 0xc4},
        {0x3a08, 0x00},
        {0x3a09, 0x93},
        {0x3a0a, 0x00},
        {0x3a0b, 0x7b},
        {0x3a0d, 0x08},
        {0x3a0e, 0x06},
        {0x3a14, 0x05},
        {0x3a15, 0xc4},

        // AEC 增益相关
        // 不清楚要如何去配置，这里直接使用 OV5640应用指南 里的设置
        {0x3a13, 0x43},
        {0x3a18, 0x00},
        {0x3a19, 0xf8},

        // 50/60Hz 灯光条纹过滤
        // 不清楚要如何去配置，这里直接使用 OV5640应用指南 里的设置
        {0x3c01, 0x34},
        {0x3c04, 0x28},
        {0x3c05, 0x98},
        {0x3c06, 0x00},
        {0x3c07, 0x08},
        {0x3c08, 0x00},
        {0x3c09, 0x1c},
        {0x3c0a, 0x9c},
        {0x3c0b, 0x40},

        // AWB 自动白平衡
        // 不清楚要如何去配置，这里直接使用 OV5640应用指南 里的设置
        {0x5180, 0xff},
        {0x5181, 0xf2},
        {0x5182, 0x00},
        {0x5183, 0x14},
        {0x5184, 0x25},
        {0x5185, 0x24},
        {0x5186, 0x09},
        {0x5187, 0x09},
        {0x5188, 0x09},
        {0x5189, 0x75},
        {0x518a, 0x54},
        {0x518b, 0xe0},
        {0x518c, 0xb2},
        {0x518d, 0x42},
        {0x518e, 0x3d},
        {0x518f, 0x56},
        {0x5190, 0x46},
        {0x5191, 0xf8},
        {0x5192, 0x04},
        {0x5193, 0x70},
        {0x5194, 0xf0},
        {0x5195, 0xf0},
        {0x5196, 0x03},
        {0x5197, 0x01},
        {0x5198, 0x04},
        {0x5199, 0x12},
        {0x519a, 0x04},
        {0x519b, 0x00},
        {0x519c, 0x06},
        {0x519d, 0x82},
        {0x519e, 0x38},

        // color matrix 色彩矩阵
        // 不清楚要如何去配置，这里直接使用 OV5640应用指南 里的设置
        {0x5381, 0x1e},
        {0x5382, 0x5b},
        {0x5383, 0x08},
        {0x5384, 0x0a},
        {0x5385, 0x7e},
        {0x5386, 0x88},
        {0x5387, 0x7c},
        {0x5388, 0x6c},
        {0x5389, 0x10},
        {0x538a, 0x01},
        {0x538b, 0x98},

        // CIP 锐化和降噪
        // 不清楚要如何去配置，这里直接使用 OV5640应用指南 里的设置
        {0x5300, 0x08},
        {0x5301, 0x30},
        {0x5302, 0x10},
        {0x5303, 0x00},
        {0x5304, 0x08},
        {0x5305, 0x30},
        {0x5306, 0x08},
        {0x5307, 0x16},
        {0x5309, 0x08},
        {0x530a, 0x30},
        {0x530b, 0x04},
        {0x530c, 0x06},

        // Gamma 伽玛曲线
        // 不清楚要如何去配置，这里直接使用 OV5640应用指南 里的设置
        {0x5480, 0x01},
        {0x5481, 0x08},
        {0x5482, 0x14},
        {0x5483, 0x28},
        {0x5484, 0x51},
        {0x5485, 0x65},
        {0x5486, 0x71},
        {0x5487, 0x7d},
        {0x5488, 0x87},
        {0x5489, 0x91},
        {0x548a, 0x9a},
        {0x548b, 0xaa},
        {0x548c, 0xb8},
        {0x548d, 0xcd},
        {0x548e, 0xdd},
        {0x548f, 0xea},
        {0x5490, 0x1d},

        // UV adjust
        // 不清楚要如何去配置，这里直接使用 OV5640应用指南 里的设置
        {0x5580, 0x06},
        {0x5583, 0x40},
        {0x5584, 0x10},
        {0x5589, 0x10},
        {0x558a, 0x00},
        {0x558b, 0xf8},
        {0x501d, 0x40},

        // AEC 自动曝光补偿
        // 不清楚要如何去配置，这里直接使用 OV5640应用指南 里的设置，手册里还给出了好几种曝光设置
        {0x3a0f, 0x30},
        {0x3a10, 0x28},
        {0x3a1b, 0x30},
        {0x3a1e, 0x26},
        {0x3a11, 0x60},
        {0x3a1f, 0x14},

        // AWB 环境光配置自动模式
        // 不清楚要如何去配置，这里直接使用 OV5640应用指南 里的设置，手册里还给出了好几种环境光设置
        {0x3406, 0x00},
        {0x3400, 0x04},
        {0x3401, 0x00},
        {0x3402, 0x04},
        {0x3403, 0x00},
        {0x3404, 0x04},
        {0x3405, 0x00},

        // lens correction (LENC) 镜头补偿设置
        // 这些设置主要为了弥补镜头的缺陷，通过计算增益来校正每个像素，以补偿改善由于透镜曲率造成的光分布状况，
        //	这些配置严格上是需要摄像头模组厂家和 OmniVision FAE 联合去设置的，但由于条件有限，这里直接
        // 使用 OV5640应用指南 里的设置
        {0x5800, 0x23},
        {0x5801, 0x14},
        {0x5802, 0x0f},
        {0x5803, 0x0f},
        {0x5804, 0x12},
        {0x5805, 0x26},
        {0x5806, 0x0c},
        {0x5807, 0x08},
        {0x5808, 0x05},
        {0x5809, 0x05},
        {0x580a, 0x08},
        {0x580b, 0x0d},
        {0x580c, 0x08},
        {0x580d, 0x03},
        {0x580e, 0x00},
        {0x580f, 0x00},
        {0x5810, 0x03},
        {0x5811, 0x09},
        {0x5812, 0x07},
        {0x5813, 0x03},
        {0x5814, 0x00},
        {0x5815, 0x01},
        {0x5816, 0x03},
        {0x5817, 0x08},
        {0x5818, 0x0d},
        {0x5819, 0x08},
        {0x581a, 0x05},
        {0x581b, 0x06},
        {0x581c, 0x08},
        {0x581d, 0x0e},
        {0x581e, 0x29},
        {0x581f, 0x17},
        {0x5820, 0x11},
        {0x5821, 0x11},
        {0x5822, 0x15},
        {0x5823, 0x28},
        {0x5824, 0x46},
        {0x5825, 0x26},
        {0x5826, 0x08},
        {0x5827, 0x26},
        {0x5828, 0x64},
        {0x5829, 0x26},
        {0x582a, 0x24},
        {0x582b, 0x22},
        {0x582c, 0x24},
        {0x582d, 0x24},
        {0x582e, 0x06},
        {0x582f, 0x22},
        {0x5830, 0x40},
        {0x5831, 0x42},
        {0x5832, 0x24},
        {0x5833, 0x26},
        {0x5834, 0x24},
        {0x5835, 0x22},
        {0x5836, 0x22},
        {0x5837, 0x26},
        {0x5838, 0x44},
        {0x5839, 0x24},
        {0x583a, 0x26},
        {0x583b, 0x28},
        {0x583c, 0x42},
        {0x583d, 0xce},

        // 系统电源控制，Bit[5:0]手册里没有说明作用，根据应用指南的说明该配置是从掉电模式中唤醒													fk
        {0x3008, 0x02},

};

#endif /* CAMERA_H */
