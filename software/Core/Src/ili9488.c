/*
 * ili9488.c
 *
 *  Created on: Feb 17, 2021
 *      Author: Calvin
 */

#include "ili9488.h"
#include "lcd_spi.h"


void ili9488_init() {
	lcd_spi_init();

	lcd_spi_delay(105);

	lcd_spi_write_cmd8(ILI9488_SWRESET);
	lcd_spi_delay(5);

	// positive gamma control
	lcd_spi_write_cmd8_data8(ILI9488_GMCTRP1, (uint8_t*) "\x00\x03\x09\x08\x16\x0A\x3F\x78\x4C\x09\x0A\x08\x16\x1A\x0F", 15);

	// negative gamma control
	lcd_spi_write_cmd8_data8(ILI9488_GMCTRN1, (uint8_t*) "\x00\x16\x19\x03\x0F\x05\x32\x45\x46\x04\x0E\x0D\x35\x37\x0F", 15);

	// Power Control 1 (Vreg1out, Verg2out)
	lcd_spi_write_cmd8_data8(ILI9488_PWCTR1, (uint8_t*) "\x17\x15", 2);
	lcd_spi_delay(5);

	// Power Control 2 (VGH,VGL)
	lcd_spi_write_cmd8(ILI9488_PWCTR2);
	lcd_spi_write_data8(0x41);
	lcd_spi_delay(5);

	// Power Control 3 (Vcom)
	lcd_spi_write_cmd8_data8(ILI9488_VMCTR1, (uint8_t*) "\x00\x12\x80", 3);
	lcd_spi_delay(5);

	// pixel format
	lcd_spi_write_cmd8(ILI9488_PIXFMT);
	lcd_spi_write_data8(ILI9488_PIXFMT_16BIT);

	// interface mode control
	lcd_spi_write_cmd8(ILI9488_IMCTR);
	lcd_spi_write_data8(0x00);

	// frame rate
	lcd_spi_write_cmd8(ILI9488_FRMCTR1);
	lcd_spi_write_data8(0xA0); // 60hz

	// display inversion control
	lcd_spi_write_cmd8(ILI9488_INVCTR);
	lcd_spi_write_data8(0x02);

	// display function control rgb/mcu interface control
	lcd_spi_write_cmd8_data8(ILI9488_DFUNCTR, (uint8_t*) "\x02\x02", 2);

	// set image function (disable 24bit data)
	lcd_spi_write_cmd8(ILI9488_IMGFUNCT);
	lcd_spi_write_data8(0x00);

	// adjust control - d7 stream, loose
	lcd_spi_write_cmd8_data8(ILI9488_ADJCTR3, (uint8_t*) "\xA9\x51\x2C\x82", 4);

	lcd_spi_delay(5);

	// exit sleep
	lcd_spi_write_cmd8(ILI9488_SLPOUT);
	lcd_spi_delay(120);

	// display on
	lcd_spi_write_cmd8(ILI9488_DISPON);
	lcd_spi_delay(5);

	lcd_spi_write_cmd8(ILI9488_MADCTL);
	lcd_spi_write_data8(ILI9488_MAD_DATA_RIGHT_THEN_DOWN);
}

void ili9488_display_on() {
	lcd_spi_write_cmd8(ILI9488_SLPOUT);
	lcd_spi_backlight_on();
}

void ili9488_display_off() {
	lcd_spi_write_cmd8(ILI9488_SLPIN);
	lcd_spi_backlight_off();
}

void ili9488_set_display_window(uint16_t x_pos, uint16_t y_pos, uint16_t width, uint16_t height) {
	uint16_t x_start = x_pos;
	uint16_t x_end = x_pos + width - 1;

	uint16_t y_start = y_pos;
	uint16_t y_end = y_pos + height - 1;


	lcd_spi_write_cmd8(ILI9488_ADR_COL);
	// col start
	lcd_spi_write_data8(x_start >> 8);
	lcd_spi_write_data8(x_start);
	// col end
	lcd_spi_write_data8(x_end >> 8);
	lcd_spi_write_data8(x_end);


	lcd_spi_write_cmd8(ILI9488_ADR_ROW);
	// row start
	lcd_spi_write_data8(y_start >> 8);
	lcd_spi_write_data8(y_start);
	// row end
	lcd_spi_write_data8(y_end >> 8);
	lcd_spi_write_data8(y_end);
}

void ili9488_draw_data(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t *data) {
	ili9488_set_display_window(x,y,width, height);
	lcd_spi_write_cmd8(ILI9488_RAMWR);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			lcd_spi_write_data8_multiple((uint8_t*)data, 2);
			data++;
		}
	}
}
