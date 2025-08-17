#include "st7789.h"
#include "board.h"
#include "stm32h7xx_hal_gpio.h"

#ifdef ST7789_USE_DMA
#include <string.h>
const uint16_t DMA_MIN_SIZE = 16;
/* If you're using DMA, then u need a "framebuffer" to store datas to be displayed.
 * If your MCU don't have enough RAM, please avoid using DMA(or set 5 to 1).
 * And if your MCU have enough RAM(even larger than full-frame size),
 * Then you can specify the framebuffer size to the full resolution below.
 */
#define BUF_HOR_LEN 5
SRAM_SET_RAM_D3 uint16_t display_buffer[ST7789_WIDTH * BUF_HOR_LEN]; // 240x320 RGB565

void memset_16(uint16_t* buf, uint16_t value, uint32_t size)
{
    uint16_t swapped = (value >> 8) | (value << 8);
    while (size > 0) {
        *buf = swapped;
        buf++;
        size--;
    }
}

#endif

#define ST7789_DELAY_MS(x) HAL_Delay(x)

/**
 * @brief Write command to ST7789 controller
 * @param cmd -> command to write
 * @return none
 */
static void st7789_write_command(uint8_t cmd)
{
    ST7789_SELECT();
    ST7789_DC_RESET();
    HAL_SPI_Transmit(&ST7789_SPI_PORT, &cmd, sizeof(cmd), HAL_MAX_DELAY);
    ST7789_UNSELECT();
}

/**
 * @brief Write data to ST7789 controller
 * @param buff -> pointer of data buffer
 * @param buff_size -> size of the data buffer
 * @return none
 */
static void st7789_write_data(uint8_t* buff, size_t buff_size)
{
    ST7789_SELECT();
    ST7789_DC_SET();

    // split data in small chunks because HAL can't send more than 64K at once

    while (buff_size > 0) {
        uint16_t chunk_size = buff_size > 65535 ? 65535 : buff_size;
#ifdef ST7789_USE_DMA
        if (DMA_MIN_SIZE <= buff_size) {
            HAL_SPI_Transmit_DMA(&ST7789_SPI_PORT, buff, chunk_size);
            while (ST7789_SPI_PORT.State != HAL_SPI_STATE_READY) {}
            // HAL_SPI_Transmit(&ST7789_SPI_PORT, buff, chunk_size, HAL_MAX_DELAY);
        } else
            HAL_SPI_Transmit(&ST7789_SPI_PORT, buff, chunk_size, HAL_MAX_DELAY);
#else
        HAL_SPI_Transmit(&ST7789_SPI_PORT, buff, chunk_size, HAL_MAX_DELAY);
#endif

        buff += chunk_size;
        buff_size -= chunk_size;
    }

    ST7789_UNSELECT();
}
/**
 * @brief Write data to ST7789 controller, simplify for 8bit data.
 * data -> data to write
 * @return none
 */
static void st7789_write_data_byte(uint8_t data)
{
    ST7789_SELECT();
    ST7789_DC_SET();
    HAL_SPI_Transmit(&ST7789_SPI_PORT, &data, sizeof(data), HAL_MAX_DELAY);
    ST7789_UNSELECT();
}

/**
 * @brief Set the rotation direction of the display
 * @param m -> rotation parameter(please refer it in st7789.h)
 * @return none
 */
void st7789_set_rotation(uint8_t m)
{
    st7789_write_command(ST7789_MADCTL); // MADCTL
    switch (m) {
        case 0:
            st7789_write_data_byte(0x00);
            break;
        case 1:
            st7789_write_data_byte(ST7789_MADCTL_MX | ST7789_MADCTL_MY);
            break;
        case 2:
            st7789_write_data_byte(ST7789_MADCTL_MY | ST7789_MADCTL_MV);
            break;
        case 3:
            st7789_write_data_byte(ST7789_MADCTL_MX | ST7789_MADCTL_MV);
            break;
        default:
            break;
    }
}

/**
 * @brief Set address of DisplayWindow
 * @param xi&yi -> coordinates of window
 * @return none
 */
static void st7789_set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    ST7789_SELECT();
    uint16_t x_start = x0 + X_SHIFT, x_end = x1 + X_SHIFT;
    uint16_t y_start = y0 + Y_SHIFT, y_end = y1 + Y_SHIFT;

    /* Column Address set */
    st7789_write_command(ST7789_CASET);
    {
        uint8_t data[] = {x_start >> 8, x_start & 0xFF, x_end >> 8, x_end & 0xFF};
        st7789_write_data(data, sizeof(data));
    }

    /* Row Address set */
    st7789_write_command(ST7789_RASET);
    {
        uint8_t data[] = {y_start >> 8, y_start & 0xFF, y_end >> 8, y_end & 0xFF};
        st7789_write_data(data, sizeof(data));
    }
    /* Write to RAM */
    st7789_write_command(ST7789_RAMWR);
    ST7789_UNSELECT();
}

/**
 * @brief Initialize ST7789 controller
 * @param none
 * @return none
 */
void st7789_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    gpio_clk_init(ST7789_DC_PORT);
    GPIO_InitStruct.Pin   = ST7789_DC_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(ST7789_DC_PORT, &GPIO_InitStruct);

    gpio_clk_init(ST7789_BLK_PORT);
    GPIO_InitStruct.Pin   = ST7789_BLK_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(ST7789_BLK_PORT, &GPIO_InitStruct);

#ifndef CFG_NO_RST
    gpio_clk_init(ST7789_RST_PORT);
    GPIO_InitStruct.Pin   = ST7789_RST_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ST7789_RST_PORT, &GPIO_InitStruct);
#endif // CFG_NO_RST

#ifndef CFG_NO_CS
    gpio_clk_init(ST7789_CS_PORT);
    GPIO_InitStruct.Pin   = ST7789_CS_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ST7789_CS_PORT, &GPIO_InitStruct);
#endif // CFG_NO_CS

#ifdef ST7789_USE_DMA
    memset(display_buffer, 0, sizeof(display_buffer));
    extern void MX_BDMA_Init(void);
    MX_BDMA_Init();
#endif

    extern void MX_SPI6_Init(void);
    MX_SPI6_Init();

    ST7789_DELAY_MS(10);
    ST7789_RST_RESET();
    ST7789_DELAY_MS(10);
    ST7789_RST_SET();
    ST7789_DELAY_MS(20);

    st7789_write_command(ST7789_COLMOD); //	Set color mode
    st7789_write_data_byte(ST7789_COLOR_MODE_16bit);
    st7789_write_command(0xB2); //	Porch control
    {
        uint8_t data[] = {0x0C, 0x0C, 0x00, 0x33, 0x33};
        st7789_write_data(data, sizeof(data));
    }
    st7789_set_rotation(ST7789_ROTATION_0); //	MADCTL (Display Rotation)

    /* Internal LCD Voltage generator settings */
    st7789_write_command(0XB7);   //	Gate Control
    st7789_write_data_byte(0x35); //	Default value
    st7789_write_command(0xBB);   //	VCOM setting
    st7789_write_data_byte(0x19); //	0.725v (default 0.75v for 0x20)
    st7789_write_command(0xC0);   //	LCMCTRL
    st7789_write_data_byte(0x2C); //	Default value
    st7789_write_command(0xC2);   //	VDV and VRH command Enable
    st7789_write_data_byte(0x01); //	Default value
    st7789_write_command(0xC3);   //	VRH set
    st7789_write_data_byte(0x12); //	+-4.45v (defalut +-4.1v for 0x0B)
    st7789_write_command(0xC4);   //	VDV set
    st7789_write_data_byte(0x20); //	Default value
    st7789_write_command(0xC6);   //	Frame rate control in normal mode
    st7789_write_data_byte(0x0F); //	Default value (60HZ)
    st7789_write_command(0xD0);   //	Power control
    st7789_write_data_byte(0xA4); //	Default value
    st7789_write_data_byte(0xA1); //	Default value
    /**************** Division line ****************/

    st7789_write_command(0xE0);
    {
        uint8_t data[] = {0xD0, 0x04, 0x0D, 0x11, 0x13, 0x2B, 0x3F, 0x54, 0x4C, 0x18, 0x0D, 0x0B, 0x1F, 0x23};
        st7789_write_data(data, sizeof(data));
    }

    st7789_write_command(0xE1);
    {
        uint8_t data[] = {0xD0, 0x04, 0x0C, 0x11, 0x13, 0x2C, 0x3F, 0x44, 0x51, 0x2F, 0x1F, 0x1F, 0x20, 0x23};
        st7789_write_data(data, sizeof(data));
    }
    st7789_write_command(ST7789_INVON);  //	Inversion ON
    st7789_write_command(ST7789_SLPOUT); //	Out of sleep mode
    st7789_write_command(ST7789_NORON);  //	Normal Display on
    st7789_write_command(ST7789_DISPON); //	Main screen turned on

    GPIO_SET_PIN(ST7789_BLK_PORT, ST7789_BLK_PIN);
    ST7789_DELAY_MS(50);
    st7789_fill_window(GREEN); //	Fill with Black.
}

/**
 * @brief Fill the DisplayWindow with single color
 * @param color -> color to Fill with
 * @return none
 */
void st7789_fill_window(uint16_t color)
{
    uint16_t i;
    st7789_set_address_window(0, 0, ST7789_WIDTH - 1, ST7789_HEIGHT - 1);
    ST7789_SELECT();
#ifdef ST7789_USE_DMA
    for (i = 0; i < ST7789_HEIGHT / BUF_HOR_LEN; i++) {
        memset(display_buffer, color, sizeof(display_buffer));
        st7789_write_data((uint8_t*)display_buffer, sizeof(display_buffer));
    }
#else
    uint16_t j;
    for (i = 0; i < ST7789_WIDTH; i++)
        for (j = 0; j < ST7789_HEIGHT; j++) {
            uint8_t data[] = {color >> 8, color & 0xFF};
            st7789_write_data(data, sizeof(data));
        }
#endif
    ST7789_UNSELECT();
}

/**
 * @brief Draw a Pixel
 * @param x&y -> coordinate to Draw
 * @param color -> color of the Pixel
 * @return none
 */
void st7789_draw_pixel(uint16_t x, uint16_t y, uint16_t color)
{
    if ((x < 0) || (x >= ST7789_WIDTH) ||
        (y < 0) || (y >= ST7789_HEIGHT)) return;

    st7789_set_address_window(x, y, x, y);
    uint8_t data[] = {color >> 8, color & 0xFF};
    ST7789_SELECT();
    st7789_write_data(data, sizeof(data));
    ST7789_UNSELECT();
}

/**
 * @brief Fill an Area with single color
 * @param xSta&ySta -> coordinate of the start point
 * @param xEnd&yEnd -> coordinate of the end point
 * @param color -> color to Fill with
 * @return none
 */
void st7789_fill_area(uint16_t xSta, uint16_t ySta, uint16_t xEnd, uint16_t yEnd, uint16_t color)
{
    if ((xEnd < 0) || (xEnd >= ST7789_WIDTH) ||
        (yEnd < 0) || (yEnd >= ST7789_HEIGHT)) return;
    ST7789_SELECT();
    uint16_t i, j;
    st7789_set_address_window(xSta, ySta, xEnd, yEnd);
    for (i = ySta; i <= yEnd; i++)
        for (j = xSta; j <= xEnd; j++) {
            uint8_t data[] = {color >> 8, color & 0xFF};
            st7789_write_data(data, sizeof(data));
        }
    ST7789_UNSELECT();
}

/**
 * @brief Draw a big Pixel at a point
 * @param x&y -> coordinate of the point
 * @param color -> color of the Pixel
 * @return none
 */
void st7789_draw_pixel_4px(uint16_t x, uint16_t y, uint16_t color)
{
    if ((x <= 0) || (x > ST7789_WIDTH) ||
        (y <= 0) || (y > ST7789_HEIGHT)) return;
    ST7789_SELECT();
    st7789_fill_area(x - 1, y - 1, x + 1, y + 1, color);
    ST7789_UNSELECT();
}

/**
 * @brief Draw a line with single color
 * @param x1&y1 -> coordinate of the start point
 * @param x2&y2 -> coordinate of the end point
 * @param color -> color of the line to Draw
 * @return none
 */
void st7789_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
                      uint16_t color)
{
    uint16_t swap;
    uint16_t steep = ABS(y1 - y0) > ABS(x1 - x0);
    if (steep) {
        swap = x0;
        x0   = y0;
        y0   = swap;

        swap = x1;
        x1   = y1;
        y1   = swap;
    }

    if (x0 > x1) {
        swap = x0;
        x0   = x1;
        x1   = swap;

        swap = y0;
        y0   = y1;
        y1   = swap;
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = ABS(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x0 <= x1; x0++) {
        if (steep) {
            st7789_draw_pixel(y0, x0, color);
        } else {
            st7789_draw_pixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

/**
 * @brief Draw a Rectangle with single color
 * @param xi&yi -> 2 coordinates of 2 top points.
 * @param color -> color of the Rectangle line
 * @return none
 */
void st7789_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    ST7789_SELECT();
    st7789_draw_line(x1, y1, x2, y1, color);
    st7789_draw_line(x1, y1, x1, y2, color);
    st7789_draw_line(x1, y2, x2, y2, color);
    st7789_draw_line(x2, y1, x2, y2, color);
    ST7789_UNSELECT();
}

/**
 * @brief Draw a circle with single color
 * @param x0&y0 -> coordinate of circle center
 * @param r -> radius of circle
 * @param color -> color of circle line
 * @return  none
 */
void st7789_draw_circle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color)
{
    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;

    ST7789_SELECT();
    st7789_draw_pixel(x0, y0 + r, color);
    st7789_draw_pixel(x0, y0 - r, color);
    st7789_draw_pixel(x0 + r, y0, color);
    st7789_draw_pixel(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        st7789_draw_pixel(x0 + x, y0 + y, color);
        st7789_draw_pixel(x0 - x, y0 + y, color);
        st7789_draw_pixel(x0 + x, y0 - y, color);
        st7789_draw_pixel(x0 - x, y0 - y, color);

        st7789_draw_pixel(x0 + y, y0 + x, color);
        st7789_draw_pixel(x0 - y, y0 + x, color);
        st7789_draw_pixel(x0 + y, y0 - x, color);
        st7789_draw_pixel(x0 - y, y0 - x, color);
    }
    ST7789_UNSELECT();
}

/**
 * @brief Draw an Image on the screen
 * @param x&y -> start point of the Image
 * @param w&h -> width & height of the Image to Draw
 * @param data -> pointer of the Image array
 * @return none
 */
void st7789_draw_image(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data)
{
    if ((x >= ST7789_WIDTH) || (y >= ST7789_HEIGHT))
        return;
    if ((x + w - 1) >= ST7789_WIDTH)
        return;
    if ((y + h - 1) >= ST7789_HEIGHT)
        return;

    ST7789_SELECT();
    st7789_set_address_window(x, y, x + w - 1, y + h - 1);
    // for (uint16_t i = 0; i < w; i++)
    //     for (uint16_t j = 0; j < h; j++) {
    //         uint8_t tmp[] = {data[i * h + j] >> 8, data[i * h + j] & 0xFF};
    //         st7789_write_data(tmp, sizeof(tmp));
    //     }
    st7789_write_data((uint8_t*)data, sizeof(uint16_t) * w * h);
    ST7789_UNSELECT();
}

/**
 * @brief Invert Fullscreen color
 * @param invert -> Whether to invert
 * @return none
 */
void st7789_invert_colors(uint8_t invert)
{
    ST7789_SELECT();
    st7789_write_command(invert ? 0x21 /* INVON */ : 0x20 /* INVOFF */);
    ST7789_UNSELECT();
}

/**
 * @brief Write a char
 * @param  x&y -> cursor of the start point.
 * @param ch -> char to write
 * @param font -> fontstyle of the string
 * @param color -> color of the char
 * @param bgcolor -> background color of the char
 * @return  none
 */
void st7789_write_char(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor)
{
    uint32_t i, b, j;
    ST7789_SELECT();
    st7789_set_address_window(x, y, x + font.width - 1, y + font.height - 1);

    for (i = 0; i < font.height; i++) {
        b = font.data[(ch - 32) * font.height + i];
        for (j = 0; j < font.width; j++) {
            if ((b << j) & 0x8000) {
                uint8_t data[] = {color >> 8, color & 0xFF};
                st7789_write_data(data, sizeof(data));
            } else {
                uint8_t data[] = {bgcolor >> 8, bgcolor & 0xFF};
                st7789_write_data(data, sizeof(data));
            }
        }
    }
    ST7789_UNSELECT();
}

/**
 * @brief Write a string
 * @param  x&y -> cursor of the start point.
 * @param str -> string to write
 * @param font -> fontstyle of the string
 * @param color -> color of the string
 * @param bgcolor -> background color of the string
 * @return  none
 */
void st7789_write_string(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor)
{
    ST7789_SELECT();
    while (*str) {
        if (x + font.width >= ST7789_WIDTH) {
            x = 0;
            y += font.height;
            if (y + font.height >= ST7789_HEIGHT) {
                break;
            }

            if (*str == ' ') {
                // skip spaces in the beginning of the new line
                str++;
                continue;
            }
        }
        st7789_write_char(x, y, *str, font, color, bgcolor);
        x += font.width;
        str++;
    }
    ST7789_UNSELECT();
}

/**
 * @brief Draw a filled Rectangle with single color
 * @param  x&y -> coordinates of the starting point
 * @param w&h -> width & height of the Rectangle
 * @param color -> color of the Rectangle
 * @return  none
 */
void st7789_draw_rectangle_filled(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    ST7789_SELECT();
    uint8_t i;

    /* Check input parameters */
    if (x >= ST7789_WIDTH ||
        y >= ST7789_HEIGHT) {
        /* Return error */
        return;
    }

    /* Check width and height */
    if ((x + w) >= ST7789_WIDTH) {
        w = ST7789_WIDTH - x;
    }
    if ((y + h) >= ST7789_HEIGHT) {
        h = ST7789_HEIGHT - y;
    }

    /* Draw lines */
    for (i = 0; i <= h; i++) {
        /* Draw lines */
        st7789_draw_line(x, y + i, x + w, y + i, color);
    }
    ST7789_UNSELECT();
}

/**
 * @brief Draw a Triangle with single color
 * @param  xi&yi -> 3 coordinates of 3 top points.
 * @param color ->color of the lines
 * @return  none
 */
void st7789_draw_triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color)
{
    ST7789_SELECT();
    /* Draw lines */
    st7789_draw_line(x1, y1, x2, y2, color);
    st7789_draw_line(x2, y2, x3, y3, color);
    st7789_draw_line(x3, y3, x1, y1, color);
    ST7789_UNSELECT();
}

/**
 * @brief Draw a filled Triangle with single color
 * @param  xi&yi -> 3 coordinates of 3 top points.
 * @param color ->color of the triangle
 * @return  none
 */
void st7789_draw_triangle_filled(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color)
{
    ST7789_SELECT();
    int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
            yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
            curpixel = 0;

    deltax = ABS(x2 - x1);
    deltay = ABS(y2 - y1);
    x      = x1;
    y      = y1;

    if (x2 >= x1) {
        xinc1 = 1;
        xinc2 = 1;
    } else {
        xinc1 = -1;
        xinc2 = -1;
    }

    if (y2 >= y1) {
        yinc1 = 1;
        yinc2 = 1;
    } else {
        yinc1 = -1;
        yinc2 = -1;
    }

    if (deltax >= deltay) {
        xinc1     = 0;
        yinc2     = 0;
        den       = deltax;
        num       = deltax / 2;
        numadd    = deltay;
        numpixels = deltax;
    } else {
        xinc2     = 0;
        yinc1     = 0;
        den       = deltay;
        num       = deltay / 2;
        numadd    = deltax;
        numpixels = deltay;
    }

    for (curpixel = 0; curpixel <= numpixels; curpixel++) {
        st7789_draw_line(x, y, x3, y3, color);

        num += numadd;
        if (num >= den) {
            num -= den;
            x += xinc1;
            y += yinc1;
        }
        x += xinc2;
        y += yinc2;
    }
    ST7789_UNSELECT();
}

/**
 * @brief Draw a Filled circle with single color
 * @param x0&y0 -> coordinate of circle center
 * @param r -> radius of circle
 * @param color -> color of circle
 * @return  none
 */
void st7789_draw_circle_filled(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
    ST7789_SELECT();
    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;

    st7789_draw_pixel(x0, y0 + r, color);
    st7789_draw_pixel(x0, y0 - r, color);
    st7789_draw_pixel(x0 + r, y0, color);
    st7789_draw_pixel(x0 - r, y0, color);
    st7789_draw_line(x0 - r, y0, x0 + r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        st7789_draw_line(x0 - x, y0 + y, x0 + x, y0 + y, color);
        st7789_draw_line(x0 + x, y0 - y, x0 - x, y0 - y, color);

        st7789_draw_line(x0 + y, y0 + x, x0 - y, y0 + x, color);
        st7789_draw_line(x0 + y, y0 - x, x0 - y, y0 - x, color);
    }
    ST7789_UNSELECT();
}

/**
 * @brief Open/Close tearing effect line
 * @param tear -> Whether to tear
 * @return none
 */
void st7789_tear_effect(uint8_t tear)
{
    ST7789_SELECT();
    st7789_write_command(tear ? 0x35 /* TEON */ : 0x34 /* TEOFF */);
    ST7789_UNSELECT();
}

/**
 * @brief A Simple test function for ST7789
 * @param  none
 * @return  none
 */
void st7789_test(void)
{
    st7789_fill_window(WHITE);
    ST7789_DELAY_MS(1000);
    st7789_write_string(10, 20, "Speed Test", Font_11x18, RED, WHITE);
    ST7789_DELAY_MS(1000);
    st7789_fill_window(CYAN);
    ST7789_DELAY_MS(500);
    st7789_fill_window(RED);
    ST7789_DELAY_MS(500);
    st7789_fill_window(BLUE);
    ST7789_DELAY_MS(500);
    st7789_fill_window(GREEN);
    ST7789_DELAY_MS(500);
    st7789_fill_window(YELLOW);
    ST7789_DELAY_MS(500);
    st7789_fill_window(BROWN);
    ST7789_DELAY_MS(500);
    st7789_fill_window(DARKBLUE);
    ST7789_DELAY_MS(500);
    st7789_fill_window(MAGENTA);
    ST7789_DELAY_MS(500);
    st7789_fill_window(LIGHTGREEN);
    ST7789_DELAY_MS(500);
    st7789_fill_window(LGRAY);
    ST7789_DELAY_MS(500);
    st7789_fill_window(LBBLUE);
    ST7789_DELAY_MS(500);
    st7789_fill_window(WHITE);
    ST7789_DELAY_MS(500);

    st7789_write_string(10, 10, "Font test.", Font_16x26, GBLUE, WHITE);
    st7789_write_string(10, 50, "Hello Steve!", Font_7x10, RED, WHITE);
    st7789_write_string(10, 75, "Hello Steve!", Font_11x18, YELLOW, WHITE);
    st7789_write_string(10, 100, "Hello Steve!", Font_16x26, MAGENTA, WHITE);
    ST7789_DELAY_MS(1000);

    st7789_fill_window(RED);
    st7789_write_string(10, 10, "Rect./Line.", Font_11x18, YELLOW, BLACK);
    st7789_draw_rectangle(30, 30, 100, 100, WHITE);
    ST7789_DELAY_MS(1000);

    st7789_fill_window(RED);
    st7789_write_string(10, 10, "Filled Rect.", Font_11x18, YELLOW, BLACK);
    st7789_draw_rectangle_filled(30, 30, 50, 50, WHITE);
    ST7789_DELAY_MS(1000);

    st7789_fill_window(RED);
    st7789_write_string(10, 10, "Circle.", Font_11x18, YELLOW, BLACK);
    st7789_draw_circle(60, 60, 25, WHITE);
    ST7789_DELAY_MS(1000);

    st7789_fill_window(RED);
    st7789_write_string(10, 10, "Filled Cir.", Font_11x18, YELLOW, BLACK);
    st7789_draw_circle_filled(60, 60, 25, WHITE);
    ST7789_DELAY_MS(1000);

    st7789_fill_window(RED);
    st7789_write_string(10, 10, "Triangle", Font_11x18, YELLOW, BLACK);
    st7789_draw_triangle(30, 30, 30, 70, 60, 40, WHITE);
    ST7789_DELAY_MS(1000);

    st7789_fill_window(RED);
    st7789_write_string(10, 10, "Filled Tri", Font_11x18, YELLOW, BLACK);
    st7789_draw_triangle_filled(30, 30, 30, 70, 60, 40, WHITE);
    ST7789_DELAY_MS(1000);

    //	If FLASH cannot storage anymore datas, please delete codes below.
    st7789_fill_window(WHITE);
    st7789_draw_image(0, 0, 128, 128, (uint16_t*)saber);
    ST7789_DELAY_MS(3000);
}
