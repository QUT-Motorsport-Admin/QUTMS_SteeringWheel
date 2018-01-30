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

uint16_t Data = 0;
uint32_t Alarms = 0;
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
		Data = CANMSG << 8;
		Data += CANMSG;
		Alarms = CANMSG << 8;
		Alarms += CANMSG;
		GearboxTempData = CANMSG << 8;
		GearboxTempData += CANMSG;
		VoltageData = CANMSG << 8;
		VoltageData += CANMSG;

		CAN_RXInit(5,8,0x200000, 0x200000);
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
	CAN_RXInit(5,8,0x200000, 0x200000);
	ADC_init();		// Initialise ADC
	spi_init();		// Initialise SPI (OLED screen communication)
	OLED_Init();	// Initialise the OLED screen
	sei();
	
	/* Splash Screen */
	Fill_RAM(0x00);			// Clear Screen
	Display_Picture(Motorsport);
	Delay(3000);
	Fill_RAM(0x00);			// Clear Screen
	
	// Main Variables
	// If using Show_String, append 0x00 as an extra character on end of the string, eg: unsigned char Acceleration[]= {'A','C','C','E','L','E','R','A','T','I','O','N',0x00};
	unsigned char DataText[] = {'D','A','T','A',':'};
	unsigned char AlarmsText[] = {'A','L','A','R','M','S',':'};
	unsigned char BlankText[] = {' ',' ',' ',' '};
	unsigned char TempBuffer[10];
	int digitLengthData = 0;
	int digitLengthDataOld = 0;
	int digitLengthAlarm = 0;
	int digitLengthAlarmOld = 0;
	uint8_t mob;
	
	while(1)
	{	

		itoa(Data,TempBuffer,10);
		digitLengthData = (Data == 0 ? 1 : (int)(log10(Data)+1));
		if(digitLengthDataOld != digitLengthData){
			Show_Big_String(&BlankText,0x2D,0x64,0x08,0x7F, 4, 0x0F, 0xF0);
		}
		Show_Big_String(&DataText,0x05,0x64,0x08,0x7F, 5, 0x0F, 0xF0);
		Show_Big_String(&TempBuffer,0x2D,0x64,0x08,0x7F, digitLengthData, 0x0F, 0xF0); // Print the current data
		digitLengthDataOld = digitLengthData;
		
		itoa(Alarms,TempBuffer,10);
		digitLengthAlarm = (Alarms == 0 ? 1 : (int)(log10(Alarms)+1));
		if(digitLengthAlarmOld != digitLengthAlarm){
			Show_Big_String(&BlankText,0x2D,0x64,0x24,0x7F, 4, 0x0F, 0xF0);
		}
		Show_Big_String(&AlarmsText,0x05,0x64,0x24,0x7F, 7, 0x0F, 0xF0);
		Show_Big_String(&TempBuffer,0x2D,0x64,0x24,0x7F, digitLengthAlarm, 0x0F, 0xF0); // Print the current alarms
		digitLengthAlarmOld = digitLengthAlarm;
		
		_delay_ms(10);
		
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