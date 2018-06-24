/*****************************************************************************
* @file    Software/src/steeringWheelFunctions.c
* @author  Zoe Goodward
* @version V1.0.0
* @date    28-April-2018
* @brief   Functions for general opperation of steering wheel
*****************************************************************************/

//External libraries
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <math.h>

#include "steeringWheelFunctions.h"
#include "steeringWheelCAN.h"
#include "steeringWheelADC.h"
#include "steeringWheelSPI.h"
#include "steeringWheelBitOpr.h"
#include "steeringWheelGraphics.h"
#include "steeringWheelInput.h"
#include "steeringWheelOLED.h"
#include "steeringWheelBitmaps.h"

// Global variables
uint16_t ThrottlePercentageData = 0;
uint32_t RPMData = 0;
uint32_t GearboxTempData = 0;
uint32_t VoltageData = 0;
uint8_t CAN_Dial = 0;

// If using show_string, append 0x00 as an extra character on end of the string, eg: unsigned char Acceleration[]= {'A','C','C','E','L','E','R','A','T','I','O','N',0x00};
unsigned char Acceleration[] = {'A','C','C','E','L','E','R','A','T','I','O','N',0x00};
unsigned char SkidPad[]= {'S','K','I','D','P','A','D'};
unsigned char AutoCross[]= {'A','U','T','O','C','R','O','S','S'};
unsigned char Endurance[]= {'E','N','D','U','R','A','N','C','E'};
unsigned char PartyParrots[]= {'P','A','R','T','Y',' ','M','O','D','E'};
	
unsigned char SelectionRight[]= {'>','>','>','>'};
unsigned char SelectionLeft[]= {'<','<','<','<'};
unsigned char Voltage[]= {'V','O','L','T','A','G','E',' ',':'};
unsigned char RPM[] = {'R','P','M',' ',':'};
unsigned char GearboxTemp[] = {'G','E','A','R','B','O','X',' ','T','E','M','P',' ',':'};
unsigned char Throttle[]= {'T','H','R','O','T','T','L','E',' ','%',' ',':'};
unsigned char BlankNumber[] = {' ',' ',' ',' ',' '};
unsigned char LeftArrow[]= {'<'};
unsigned char RightArrow[]= {'>'};
unsigned char TempBuffer[10];
unsigned int RPMMaxFlag = 0;
unsigned int ThrottleMaxFlag = 0;
unsigned int ThrottleGreaterThan10 = 0;
unsigned int RPMGreaterThan100 = 0;
unsigned int RPMGreaterThan1000 = 0;
unsigned int GearboxTempCount = 0;
unsigned int GearboxTempMaxFlag = 0;
unsigned int RPMCounter = 0;
unsigned int BootFlag = 0;
int ScreenFlag = 0;
int ADC_Change = 1;
int ADC_Previous = 0;
int digitLength = 0;
uint16_t LeftDialADC;
uint16_t LeftDialADCOld = 0;
int LeftDialADCScaled;
uint16_t RightDialADC;
uint16_t RightDialADCOld = 0;
int RightDialADCScaled;
uint8_t mob;

/*============================================================================
Function:   steering_wheel_init()
------------------------------------------------------------------------------
Purpose :   consolidates all the functions required for the initialisation of
			the steering wheel
Input   :   none
Returns :   void
Notes   :
============================================================================*/
void steering_wheel_init() 
{
	DDRB = 0b11100011;	//b7 - SCK; b1 - MOSI
	//DDRC &= ~(1 << PINC0);
	DDRD = 0b00001001;	//b3 - SS pin
	SPI_clock_high;

	can_init();
	can_rx_init(5,8,0x400000, 0x400000);
	adc_init();
	spi_init();
	button_init();
	oled_init();
	sei(); /* enable interrupts */	
	splash_screen();
}

/*============================================================================
Function:   splash_screen()
------------------------------------------------------------------------------
Purpose :   displays the QUT Motorsport logo when the steering wheel system
			is first initialised / alternativly can display party parrot
Input   :   none
Returns :   void
Notes   :
============================================================================*/
void splash_screen()
{
	fill_ram(CLEAR_SCREEN);
	party_mode();
	//display_picture(MOTORSPORT);
	delay(3000);
	fill_ram(CLEAR_SCREEN);
	//fade out?
}

/*============================================================================
Function:   party_mode()
------------------------------------------------------------------------------
Purpose :   cycles through bitmap images to create party parrot animation
Input   :   none
Returns :   void
Notes   :	never been tested
============================================================================*/
void party_mode() {
	fill_ram(CLEAR_SCREEN);
	for ( int i = 0; i < N_PARROT; i++ ) {
		char parrotFrame[N_PARROT+1];
		sprintf(parrotFrame, "PARROT_00%d", i);
		display_picture(parrotFrame);
		delay(100); /* temp value, will test later */
		//fill_ram(CLEAR_SCREEN); /* may be necessary in between each frame? */
		if ( i == (N_PARROT - 1) ) {
			i = 0;
		}
	}
}

/*============================================================================
Function:   emergency()
------------------------------------------------------------------------------
Purpose :   displays a warning message to the driver when an error with the 
			car's system occurs
			Emergency situations: 
Input   :   none
Returns :   void
Notes   :	never been tested
============================================================================*/
void emergency() {
	//
}


/// SUDO CODE - NEEDS FULL IMPLEMENTATION ///	
int RIGHT = 0;
int LEFT = 1;	
void endurance() {
	// clear screen at start
	fill_ram(CLEAR_SCREEN);

	if(BIT_IS_SET(PINC, RIGHT) && BIT_IS_SET(PINC, LEFT)) {
		main_menu();
	}

	int page = 0;

	if(BIT_IS_SET(PINC, RIGHT)) {
		page += 1;
		if(page > 5) {
			//page == 0;
		}
	}
	if(BIT_IS_SET(PINC, LEFT)) {
		page -= 1;
	}

	switch(page) {
		case 1 :
			//display_speed();
			break;
		case 2 :
			//display_battery();
			break;
		case 3 :
			//display_lap_time();
			break;		
	}
}
/*
void display_speed() {
	itoa()
	//itoa(GearboxTempData,TempBuffer,10);
	//digitLength = (GearboxTempData == 0 ? 1 : (int)(log10(GearboxTempData)+1));
	//show_string(1,TempBuffer,0x28,0x05);
	//show_big_string(&GearboxTemp,0x05,0x64,0x16,0x7F, 14, 0x0F, 0xF0);
	//show_big_string(&TempBuffer,0x30,0x64,0x16,0x7F, digitLength, 0x0F, 0xF0);
}
void display_battery() {
	//
}
void dashboard_display(uint8_t race_mode) {
	if (race_mode == 1) {
		// endurance
	}
}
void display_lap_time() {}
*/
void acceleration() {
	// do stuff
}

void skidpad() {
	// do stuff
}
void auto_cross() {
	// do stuff
}

void main_menu() {
	// clear screen at start
	fill_ram(CLEAR_SCREEN);
	// display race modes as a selection
	//vertical_scroll(UPWARD, 32/*half screen*/, ); maybe?
	show_string(1, &Endurance, 10, 10);
	show_string(1, &Acceleration, 20, 10);
	show_string(1, &SkidPad, 30, 10);
	show_string(1, &AutoCross, 40, 10);
	show_string(1, &PartyParrots, 50, 10);
	
	// diaplay one at a time -- cycle back around or just stop
	// resistors inbetween pins give unique voltage that is then converted by ADC to binary code
	// if left_dial == pin number invert the colour to display selection
	// if right button selected confirm selection and move to mode
	uint16_t left_dial = adc_read(LEFT_DIAL);

	switch(left_dial) {
		case 1 :
			// invert endurance
			if(BIT_IS_SET(PINC, RIGHT)) {
				endurance();
			}
			break;
		case 2 :
			// invert acceleration
			if(BIT_IS_SET(PINC, RIGHT)) {
				acceleration();
			}
			break;
		case 3 :
			// invert skidpad
			if(BIT_IS_SET(PINC, RIGHT)) {
				skidpad();
			}
			break;
		case 4 :
			if(BIT_IS_SET(PINC, RIGHT)) {
				auto_cross();
			}
		case 5 :
			// invert party mode
			if(BIT_IS_SET(PINC, RIGHT)) {
				party_mode();
			}
			break;
	}

}
void sudo_update_screen() {
	main_menu();
}

// old
void update_screen() {
	read_dials();
	LeftDialADC = adc_read(LEFT_DIAL);
	LeftDialADCScaled = (int)(LeftDialADC >> 2) / 24; // Scale values from 1 to 8 - need to verify with new PCB since dials are playing up
	RightDialADC = adc_read(RIGHT_DIAL);
	RightDialADCScaled = (int)(RightDialADC >> 2) / 24; // Scale values from 1 to 8 - need to verify with new PCB since dials are playing up
	mob = CAN_findFreeTXMOB();
	//if(mob > 0)
	can_tx_mob(0, 1, &CAN_Dial, 0x400001,0);
	_delay_ms(100);
	
	if((LeftDialADCOld != LeftDialADCScaled) /*|| (RightDialADCOld != RightDialADCScaled)*/)
	{
		ADC_Change = 1;
	}
	
	if(LeftDialADCScaled == 3)
	{					
		if(ADC_Change == 1)
		{
			ADC_Change = 0;
			if(LeftDialADCScaled > LeftDialADCOld){
				show_bigger_string(&RightArrow,0x3A,0x60,0x11,0x7F, 1, 0x0F, 0xF0);
			}
			if(LeftDialADCScaled < LeftDialADCOld){
				show_bigger_string(&LeftArrow,0x00,0x15,0x11,0x7F, 1, 0x0F, 0xF0);
			}
			LeftDialADCOld = LeftDialADCScaled;
			delay(500);
			fill_ram(CLEAR_SCREEN);
		}
		
		if(ThrottleMaxFlag == 1){
			//show_string(1,&BlankNumber,0x28,0x05);
			ThrottleMaxFlag = 0;
		}
		if(ThrottlePercentageData == 100){ ThrottleMaxFlag = 1;	}
		// If throttle is at 10%, when the throttle goes down, a trailing 0 will be left - this needs to be cleared
		if(ThrottlePercentageData < 10){ ThrottleGreaterThan10 = 1;	}
		if(ThrottleGreaterThan10 == 1){
			//show_string(1,&BlankNumber,0x28,0x05);
			ThrottleGreaterThan10 = 0;
		}
		itoa(ThrottlePercentageData,TempBuffer,10);
		digitLength = (ThrottlePercentageData == 0 ? 1 : (int)(log10(ThrottlePercentageData)+1));
		show_big_string(&Throttle,0x05,0x64,0x16,0x7F, 12, 0x0F, 0xF0);		
		show_string(1,TempBuffer,0x28,0x05);
		show_big_string(&TempBuffer,0x2D,0x64,0x16,0x7F, digitLength, 0x0F, 0xF0); // Print the current throttle percentage
		show_bigger_string(&RightArrow,0x3A,0x60,0x11,0x7F, 1, 0x04, 0x40);
	}
	
	if(LeftDialADCScaled == 4)
	{
		if(ADC_Change == 1)
		{
			ADC_Change = 0;
			if(LeftDialADCScaled > LeftDialADCOld){
				show_bigger_string(&RightArrow,0x3A,0x60,0x11,0x7F, 1, 0x0F, 0xF0);
			}
			if(LeftDialADCScaled < LeftDialADCOld){
				show_bigger_string(&LeftArrow,0x00,0x15,0x11,0x7F, 1, 0x0F, 0xF0);
			}
			LeftDialADCOld = LeftDialADCScaled;
			delay(500);
			fill_ram(CLEAR_SCREEN);
		}
		
		// If RPM is greater than 100 (or 1000), when the throttle goes down, a trailing 0 will be left
		// This needs to be cleared
		if(RPMData < 99){ RPMGreaterThan100 = 1; }
		if(RPMData < 999){ RPMGreaterThan100 = 1; }
		if(RPMGreaterThan100 == 1){
			//show_string(1,&BlankNumber,0x28,0x15);
			RPMGreaterThan100 = 0;
		}
		itoa(RPMData,TempBuffer,10);
		digitLength = (RPMData == 0 ? 1 : (int)(log10(RPMData)+1));
		show_string(1,TempBuffer,0x28,0x05);
		show_big_string(&RPM,0x05,0x64,0x16,0x7F, 5, 0x0F, 0xF0);
		show_big_string(&TempBuffer,0x2D,0x64,0x16,0x7F, digitLength, 0x0F, 0xF0);
		show_bigger_string(&LeftArrow,0x00,0x15,0x11,0x7F, 1, 0x04, 0x40);
		show_bigger_string(&RightArrow,0x3A,0x60,0x11,0x7F, 1, 0x04, 0x40);
	}
	
	if(LeftDialADCScaled == 5)
	{
		if(ADC_Change == 1)
		{
			ADC_Change = 0;
			if(LeftDialADCScaled > LeftDialADCOld){
				show_bigger_string(&RightArrow,0x3A,0x60,0x11,0x7F, 1, 0x0F, 0xF0);
			}
			if(LeftDialADCScaled < LeftDialADCOld){
				show_bigger_string(&LeftArrow,0x00,0x15,0x11,0x7F, 1, 0x0F, 0xF0);
			}
			LeftDialADCOld = LeftDialADCScaled;
			delay(500);
			fill_ram(CLEAR_SCREEN);
		}
		
		itoa(GearboxTempData,TempBuffer,10);
		digitLength = (GearboxTempData == 0 ? 1 : (int)(log10(GearboxTempData)+1));
		show_string(1,TempBuffer,0x28,0x05);
		show_big_string(&GearboxTemp,0x05,0x64,0x16,0x7F, 14, 0x0F, 0xF0);
		show_big_string(&TempBuffer,0x30,0x64,0x16,0x7F, digitLength, 0x0F, 0xF0);
		show_bigger_string(&LeftArrow,0x00,0x15,0x11,0x7F, 1, 0x04, 0x40);
		show_bigger_string(&RightArrow,0x3A,0x60,0x11,0x7F, 1, 0x04, 0x40);
	}
	
	if(LeftDialADCScaled == 6)
	{
		if(ADC_Change == 1)
		{
			ADC_Change = 0;
			if(LeftDialADCScaled > LeftDialADCOld){
				show_bigger_string(&RightArrow,0x3A,0x60,0x11,0x7F, 1, 0x0F, 0xF0);
			}
			if(LeftDialADCScaled < LeftDialADCOld){
				show_bigger_string(&LeftArrow,0x00,0x15,0x11,0x7F, 1, 0x0F, 0xF0);
			}
			LeftDialADCOld = LeftDialADCScaled;
			delay(500);
			fill_ram(CLEAR_SCREEN);
		}
		
		itoa(VoltageData/1000, TempBuffer, 10);		//display the average cell voltage in Volts instead of millivolts (find the whole volts in the value)
		TempBuffer[1] = '.';						//add the decimal point
		itoa(VoltageData-((VoltageData/1000)*1000), TempBuffer+2, 10);	//add the remaining millivolts
		show_string(1,TempBuffer,0x28,0x05);		//display the result.
		show_big_string(&Voltage,0x05,0x64,0x16,0x7F, 9, 0x0F, 0xF0);
		show_big_string(&TempBuffer,0x2D,0x64,0x16,0x7F, 4, 0x0F, 0xF0);
		show_bigger_string(&LeftArrow,0x00,0x15,0x11,0x7F, 1, 0x04, 0x40);
		show_bigger_string(&RightArrow,0x3A,0x60,0x11,0x7F, 1, 0x04, 0x40);
	}
	
	if(LeftDialADCScaled > 6)
	{
		if(ADC_Change == 1)
		{
			ADC_Change = 0;
			if(LeftDialADCScaled > LeftDialADCOld){
				show_bigger_string(&RightArrow,0x3A,0x60,0x11,0x7F, 1, 0x0F, 0xF0);
			}
			if(LeftDialADCScaled < LeftDialADCOld){
				show_bigger_string(&LeftArrow,0x00,0x15,0x11,0x7F, 1, 0x0F, 0xF0);
			}
			LeftDialADCOld = LeftDialADCScaled;
			delay(500);
			fill_ram(CLEAR_SCREEN);
		}
		show_bigger_string(&LeftArrow,0x00,0x15,0x11,0x7F, 1, 0x04, 0x40);
	}
}
	
	/*
	unsigned char c = 0x08;
	unsigned char d = 0x04;
	set_column_address(Shift+c,Shift+c);
	set_row_address(d,d+7);
	set_write_ram();
	Write_Data(0x00);
	Write_Data(0xF0);
	//Write_Data(0x77);
	//Write_Data(0xFF);
	//Write_Data(0xFF);
	
	c = 0x08;
	d = 0x08;
	set_column_address(Shift+c,Shift+c);
	set_row_address(d,d+7);
	set_write_ram();
	Write_Data(0x00);
	Write_Data(0xF0);
*/

	/*
	while(BootFlag == 0)
	{
		show_string(1,&Acceleration,0x13,0x05);
		show_string(1,SkidPad,0x17,0x15);
		show_string(1,&AutoCross,0x16,0x25);
		show_string(1,&Endurance,0x16,0x35);
		test = adc_read(2);
		test2 = (int)(test >> 2) / 24;
		
		if(test2 == 3)
		{
			show_string(1,&BlankNumber,0x05,0x25);
			show_string(1,&BlankNumber,0x31,0x25);
			show_string(1,&BlankNumber,0x05,0x15);
			show_string(1,&BlankNumber,0x31,0x15);
			show_string(1,&BlankNumber,0x05,0x35);
			show_string(1,&BlankNumber,0x31,0x35);
			show_string(1,&SelectionLeft,0x05,0x05);
			show_string(1,&SelectionRight,0x31,0x05);

		}
		if(test2 == 4)
		{
			show_string(1,&BlankNumber,0x05,0x25);
			show_string(1,&BlankNumber,0x31,0x25);
			show_string(1,&BlankNumber,0x05,0x05);
			show_string(1,&BlankNumber,0x31,0x05);
			show_string(1,&BlankNumber,0x05,0x35);
			show_string(1,&BlankNumber,0x31,0x35);
			show_string(1,&SelectionLeft,0x05,0x15);
			show_string(1,&SelectionRight,0x31,0x15);
		}
		if(test2 == 5)
		{
			show_string(1,&BlankNumber,0x05,0x05);
			show_string(1,&BlankNumber,0x31,0x05);
			show_string(1,&BlankNumber,0x05,0x15);
			show_string(1,&BlankNumber,0x31,0x15);
			show_string(1,&BlankNumber,0x05,0x35);
			show_string(1,&BlankNumber,0x31,0x35);
			show_string(1,&SelectionLeft,0x05,0x25);
			show_string(1,&SelectionRight,0x31,0x25);
		}
		if(test2 == 6)
		{
			show_string(1,&BlankNumber,0x05,0x25);
			show_string(1,&BlankNumber,0x31,0x25);
			show_string(1,&BlankNumber,0x05,0x15);
			show_string(1,&BlankNumber,0x31,0x15);
			show_string(1,&BlankNumber,0x05,0x05);
			show_string(1,&BlankNumber,0x31,0x305);
			show_string(1,&SelectionLeft,0x05,0x35);
			show_string(1,&SelectionRight,0x31,0x35);
		}
		
		// if button press break
		if(PINC & (1<<PC0) == 0)
		{
			fill_ram(0x00);			// Clear Screen
			Draw_Rectangle(0xFF,0x01,0x00,0x3F,0x00,0x10);
			Draw_Rectangle(0xFF,0x01,0x00,0x3F,0x11,0x20);
			Draw_Rectangle(0xFF,0x01,0x00,0x3F,0x21,0x30);
			Draw_Rectangle(0xFF,0x01,0x00,0x3F,0x31,0x3F);
			// send message to chassis controller regarding current dial state  
			BootFlag = 1;
		}
		
	}

*/	
	
	// if button press return back to boot menu
	//if(PINC & (1<<PC0) == 0)
	//{
	//	BootFlag = 0;
	//}
	
	
	// If throttle is at 100%, when the throttle goes down, a trailing 0 will be left
	// This needs to be cleared 
	
	//_delay_ms(5); 

/*============================================================================
Function:   read_dials()
------------------------------------------------------------------------------
Purpose :   reads input from dials on steering wheel 
Input   :   none
Returns :   none
Notes   :
============================================================================*/
void read_dials(void)
{
	uint8_t counter = 0;
	uint16_t SwitchRight;
	uint16_t SwitchLeft;
	uint16_t SwitchRight2;
	uint16_t SwitchLeft2;
	SwitchRight = adc_read(2);
	SwitchLeft = adc_read(3);
	/*while(1)
	{
		// Pseudo debouncing

		if(SwitchRight == SwitchRight2)
		{
			break;			
		}
		if(SwitchLeft == SwitchLeft2)
		{
			break;
		}
	}*/	
	int intSwitchLeft = (SwitchLeft >> 2) / 24;
	int intSwitchRight = (SwitchRight >> 2) / 24;
	unsigned char SwitchLeftBuffer[10];
	unsigned char SwitchRightBuffer[10];
	itoa(intSwitchRight,SwitchRightBuffer,10);
	itoa(intSwitchLeft,SwitchLeftBuffer,10);
	strcat(SwitchLeftBuffer,SwitchRightBuffer);
	//Show_String(1,&SwitchLeftBuffer,0x28,0x05);
	CAN_Dial = intSwitchRight | (intSwitchLeft<<4);
}