/*
 * lcd_spi.h
 *
 *  Created on: Feb 18, 2021
 *      Author: Calvin Johnson
 */

#ifndef INC_LCD_SPI_H_
#define INC_LCD_SPI_H_

void lcd_spi_delay(uint32_t delay);

inline void lcd_spi_backlight_on();
inline void lcd_spi_backlight_off();

void lcd_spi_write_8(uint8_t data);
void lcd_spi_write_8_multiple(uint8_t *data, uint32_t length);

void lcd_spi_write_cmd8(uint8_t cmd);
void lcd_spi_write_cmd8_data8(uint8_t cmd, uint8_t *data, uint32_t length);

void lcd_spi_write_data8(uint8_t data);
void lcd_spi_write_data8_multiple(uint8_t *data, uint32_t length);

void lcd_spi_init();


#endif /* INC_LCD_SPI_H_ */
