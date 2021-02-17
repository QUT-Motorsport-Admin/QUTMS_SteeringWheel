/*
 * lcd_spi_spi.c
 *
 *  Created on: 17 Feb 2021
 *      Author: Calvin J
 */

#include "main.h"
#include "lcd_spi.h"

extern SPI_HandleTypeDef hspi1;

void lcd_spi_mode_cmd();
void lcd_spi_mode_data();
void lcd_spi_cs_on();
void lcd_spi_cs_off();
void lcd_spi_rs_on();
void lcd_spi_rs_off();
void lcd_spi_mode_8bit();
void lcd_spi_mode_16bit();

void lcd_spi_delay(uint32_t delay) {
	HAL_Delay(delay);
}

void lcd_spi_mode_cmd() {
	HAL_GPIO_WritePin(SCREEN_D_C_GPIO_Port, SCREEN_D_C_Pin, GPIO_PIN_RESET);
}

void lcd_spi_mode_data() {
	HAL_GPIO_WritePin(SCREEN_D_C_GPIO_Port, SCREEN_D_C_Pin, GPIO_PIN_SET);
}

void lcd_spi_cs_on() {
	HAL_GPIO_WritePin(SCREEN_CS_GPIO_Port, SCREEN_CS_Pin, GPIO_PIN_RESET);
}

void lcd_spi_cs_off() {
	HAL_GPIO_WritePin(SCREEN_CS_GPIO_Port, SCREEN_CS_Pin, GPIO_PIN_SET);
}

void lcd_spi_rs_on() {
	HAL_GPIO_WritePin(SCREEN_RS_GPIO_Port, SCREEN_RS_Pin, GPIO_PIN_RESET);

}

void lcd_spi_rs_off() {
	HAL_GPIO_WritePin(SCREEN_RS_GPIO_Port, SCREEN_RS_Pin, GPIO_PIN_SET);
}

void lcd_spi_backlight_on() {
	HAL_GPIO_WritePin(SCREEN_BL_GPIO_Port, SCREEN_BL_Pin, GPIO_PIN_SET);
}

void lcd_spi_backlight_off() {
	HAL_GPIO_WritePin(SCREEN_BL_GPIO_Port, SCREEN_BL_Pin, GPIO_PIN_RESET);
}

void lcd_spi_mode_8bit() {
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Instance->CR1 &= ~SPI_CR2_DS_Msk;
	hspi1.Instance->CR1 |= (SPI_DATASIZE_8BIT & SPI_CR2_DS_Msk);
}

void lcd_spi_mode_16bit() {
	hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
	hspi1.Instance->CR1 &= ~SPI_CR2_DS_Msk;
	hspi1.Instance->CR1 |= (SPI_DATASIZE_16BIT & SPI_CR2_DS_Msk);
}

void lcd_spi_write_8(uint8_t data) {
	HAL_SPI_Transmit(&hspi1, &data, 1, HAL_MAX_DELAY);
}

void lcd_spi_write_8_multiple(uint8_t *data, uint32_t length) {
	HAL_SPI_Transmit(&hspi1, data, length, HAL_MAX_DELAY);
}

void lcd_spi_write_cmd8(uint8_t cmd) {
	lcd_spi_mode_8bit();
	lcd_spi_cs_on();

	lcd_spi_mode_cmd();
	lcd_spi_write_8(cmd);
	lcd_spi_mode_data();

	lcd_spi_cs_off();
}

void lcd_spi_write_cmd8_data8(uint8_t cmd, uint8_t *data, uint32_t length) {
	lcd_spi_mode_8bit();
	lcd_spi_cs_on();

	lcd_spi_mode_cmd();
	lcd_spi_write_8(cmd);

	lcd_spi_mode_data();
	lcd_spi_write_8_multiple(data, length);

	lcd_spi_cs_off();
}

void lcd_spi_write_data8(uint8_t data) {
	lcd_spi_mode_8bit();
	lcd_spi_cs_on();

	lcd_spi_write_8(data);

	lcd_spi_cs_off();
}

void lcd_spi_write_data8_multiple(uint8_t *data, uint32_t length) {
	lcd_spi_mode_8bit();
	lcd_spi_cs_on();

	lcd_spi_write_8_multiple(data, length);

	lcd_spi_cs_off();
}

void lcd_spi_init() {
	lcd_spi_mode_data();
	lcd_spi_cs_off();

	lcd_spi_delay(10);
	lcd_spi_rs_on();
	lcd_spi_delay(10);
	lcd_spi_rs_off();
	lcd_spi_delay(10);
}

