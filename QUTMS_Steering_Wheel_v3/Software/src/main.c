/*****************************************************************************
* @file    Software/src/main.c
* @author  Zoe Goodward
* @version V1.0.0
* @date    28-April-2018
* @brief   Main program body
*****************************************************************************/

//External libraries
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <string.h>
#include <math.h>

//Internal libraries
#include "steeringWheelCAN.h"
#include "steeringWheelOLED.h"
#include "steeringWheelGraphics.h"
#include "steeringWheelBitOpr.h"
#include "steeringWheelBitmaps.h"

uint16_t ThrottlePercentageData = 0;
uint32_t RPMData = 0;
uint32_t GearboxTempData = 0;
uint32_t VoltageData = 0;
uint8_t CAN_Dial = 0;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  ISR
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

ISR(INT3_vect)
{
	SMCR = (0<<SM2)|(0<<SM1)|(1<<SM0)|(0<<SE);  //disable the mode, and disable the enable bit.
	// or SMCR = 2
}

ISR(CAN_INT_vect)
{
	//uint8_t authority;
	int8_t mob;
	if( (CHECK_BIT(CANSIT2, SIT5)))  //(CANSIT2 & (1 << SIT5)))	//we received a CAN message on mob 5, which is set up to receive exclusively from the AMU.
	{
		
		CANPAGE = (5 << 4);			//set the canpage to the receiver MOB
		ThrottlePercentageData = CANMSG << 8;
		ThrottlePercentageData += CANMSG;
		ThrottlePercentageData = ThrottlePercentageData * 100 / 511; // Scale to percentage
		RPMData = CANMSG << 8;
		RPMData += CANMSG;
		GearboxTempData = CANMSG << 8;
		GearboxTempData += CANMSG;
		VoltageData = CANMSG << 8;
		VoltageData += CANMSG;

		can_rx_init(5,8,0x400000, 0x400000);
	}
	
	CANPAGE = (5 << 4);		   //set the canpage to the receiver MOB
	CLEAR_BIT(CANSTMOB, RXOK); // CANSTMOB &= ~(1 << RXOK);	//unset the RXOK bit to clear the interrupt.
	CANPAGE = (4 << 4);
	CLEAR_BIT(CANSTMOB, RXOK); // CANSTMOB &= ~(1 << RXOK);	//unset the RXOK bit to clear the interrupt.
}

// ADDRESS TO LISTEN FOR: 10010000000000000000000000001
// FIRST UNSIGNED INT: THROTTLE
// SECOND UNSIGNED INT: RPM
// THIRD UNSIGNED INT: GEARBOX TEMP
// FOURTH UNSIGNED INT: VOLTAGE


void initialise() 
{
	can_init();
	can_rx_init(5,8,0x400000, 0x400000);
	adc_init();
	spi_init(); // Initialise SPI (OLED screen communication)
	oled_init();
	sei(); // Enable interrupts
}

void loading_screen() 
{
	fill_ram(CLEAR_SCREEN);
	Display_Picture(motorsport);
	Delay(3000);
	fill_ram(CLEAR_SCREEN);
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Main Program
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
int main()
{
	DDRB = 0b11100011;	//b7 - SCK; b1 - MOSI
	//DDRC &= ~(1 << PINC0);
	DDRD = 0b00001001;	//b3 - SS pin
	SPI_clock_high;
	
	initialise();
	loading_screen();
	
	/*
	Draw_Rectangle(0xFF,0x01,0x00,0x3F,0x00,0x10);
	Draw_Rectangle(0xFF,0x01,0x00,0x3F,0x11,0x20);
	Draw_Rectangle(0xFF,0x01,0x00,0x3F,0x21,0x30);
	Draw_Rectangle(0xFF,0x01,0x00,0x3F,0x31,0x3F);
	*/
	
	// Globals
	// If using Show_String, append 0x00 as an extra character on end of the string, eg: unsigned char Acceleration[]= {'A','C','C','E','L','E','R','A','T','I','O','N',0x00};
	unsigned char Acceleration[]= {'A','C','C','E','L','E','R','A','T','I','O','N'};
	unsigned char SkidPad[]= {'S','K','I','D','P','A','D'};
	unsigned char AutoCross[]= {'A','U','T','O','C','R','O','S','S'};
	unsigned char Endurance[]= {'E','N','D','U','R','A','N,','C','E'};
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
	///
		
	while(1)
	{	
		#define LEFT_DIAL 3
		#define RIGHT_DIAL 2
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
					Show_Bigger_String(&RightArrow,0x3A,0x60,0x11,0x7F, 1, 0x0F, 0xF0);
				}
				if(LeftDialADCScaled < LeftDialADCOld){
					Show_Bigger_String(&LeftArrow,0x00,0x15,0x11,0x7F, 1, 0x0F, 0xF0);
				}
				LeftDialADCOld = LeftDialADCScaled;
				Delay(500);
				fill_ram(CLEAR_SCREEN);
			}
			
			if(ThrottleMaxFlag == 1){
				//Show_String(1,&BlankNumber,0x28,0x05);
				ThrottleMaxFlag = 0;
			}
			if(ThrottlePercentageData == 100){ ThrottleMaxFlag = 1;	}
			// If throttle is at 10%, when the throttle goes down, a trailing 0 will be left - this needs to be cleared
			if(ThrottlePercentageData < 10){ ThrottleGreaterThan10 = 1;	}
			if(ThrottleGreaterThan10 == 1){
				//Show_String(1,&BlankNumber,0x28,0x05);
				ThrottleGreaterThan10 = 0;
			}
			itoa(ThrottlePercentageData,TempBuffer,10);
			digitLength = (ThrottlePercentageData == 0 ? 1 : (int)(log10(ThrottlePercentageData)+1));
			Show_Big_String(&Throttle,0x05,0x64,0x16,0x7F, 12, 0x0F, 0xF0);		
			Show_String(1,TempBuffer,0x28,0x05);
			Show_Big_String(&TempBuffer,0x2D,0x64,0x16,0x7F, digitLength, 0x0F, 0xF0); // Print the current throttle percentage
			Show_Bigger_String(&RightArrow,0x3A,0x60,0x11,0x7F, 1, 0x04, 0x40);
		}
		
		if(LeftDialADCScaled == 4)
		{
			if(ADC_Change == 1)
			{
				ADC_Change = 0;
				if(LeftDialADCScaled > LeftDialADCOld){
					Show_Bigger_String(&RightArrow,0x3A,0x60,0x11,0x7F, 1, 0x0F, 0xF0);
				}
				if(LeftDialADCScaled < LeftDialADCOld){
					Show_Bigger_String(&LeftArrow,0x00,0x15,0x11,0x7F, 1, 0x0F, 0xF0);
				}
				LeftDialADCOld = LeftDialADCScaled;
				Delay(500);
				fill_ram(CLEAR_SCREEN);
			}
			
			// If RPM is greater than 100 (or 1000), when the throttle goes down, a trailing 0 will be left
			// This needs to be cleared
			if(RPMData < 99){ RPMGreaterThan100 = 1; }
			if(RPMData < 999){ RPMGreaterThan100 = 1; }
			if(RPMGreaterThan100 == 1){
				//Show_String(1,&BlankNumber,0x28,0x15);
				RPMGreaterThan100 = 0;
			}
			itoa(RPMData,TempBuffer,10);
			digitLength = (RPMData == 0 ? 1 : (int)(log10(RPMData)+1));
			Show_String(1,TempBuffer,0x28,0x05);
			Show_Big_String(&RPM,0x05,0x64,0x16,0x7F, 5, 0x0F, 0xF0);
			Show_Big_String(&TempBuffer,0x2D,0x64,0x16,0x7F, digitLength, 0x0F, 0xF0);
			Show_Bigger_String(&LeftArrow,0x00,0x15,0x11,0x7F, 1, 0x04, 0x40);
			Show_Bigger_String(&RightArrow,0x3A,0x60,0x11,0x7F, 1, 0x04, 0x40);
		}
		
		if(LeftDialADCScaled == 5)
		{
			if(ADC_Change == 1)
			{
				ADC_Change = 0;
				if(LeftDialADCScaled > LeftDialADCOld){
					Show_Bigger_String(&RightArrow,0x3A,0x60,0x11,0x7F, 1, 0x0F, 0xF0);
				}
				if(LeftDialADCScaled < LeftDialADCOld){
					Show_Bigger_String(&LeftArrow,0x00,0x15,0x11,0x7F, 1, 0x0F, 0xF0);
				}
				LeftDialADCOld = LeftDialADCScaled;
				Delay(500);
				fill_ram(CLEAR_SCREEN);
			}
			
			itoa(GearboxTempData,TempBuffer,10);
			digitLength = (GearboxTempData == 0 ? 1 : (int)(log10(GearboxTempData)+1));
			Show_String(1,TempBuffer,0x28,0x05);
			Show_Big_String(&GearboxTemp,0x05,0x64,0x16,0x7F, 14, 0x0F, 0xF0);
			Show_Big_String(&TempBuffer,0x30,0x64,0x16,0x7F, digitLength, 0x0F, 0xF0);
			Show_Bigger_String(&LeftArrow,0x00,0x15,0x11,0x7F, 1, 0x04, 0x40);
			Show_Bigger_String(&RightArrow,0x3A,0x60,0x11,0x7F, 1, 0x04, 0x40);
		}
		
		if(LeftDialADCScaled == 6)
		{
			if(ADC_Change == 1)
			{
				ADC_Change = 0;
				if(LeftDialADCScaled > LeftDialADCOld){
					Show_Bigger_String(&RightArrow,0x3A,0x60,0x11,0x7F, 1, 0x0F, 0xF0);
				}
				if(LeftDialADCScaled < LeftDialADCOld){
					Show_Bigger_String(&LeftArrow,0x00,0x15,0x11,0x7F, 1, 0x0F, 0xF0);
				}
				LeftDialADCOld = LeftDialADCScaled;
				Delay(500);
				fill_ram(CLEAR_SCREEN);
			}
			
			itoa(VoltageData/1000, TempBuffer, 10);		//display the average cell voltage in Volts instead of millivolts (find the whole volts in the value)
			TempBuffer[1] = '.';						//add the decimal point
			itoa(VoltageData-((VoltageData/1000)*1000), TempBuffer+2, 10);	//add the remaining millivolts
			Show_String(1,TempBuffer,0x28,0x05);		//display the result.
			Show_Big_String(&Voltage,0x05,0x64,0x16,0x7F, 9, 0x0F, 0xF0);
			Show_Big_String(&TempBuffer,0x2D,0x64,0x16,0x7F, 4, 0x0F, 0xF0);
			Show_Bigger_String(&LeftArrow,0x00,0x15,0x11,0x7F, 1, 0x04, 0x40);
			Show_Bigger_String(&RightArrow,0x3A,0x60,0x11,0x7F, 1, 0x04, 0x40);
		}
		
		if(LeftDialADCScaled > 6)
		{
			if(ADC_Change == 1)
			{
				ADC_Change = 0;
				if(LeftDialADCScaled > LeftDialADCOld){
					Show_Bigger_String(&RightArrow,0x3A,0x60,0x11,0x7F, 1, 0x0F, 0xF0);
				}
				if(LeftDialADCScaled < LeftDialADCOld){
					Show_Bigger_String(&LeftArrow,0x00,0x15,0x11,0x7F, 1, 0x0F, 0xF0);
				}
				LeftDialADCOld = LeftDialADCScaled;
				Delay(500);
				fill_ram(CLEAR_SCREEN);
			}
			Show_Bigger_String(&LeftArrow,0x00,0x15,0x11,0x7F, 1, 0x04, 0x40);
		}
		
		/*
		unsigned char c = 0x08;
		unsigned char d = 0x04;
		Set_Column_Address(Shift+c,Shift+c);
		Set_Row_Address(d,d+7);
		Set_Write_RAM();
		Write_Data(0x00);
		Write_Data(0xF0);
		//Write_Data(0x77);
		//Write_Data(0xFF);
		//Write_Data(0xFF);
		
		c = 0x08;
		d = 0x08;
		Set_Column_Address(Shift+c,Shift+c);
		Set_Row_Address(d,d+7);
		Set_Write_RAM();
		Write_Data(0x00);
		Write_Data(0xF0);
*/
	
		/*
		while(BootFlag == 0)
		{
			Show_String(1,&Acceleration,0x13,0x05);
			Show_String(1,SkidPad,0x17,0x15);
			Show_String(1,&AutoCross,0x16,0x25);
			Show_String(1,&Endurance,0x16,0x35);
			test = adc_read(2);
			test2 = (int)(test >> 2) / 24;
			
			if(test2 == 3)
			{
				Show_String(1,&BlankNumber,0x05,0x25);
				Show_String(1,&BlankNumber,0x31,0x25);
				Show_String(1,&BlankNumber,0x05,0x15);
				Show_String(1,&BlankNumber,0x31,0x15);
				Show_String(1,&BlankNumber,0x05,0x35);
				Show_String(1,&BlankNumber,0x31,0x35);
				Show_String(1,&SelectionLeft,0x05,0x05);
				Show_String(1,&SelectionRight,0x31,0x05);

			}
			if(test2 == 4)
			{
				Show_String(1,&BlankNumber,0x05,0x25);
				Show_String(1,&BlankNumber,0x31,0x25);
				Show_String(1,&BlankNumber,0x05,0x05);
				Show_String(1,&BlankNumber,0x31,0x05);
				Show_String(1,&BlankNumber,0x05,0x35);
				Show_String(1,&BlankNumber,0x31,0x35);
				Show_String(1,&SelectionLeft,0x05,0x15);
				Show_String(1,&SelectionRight,0x31,0x15);
			}
			if(test2 == 5)
			{
				Show_String(1,&BlankNumber,0x05,0x05);
				Show_String(1,&BlankNumber,0x31,0x05);
				Show_String(1,&BlankNumber,0x05,0x15);
				Show_String(1,&BlankNumber,0x31,0x15);
				Show_String(1,&BlankNumber,0x05,0x35);
				Show_String(1,&BlankNumber,0x31,0x35);
				Show_String(1,&SelectionLeft,0x05,0x25);
				Show_String(1,&SelectionRight,0x31,0x25);
			}
			if(test2 == 6)
			{
				Show_String(1,&BlankNumber,0x05,0x25);
				Show_String(1,&BlankNumber,0x31,0x25);
				Show_String(1,&BlankNumber,0x05,0x15);
				Show_String(1,&BlankNumber,0x31,0x15);
				Show_String(1,&BlankNumber,0x05,0x05);
				Show_String(1,&BlankNumber,0x31,0x305);
				Show_String(1,&SelectionLeft,0x05,0x35);
				Show_String(1,&SelectionRight,0x31,0x35);
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
		
	}	
	return 0;
}

//implement speed, heat, battery amount, race mode, 