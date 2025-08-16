#ifndef __ST7789_H
#define __ST7789_H

#include "fonts.h"
#include "board.h"

/* user configure */
// #define CFG_NO_CS
#define CFG_NO_RST

/* choose a Hardware SPI port to use. */
#define ST7789_SPI_PORT hspi6
extern SPI_HandleTypeDef ST7789_SPI_PORT;

/* choose whether use DMA or not */
#define ST7789_USE_DMA

/* soft spi chip select */
#ifndef CFG_NO_CS
#define ST7789_CS_PORT GPIOG
#define ST7789_CS_PIN  GPIO_PIN_8
#endif

/* gpio hard reset */
#ifndef CFG_NO_RST
#define ST7789_RST_PORT ST7789_RST_GPIO_Port
#define ST7789_RST_PIN  ST7789_RST_Pin
#endif

/* Data or Command specification pin*/
#define ST7789_DC_PORT          GPIOG
#define ST7789_DC_PIN           GPIO_PIN_15

/* Backlight control */
#define ST7789_BLK_PORT         GPIOG
#define ST7789_BLK_PIN          GPIO_PIN_12

/* Choose a display rotation you want to use: (0-3) */
#define ST7789_ROTATION_0       0 // default
#define ST7789_ROTATION_1       1 // mirror and flip
#define ST7789_ROTATION_2       2 // 90 degree
#define ST7789_ROTATION_3       3 // 270 degree

#define ST7789_WIDTH            240
#define ST7789_HEIGHT           320
#define X_SHIFT                 0
#define Y_SHIFT                 0

/**
 *Color of pen
 *If you want to use another color, you can choose one in RGB565 format.
 */

#define WHITE                   0xFFFF
#define BLACK                   0x0000
#define BLUE                    0x001F
#define RED                     0xF800
#define MAGENTA                 0xF81F
#define GREEN                   0x07E0
#define CYAN                    0x7FFF
#define YELLOW                  0xFFE0
#define GRAY                    0X8430
#define BRED                    0XF81F
#define GRED                    0XFFE0
#define GBLUE                   0X07FF
#define BROWN                   0XBC40
#define BRRED                   0XFC07
#define DARKBLUE                0X01CF
#define LIGHTBLUE               0X7D7C
#define GRAYBLUE                0X5458

#define LIGHTGREEN              0X841F
#define LGRAY                   0XC618
#define LGRAYBLUE               0XA651
#define LBBLUE                  0X2B12

/* Control Registers and constant codes */
#define ST7789_NOP              0x00
#define ST7789_SWRESET          0x01
#define ST7789_RDDID            0x04
#define ST7789_RDDST            0x09

#define ST7789_SLPIN            0x10
#define ST7789_SLPOUT           0x11
#define ST7789_PTLON            0x12
#define ST7789_NORON            0x13

#define ST7789_INVOFF           0x20
#define ST7789_INVON            0x21
#define ST7789_DISPOFF          0x28
#define ST7789_DISPON           0x29
#define ST7789_CASET            0x2A
#define ST7789_RASET            0x2B
#define ST7789_RAMWR            0x2C
#define ST7789_RAMRD            0x2E

#define ST7789_PTLAR            0x30
#define ST7789_COLMOD           0x3A
#define ST7789_MADCTL           0x36

/**
 * Memory Data Access Control Register (0x36H)
 * MAP:     D7  D6  D5  D4  D3  D2  D1  D0
 * param:   MY  MX  MV  ML  RGB MH  -   -
 *
 */

/* Page Address Order, Y-axis ('0': Top to Bottom, '1': the opposite) */
#define ST7789_MADCTL_MY        0x80
/* Column Address Order, X-axis ('0': Left to Right, '1': the opposite) */
#define ST7789_MADCTL_MX        0x40
/* Page/Column Order, all axis ('0' = Normal Mode, '1' = Reverse Mode) */
#define ST7789_MADCTL_MV        0x20
/* Line Refresh Order ('0' = LCD Refresh Top to Bottom, '1' = the opposite) */
#define ST7789_MADCTL_ML        0x10
/* RGB/BGR Order ('0' = RGB, '1' = BGR) */
#define ST7789_MADCTL_RGB       0x08
/* Horizontal Refresh Order ('0' = LCD Refresh Left to Right, '1' = the opposite) */
#define ST7789_MADCTL_MH        0x04

#define ST7789_RDID1            0xDA
#define ST7789_RDID2            0xDB
#define ST7789_RDID3            0xDC
#define ST7789_RDID4            0xDD

/* Advanced options */
#define ST7789_COLOR_MODE_16bit 0x55 //  RGB565 (16bit)
#define ST7789_COLOR_MODE_18bit 0x66 //  RGB666 (18bit)

/* Basic operations */
#define ST7789_DC_RESET()       GPIO_RESET_PIN(ST7789_DC_PORT, ST7789_DC_PIN)
#define ST7789_DC_SET()         GPIO_SET_PIN(ST7789_DC_PORT, ST7789_DC_PIN)

#ifndef CFG_NO_RST
#define ST7789_RST_RESET() GPIO_RESET_PIN(ST7789_RST_PORT, ST7789_RST_PIN)
#define ST7789_RST_SET()   GPIO_SET_PIN(ST7789_RST_PORT, ST7789_RST_PIN)
#else
#define ST7789_RST_RESET() asm("nop")
#define ST7789_RST_SET()   asm("nop")
#endif

#ifndef CFG_NO_CS
#define ST7789_SELECT()   GPIO_RESET_PIN(ST7789_CS_PORT, ST7789_CS_PIN)
#define ST7789_UNSELECT() GPIO_SET_PIN(ST7789_CS_PORT, ST7789_CS_PIN)
#else
#define ST7789_SELECT()   asm("nop")
#define ST7789_UNSELECT() asm("nop")
#endif

#define ABS(x) ((x) > 0 ? (x) : -(x))

/* Basic functions. */
void st7789_init(void);
void st7789_set_rotation(uint8_t m);
void st7789_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void st7789_draw_pixel_4px(uint16_t x, uint16_t y, uint16_t color);
void st7789_fill_window(uint16_t color);
void st7789_fill_area(uint16_t xSta, uint16_t ySta, uint16_t xEnd, uint16_t yEnd, uint16_t color);

/* Graphical functions. */
void st7789_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void st7789_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void st7789_draw_circle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);
void st7789_draw_image(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data);
void st7789_invert_colors(uint8_t invert);

/* Text functions. */
void st7789_write_char(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor);
void st7789_write_string(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor);

/* Extented Graphical functions. */
void st7789_draw_rectangle_filled(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void st7789_draw_triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color);
void st7789_draw_triangle_filled(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color);
void st7789_draw_circle_filled(int16_t x0, int16_t y0, int16_t r, uint16_t color);

/* Command functions */
void st7789_tear_effect(uint8_t tear);

/* Simple test function. */
void st7789_test(void);

#endif
