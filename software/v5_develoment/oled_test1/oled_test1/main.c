//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//
//    Dot Matrix: 256*64
//    Driver IC : SSD1322 (Solomom Systech)
//    Interface : 8-bit 68XX/80XX Parallel, 3-/4-wire SPI
//    Revision  :
//    Date      : 2009/04/22
//
//	  Mods by Ant: 2016/07/04
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <avr/interrupt.h>
#include <math.h>

#include "inc/Functions.h"

uint16_t ThrottlePercentageData = 0;
uint16_t BreakData = 0;
uint32_t PotData = 0;

uint32_t RPMData = 0;
uint32_t GearboxTempData = 0;
uint32_t VoltageData = 0;
uint8_t radPoint;

unsigned char *firstLinePointer;// = "Throttle:";
unsigned char *secondLinePointer;// = "Break:";
unsigned char *thirdLinePointer;// = "Potentiometer:";

unsigned char emptyAll[] = "`";

unsigned char MTemp4[] = "supPeeps69";

unsigned char throttleArray[] = "Throttle:";
unsigned char BreakArray[] = "Break:";
unsigned char PotArray[] = "Potentiometer:";
unsigned char leftMotionArray[] = "Left`Dial:";
unsigned char leftMotionValueArray[] = "LeftDialValue:";

unsigned char firstLineValue[10];
unsigned char secondLineValue[10];
unsigned char thirdLineValue[10];

//unsigned char otherData[10];
unsigned char Super[] = "Azoz158";
//"supPeeps69";
//https://www.diffnow.com/compare-files


uint16_t countDigits(uint16_t tempInt){
	unsigned char intCountingArray[10];
	sprintf(intCountingArray, "%d", tempInt);
	for(uint8_t i = 0; i < sizeof intCountingArray; i++){
		if(intCountingArray[i] == '\0'){
			return i;
		}
	}
}

void parseCharArrayForDisplay(unsigned char temp[], uint16_t maximumNumberExpected){
	uint16_t digitsNumber = 0;
	for(uint8_t i = 0; i < sizeof temp; i++){
		if(temp[i] == '\0'){
			digitsNumber = countDigits(maximumNumberExpected);
			for (uint8_t k = 0; k < digitsNumber; k++)
			{
				temp[i+k] = '`';
				if(k == digitsNumber - 1){
					temp[i+k] = '\0';
				}
			}
			i = sizeof temp;
		}
	}
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Main Program
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
int main()
{
	
	DDRB = 0b11100011;	//b7 - SCK; b1 - MOSI
	DDRD = 0b00001001;	//b3 - SS pin
	SPI_clock_high;

	ADC_init();		// Initialise ADC
	spi_init();
	OLED_Init();

	CAN_init();		// Initialise CAN
	CAN_RXInit(5,8,0x400000, 0x400000);

	sei();

	//Fill_RAM(0x00);			// Clear Screen
	unsigned int index33 = 0x17;
	unsigned char character66 = 65;

	unsigned char temp_brightness = 0;

	//Display_Picture(Motorsport);

	//Delay(2000);

	//Fill_RAM(0x00);			// Clear Screen
	//Fill_RAM(0x00);

	uint16_t LeftDialADC;
	uint16_t LeftDialADCScaled;
	uint16_t RightDialADC;
	int RightDialADCScaled;
	uint8_t mob;

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
	uint16_t LeftDialADCOld = 0;
	uint16_t RightDialADCOld = 0;
	
	uint16_t chargeMax = 0;
	uint16_t breakDataMax = 0;
	uint16_t potDataMax = 0;
	
	uint16_t LeftDialADCScaledOld = 0;
	
	uint16_t isScreenChanged = 0;

	while(1)
	{
		
		readDials();
		LeftDialADC = ADC_read(3); // 3 = LEFT DIAL
		LeftDialADCScaled = (uint16_t)(LeftDialADC >> 2) / 24; // Scale values from 1 to 8 - need to verify with new PCB since dials are playing up
		//RightDialADC = ADC_read(2); // 2 = RIGHT DIAL
		//RightDialADCScaled = (int)(RightDialADC >> 2) / 24; // Scale values from 1 to 8 - need to verify with new PCB since dials are playing up
		//mob = CAN_findFreeTXMOB();
		//radPoint = readDials();
		//CAN_TXMOB(0, 1, &radPoint, 0x400001,0);
		//Display_Picture(Motorsport);
		
		
		//if((LeftDialADCOld != LeftDialADCScaled) /*|| (RightDialADCOld != RightDialADCScaled)*/)
		//{
			//ADC_Change = 1;
		//}
		
		if (LeftDialADCScaledOld != LeftDialADCScaled)
		{
			LeftDialADCScaledOld = LeftDialADCScaled;
			Fill_RAM(0x00);
			Delay(500);
		}
		
		
		if (LeftDialADCScaled == 1)
		{
			sprintf(firstLineValue, "%d", ThrottlePercentageData);
			sprintf(secondLineValue, "%d", BreakData);
			sprintf(thirdLineValue, "%d", PotData);
			firstLinePointer = throttleArray;
			secondLinePointer = BreakArray;
			thirdLinePointer = PotArray;
			chargeMax = 75;
			breakDataMax = 100;
			potDataMax = 100;
			
		}else if (LeftDialADCScaled == 2)
		{
			sprintf(firstLineValue, "%d", LeftDialADC);
			sprintf(secondLineValue, "%c", "'");
			sprintf(thirdLineValue, "%d", LeftDialADCScaled);
			
			firstLinePointer = leftMotionArray;
			secondLinePointer = emptyAll;
			thirdLinePointer = leftMotionValueArray;
			chargeMax = 1023;
			breakDataMax = 0;
			potDataMax = 100;
		}else{
			sprintf(firstLineValue, "%c", "'");
			sprintf(secondLineValue, "%c", "'");
			sprintf(thirdLineValue, "%c", "'");
			
			firstLinePointer = emptyAll;
			secondLinePointer = emptyAll;
			thirdLinePointer = emptyAll;
		}
		
		/*
		//sprintf(Charge, "%d", ThrottlePercentageData);
		sprintf(Charge, "%d", LeftDialADC);
		sprintf(breakDataArray, "%d", BreakData);
		//sprintf(PotDataArray, "%d", PotData);
		sprintf(PotDataArray, "%d", LeftDialADCScaled);
		*/
		parseCharArrayForDisplay(firstLineValue, chargeMax);
		parseCharArrayForDisplay(secondLineValue, breakDataMax);
		parseCharArrayForDisplay(thirdLineValue, potDataMax);

		Delay(100);

		//Fill_RAM(0x00);			// Clear Screen
		//Delay(10);
		
		Show_String(1,&(*firstLinePointer),0x05,0x05);
		Show_String(1,&firstLineValue,0x27,0x05);

		Show_String(1,&(*secondLinePointer),0x05,0x15);
		Show_String(1,&secondLineValue,0x27,0x15);

		Show_String(1,&(*thirdLinePointer),0x05,0x25);
		Show_String(1,&thirdLineValue,0x27,0x25);
	}
	
	return 0;
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Pin Definition
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//Serial Clock	PB7
//Serial Data	PB0
//	D/C			PB5
//	CS			PD0
//	RST			PB6

////////////////////////////////////////////////////////////////////////
//#define xData	P1				// Parallel Data Input/Output

//#define sclk	p1_0				// serial clock input
//#define sdin	p1_1				// serial data input
//
//#define res	p3_3				// reset
//#define cs	p3_4				// chip select
//#define dc	p3_2				// data/command control
//
//#define e	p3_0				// read/write enable
//#define rw	p3_1				// read/write select
//
//#define rd	p3_0				// read signal
//#define wr	p3_1				// write signal


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Interrupts
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

ISR(INT3_vect)
{
	SMCR = (0<<SM2)|(0<<SM1)|(1<<SM0)|(0<<SE);		//disable the mode, and disable the enable bit.
}

ISR(CAN_INT_vect)
{
	if((CANSIT2 & (1 << SIT5)))	//we received a CAN message on mob 5, which is set up to receive exclusively from the AMU.
	{		
		CANPAGE = (5 << 4);			//set the canpage to the receiver MOB
		ThrottlePercentageData = CANMSG;
		
		BreakData = CANMSG;
		
		PotData = CANMSG;

		CAN_RXInit(5,8,0x400000, 0x400000);
	}
	
	CANPAGE = (5 << 4);			//set the canpage to the receiver MOB
	CANSTMOB &= ~(1 << RXOK);	//unset the RXOK bit to clear the interrupt.
	CANPAGE = (4 << 4);
	CANSTMOB &= ~(1 << RXOK);	//unset the RXOK bit to clear the interrupt.
}


/*
//	\0
		
	for(uint8_t i = 0; i < sizeof Charge; i++){
		if(Charge[i] == '\0'){
			Charge[i] = '`';
			Charge[i+1] = '`';
			Charge[i+2] = '\0';//0x2302;
			i = sizeof Charge;
		}
	}
	
	//ThrottlePercentageData = CANMSG << 8;
	//ThrottlePercentageData += CANMSG;
	//ThrottlePercentageData = ThrottlePercentageData * 100 /511; // Scale to percentage
	//ThrottlePercentageData = 46;
	//MTemp2[3] = ThrottlePercentageData;
	
	//MTemp2[3] = ThrottlePercentageData;
	//CANMSG = 100000000;
	//RPMData = CANMSG << 1;
	
	
	//GearboxTempData = CANMSG << 8;
	//GearboxTempData += CANMSG;
	//VoltageData = CANMSG << 8;
	//VoltageData += CANMSG;
	
	// JOhn said NO
	//radPoint = readDials();
	//CAN_TXMOB(0, 1, &radPoint, 0x400001,0);
	
	//BreakData = CANMSG;
	
	//memset(PotDataArray, 0, sizeof PotDataArray);
		

*/