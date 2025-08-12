#ifndef __LCD_SPI_H__
#define __LCD_SPI_H__

#include "board.h"

void lcd_spi_init(void);
void lcd_spi_deinit(void);
void lcd_spi_send(uint8_t* data, uint16_t len);

#endif /* __LCD_SPI_H__ */