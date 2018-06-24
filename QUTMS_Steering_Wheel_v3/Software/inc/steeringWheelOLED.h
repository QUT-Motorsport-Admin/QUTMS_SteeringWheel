/*****************************************************************************
* @file    Software/inc/steeringWheelOLED.h 
* @author  Zoe Goodward
* @version V1.0.0
* @date    28-April-2018
* @brief   This file declares the variables and functions that are used to 
*          interface with the OLED screen
*****************************************************************************/

#ifndef STEERINGWHEELOLED_H_
#define STEERINGWHEELOLED_H_

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>


 // Dimensions of the OLED Screen
#define OLED_X     256
#define OLED_Y     64


#define SLEEP_MODE_ON 0x01
#define SLEEP_MODE_OFF 0x00


 // LCD Command and Data

 // Display Modes
 // display mode function will shift 0xA4 by these bits to set to the different modes
#define DISPLAY_ON     0x01    //0xA4
#define DISPLAY_OFF    0x00    //0xA5
#define NORMAL_MODE    0x02    //0xA6
#define INVERSE_MODE   0x03    //0xA7

// Functions for interfacing with the OLED screen
void oled_init();

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Global Variables
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#define	Shift		0x1C
#define Max_Column	0x3F			// 256/4-1
#define Max_Row		0x3F			// 64-1
#define	Brightness	0x0F

#define	SPI				// 4-wire SPI
//   BS0=0; BS1=0
//   The unused pins should be connected with VSS mostly or floating (D2).
//   Please refer to the SSD1322 specification for detail.
#define CS_high PORTD |= 1
#define CS_low PORTD &= ~1
#define RESET_high	PORTB |= 64
#define RESET_low PORTB &= ~64
#define DC_data PORTB |= 32
#define DC_instruction PORTB &= ~32
#define SPI_clock_high PORTB |= 128
#define SPI_clock_low PORTB &= ~128
#define SPI_data_high PORTB |= 2
#define SPI_data_low PORTB &= ~2

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Read/Write Sequence
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void write_command(unsigned char Data);

void write_data(unsigned char Data);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Instruction Setting
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void set_column_address(unsigned char a, unsigned char b);

void set_row_address(unsigned char a, unsigned char b);

void set_write_ram();

void set_read_ram();

void set_remap_format(unsigned char d);

void set_start_line(unsigned char d);

void set_display_offset(unsigned char d);

void set_display_mode(unsigned char d);

void set_partial_display(unsigned char a, unsigned char b, unsigned char c);

void set_function_selection(unsigned char d);

void set_display_on_off(unsigned char d);

void set_phase_length(unsigned char d);

void set_display_clock(unsigned char d);

void set_display_enhancement_a(unsigned char a, unsigned char b);

void set_gpio(unsigned char d);

void set_precharge_period(unsigned char d);

void set_precharge_voltage(unsigned char d);

void set_vcomh(unsigned char d);

void set_contrast_current(unsigned char d);

void set_master_current(unsigned char d);

void set_multiplex_ratio(unsigned char d);

void set_display_enhancement_b(unsigned char d);

void set_command_lock(unsigned char d);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Regular Pattern (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void fill_ram(unsigned char Data);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  DATA PROCESSING
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void data_processing(unsigned char temp);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Sleep Mode
//
//    "0x01" Enter Sleep Mode
//    "0x00" Exit Sleep Mode
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void sleep(unsigned char a);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Connection Test
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void test();

void delay(unsigned int n);
void udelay(unsigned int l);

#endif /* STEERINGWHEELOLED_H_ */