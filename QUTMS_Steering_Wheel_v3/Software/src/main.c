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
#include <math.h>

//Internal libraries
#include "steeringWheelCAN.h"
#include "steeringWheelOLED.h"
#include "steeringWheelGraphics.h"
#include "steeringWheelBitOpr.h"
//#include "steeringWheelBitmaps.h"
#include "steeringWheelFunctions.h"
#include "main.h"

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  ISR
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

/*
// Global variables
uint16_t ThrottlePercentageData = 0;
uint32_t RPMData = 0;
uint32_t GearboxTempData = 0;
uint32_t VoltageData = 0;
uint8_t CAN_Dial = 0;
*/

ISR(INT3_vect)
{
	SMCR = (0<<SM2)|(0<<SM1)|(1<<SM0)|(0<<SE);  //disable the mode, and disable the enable bit.
	// or SMCR = 2
}
/*
ISR(CAN_INT_vect)
{
	//uint8_t authority;
	//int8_t mob;
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
*/

// ADDRESS TO LISTEN FOR: 10010000000000000000000000001
// FIRST UNSIGNED INT: THROTTLE
// SECOND UNSIGNED INT: RPM
// THIRD UNSIGNED INT: GEARBOX TEMP
// FOURTH UNSIGNED INT: VOLTAGE

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Main Program
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
int main(void)
{
	// Initialise steering functionality: buttons, oled etc..
	steering_wheel_init();
			
	while(1)
	{	
		update_screen();		
	}	
	return 0;
}