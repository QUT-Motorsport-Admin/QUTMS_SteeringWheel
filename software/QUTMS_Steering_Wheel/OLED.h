#ifndef OLED_H_
#define OLED_H_

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

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
void Write_Command(unsigned char Data);

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
void Fill_RAM(unsigned char Data);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Regular Pattern (Partial or Full Screen)
//
//    a: Column Address of Start
//    b: Column Address of End (Total Columns Devided by 4)
//    c: Row Address of Start
//    d: Row Address of End
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fill_Block(unsigned char Data, unsigned char a, unsigned char b, unsigned char c, unsigned char d);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Checkboard (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Checkerboard();

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Gray Scale Bar (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Grayscale();

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Character (5x7)
//
//    a: Database
//    b: Ascii
//    c: Start X Address
//    d: Start Y Address
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_Font57(unsigned char a, unsigned char b, unsigned char c, unsigned char d);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Character
//
//    a: Database
//    b: Start X Address
//    c: Start Y Address
//    * Must write "0" in the end...
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_Char(unsigned char a, unsigned char Data, unsigned char b, unsigned char c);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show String
//
//    a: Database
//    b: Start X Address
//    c: Start Y Address
//    * Must write "0" in the end...
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_String(unsigned char a, unsigned char *Data_Pointer, unsigned char b, unsigned char c);

void Show_Huge_String(unsigned char *Data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned int length,  unsigned char LwrNibbleBrightness, unsigned char UppNibbleBrightness);

void Show_Bigger_String(unsigned char *Data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned int length,  unsigned char LwrNibbleBrightness, unsigned char UppNibbleBrightness);

void Show_Big_String(unsigned char *Data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned int length, unsigned char LwrNibbleBrightness, unsigned char UppNibbleBrightness);

void Show_Huge_Char(unsigned char data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char LwrNibbleBrightness, unsigned char UppNibbleBrightness);

void Show_Bigger_Char(unsigned char data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char LwrNibbleBrightness, unsigned char UppNibbleBrightness);

void Show_Big_Char(unsigned char data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char LwrNibbleBrightness, unsigned char UppNibbleBrightness);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Pattern (Partial or Full Screen)
//
//    a: Column Address of Start
//    b: Column Address of End (Total Columns Devided by 4)
//    c: Row Address of Start
//    d: Row Address of End
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_Pattern(unsigned char *Data_Pointer, unsigned char a, unsigned char b, unsigned char c, unsigned char d);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  DATA PROCESSING
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Data_processing(unsigned char temp);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  DISPLAY PICTURE
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Display_Picture(unsigned char pic[]);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Vertical Scrolling (Full Screen)
//
//    a: Scrolling Direction
//       "0x00" (Upward)
//       "0x01" (Downward)
//    b: Set Numbers of Row Scroll per Step
//    c: Set Time Interval between Each Scroll Step
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Vertical_Scroll(unsigned char a, unsigned char b, unsigned char c);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Vertical Fade Scrolling (Full Screen)
//
//    a: Scrolling Direction
//       "0x00" (Upward - In)
//       "0x01" (Downward - In)
//       "0x02" (Upward - Out)
//       "0x03" (Downward - Out)
//    b: Set Numbers of Row Scroll per Step
//    c: Set Time Interval between Each Scroll Step
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fade_Scroll(unsigned char a, unsigned char b, unsigned char c);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Fade In (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fade_In();

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Fade Out (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fade_Out();

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

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Gray Scale Table Setting (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Set_Gray_Scale_Table();

void Set_Linear_Gray_Scale_Table();

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Initialization
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void OLED_Init();

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Example Function Usage
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

/*
// All Pixels On (Test Pattern)
Fill_RAM(0xFF);
//Delay(500);

//Checkerboard (Test Pattern)
//Checkerboard();
//Delay(500);

// Gray Scale Bar (Test Pattern)
Grayscale();
//Delay(500);
Fill_RAM(0x00);			// Clear Screen

// Show Pattern - Frame (Test Pattern)
Draw_Rectangle(0xFF,0x01,0x00,Max_Column,0x00,Max_Row);
//Delay(500);

// Scrolling (Full Screen)
Vertical_Scroll(0x00,0x01,0x40);
// Upward
Delay(1000);
Vertical_Scroll(0x01,0x01,0x40);

// Downward
Set_Display_Offset(0x00);
Fade_Scroll(0x03,0x01,0x40);	// Downward - Out
Delay(1000);
Fade_Scroll(0x00,0x01,0x40);	// Upward - In
Fade_Scroll(0x02,0x01,0x40);	// Upward - Out
Delay(1000);
Fade_Scroll(0x01,0x01,0x40);	// Downward - In
Set_Start_Line(0x00);
Delay(1000);

// All Pixels On (Test Pattern)
Fill_RAM(0xFF);
Delay(1000);

// Checkerboard (Test Pattern)
Checkerboard();
Delay(1000);

// Gray Scale Bar (Test Pattern)
Grayscale();
Delay(1000);
Fill_RAM(0x00);			// Clear Screen

// Show Pattern - Frame (Test Pattern)
Draw_Rectangle(0xFF,0x01,0x00,Max_Column,0x00,Max_Row);
Delay(1000);
*/

#endif
