//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//    Dot Matrix: 256*64
//    Driver IC : SSD1322 (Solomon Systech)                                             |  |
//    Interface : 4-wire SPI							  					            v  v
//	  MAKE SURE BSO PADS 00 ON BACK OF SCREEN ARE SHORTED WITH RESISTORS. VISUALLY PADS 0110 (pads 0,0 are shorted, pads 1,1 are open)
//																		                ^  ^
//	  Mods by Ant: 2016/07/04												            |  |
//	  Various mods by Shaun: 2016/11/21 (Last edited)
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <string.h>
#include <math.h>
#include "AtmelCAN.h"
#include "OLED.h"
#include "Images.h"

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
	SMCR = (0<<SM2)|(0<<SM1)|(1<<SM0)|(0<<SE);		//disable the mode, and disable the enable bit.
}

ISR(CAN_INT_vect)
{
	//uint8_t authority;
	int8_t mob;
	if((CANSIT2 & (1 << SIT5)))	//we received a CAN message on mob 5, which is set up to receive exclusively from the AMU.
	{
		
		CANPAGE = (5 << 4);			//set the canpage to the receiver MOB
		ThrottlePercentageData = CANMSG << 8;
		ThrottlePercentageData += CANMSG;
		ThrottlePercentageData = ThrottlePercentageData * 100 /511; // Scale to percentage
		RPMData = CANMSG << 8;
		RPMData += CANMSG;
		GearboxTempData = CANMSG << 8;
		GearboxTempData += CANMSG;
		VoltageData = CANMSG << 8;
		VoltageData += CANMSG;

		CAN_RXInit(5,8,0x400000, 0x400000);
	}
	
	CANPAGE = (5 << 4);			//set the canpage to the receiver MOB
	CANSTMOB &= ~(1 << RXOK);	//unset the RXOK bit to clear the interrupt.
	CANPAGE = (4 << 4);
	CANSTMOB &= ~(1 << RXOK);	//unset the RXOK bit to clear the interrupt.
}

// ADDRESS TO LISTEN FOR: 10010000000000000000000000001
// FIRST UNSIGNED INT: THROTTLE
// SECOND UNSIGNED INT: RPM
// THIRD UNSIGNED INT: GEARBOX TEMP
// FOURTH UNSIGNED INT: VOLTAGE

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  ADC
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void ADC_init()
{
	ADMUX=(1<<REFS0)|(1<<AREFEN);                      // For Aref=AVcc with external capacitor;
	ADMUX &= ~(1<<ADLAR);								//make sure adlar is not set.
	ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //Prescaler div factor = 128, 125kHz --> lowest we can go for best accuracy.
}
uint16_t ADC_read(uint8_t channel)
{
	channel = (ADMUX & 0xe0)|(channel & 0x1F); //ADMUX | 0b11100000 and channel | 0b00011111 --> this keeps all bits of ADMUX the same except for the bits signalling which channel to use.
	ADMUX = channel;
	ADCSRA |= (1<<ADSC);							//ADSC (single conversion bit) is set to 1 to start the conversion process.
	while(!(ADCSRA & (1<<ADIF)));				//run a loop while the conversion is taking place.
	uint16_t result = 0;
	result = ADCL;								//read ADCL first, ADCH after --> order is important! --> also not sure if this code is correct. other ADC examples return 'ADC' instead.
	result |= ((3 & ADCH) << 8);
	ADCSRA|=(1<<ADIF);							//once read and done, clear the 'complete' status by writing 1 to the ADIF bit.
	return result;								//pass the 10 bit ADC number to requesting function.
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  SPI Functions
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void spi_init()
{
	MCUCR &= ~(1<<SPIPS);		//make sure we are getting spi not on the _A spi pins.
	SPCR = (0<<SPIE)|(1<<SPE)|(0<<DORD)|(1<<MSTR)|(0<<CPOL)|(0<<CPHA)|(0<<SPR0)|(0<<SPR1);  //set interrupt, enable SPI, MSB mode, set as master, low is idle, read on leading edge, Set to speed(fosc/4) SPR0=1, SPR1=0
	SPSR = (1<<SPI2X);	//set whether we want 2x speed or not (1=2x speed).
	SPDR = 0x00;		//ensure data register has nothing in it
}

uint8_t spi_send_byte(uint8_t c)
{
	SPDR = c;					//write data to the transmission register. Writing to this initiates transmission.
	while(!(SPSR & (1<<SPIF)));
	return SPDR;				//return any data that was shifted into this register upon transmission.
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
	
	CAN_init();		// Initialise CAN
	CAN_RXInit(5,8,0x400000, 0x400000);
	ADC_init();		// Initialise ADC
	spi_init();		// Initialise SPI (OLED screen communication)
	OLED_Init();	// Initialise the OLED screen
	sei();
	
	Fill_RAM(0x00);			// Clear Screen
	
	Display_Picture(Motorsport);
	Delay(3000);

	Fill_RAM(0x00);			// Clear Screen

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
	
	while(1)
	{	
		readDials();
		LeftDialADC = ADC_read(3); // 3 = LEFT DIAL
		LeftDialADCScaled = (int)(LeftDialADC >> 2) / 24; // Scale values from 1 to 8 - need to verify with new PCB since dials are playing up
		RightDialADC = ADC_read(2); // 2 = RIGHT DIAL
		RightDialADCScaled = (int)(RightDialADC >> 2) / 24; // Scale values from 1 to 8 - need to verify with new PCB since dials are playing up
		mob = CAN_findFreeTXMOB();
		//if(mob > 0)
		CAN_TXMOB(0, 1, &CAN_Dial, 0x400001,0);
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
				Fill_RAM(0x00);
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
				Fill_RAM(0x00);
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
				Fill_RAM(0x00);
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
				Fill_RAM(0x00);
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
				Fill_RAM(0x00);
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
			test = ADC_read(2);
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
				Fill_RAM(0x00);			// Clear Screen
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

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Read Dials
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void readDials(void)
{
	uint8_t counter = 0;
	uint16_t SwitchRight;
	uint16_t SwitchLeft;
	uint16_t SwitchRight2;
	uint16_t SwitchLeft2;
	SwitchRight = ADC_read(2);
	SwitchLeft = ADC_read(3);
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

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  FTOA
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// reverses a string 'str' of length 'len'
void reverse(char *str, int len)
{
	int i=0, j=len-1, temp;
	while (i<j)
	{
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
		i++; j--;
	}
}

// Converts a given integer x to string str[].  d is the number
// of digits required in output. If d is more than the number
// of digits in x, then 0s are added at the beginning.
int intToStr(int x, char str[], int d)
{
	int i = 0;
	while (x)
	{
		str[i++] = (x%10) + '0';
		x = x/10;
	}
	
	// If number of digits required is more, then
	// add 0s at the beginning
	while (i < d)
	str[i++] = '0';
	
	reverse(str, i);
	str[i] = '\0';
	return i;
}

// Converts a floating point number to string.
void ftoa(float n, char *res, int afterpoint)
{
	// Extract integer part
	int ipart = (int)n;
	
	// Extract floating part
	float fpart = n - (float)ipart;
	
	// convert integer part to string
	int i = intToStr(ipart, res, 0);
	
	// check for display option after point
	if (afterpoint != 0)
	{
		res[i] = '.';  // add dot
		
		// Get the value of fraction part upto given no.
		// of points after dot. The third parameter is needed
		// to handle cases like 233.007
		fpart = fpart * pow(10, afterpoint);
		
		intToStr((int)fpart, res + i + 1, afterpoint);
	}
}