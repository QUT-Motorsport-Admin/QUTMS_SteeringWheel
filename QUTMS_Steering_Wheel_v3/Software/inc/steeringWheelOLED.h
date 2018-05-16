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

void Write_Data(unsigned char Data);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Instruction Setting
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Set_Column_Address(unsigned char a, unsigned char b);

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Graphic Acceleration (Partial or Full Screen)
//
//    a: Line Width
//    b: Column Address of Start
//    c: Column Address of End
//    d: Row Address of Start
//    e: Row Address of End
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Draw_Rectangle(unsigned char Data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Regular Pattern (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void fill_ram(unsigned char Data);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  DATA PROCESSING
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Data_processing(unsigned char temp);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Sleep Mode
//
//    "0x01" Enter Sleep Mode
//    "0x00" Exit Sleep Mode
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Sleep(unsigned char a);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Connection Test
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Test();

#endif /* STEERINGWHEELOLED_H_ */