/*****************************************************************************
* @file    Software/src/steeringWheelInput.c 
* @author  Zoe Goodward
* @version V1.0.0
* @date    28-April-2018
* @brief   
*****************************************************************************/

#include "steeringWheelInput.h"

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

/*============================================================================
Function:   read_buttons()
------------------------------------------------------------------------------
Purpose :   reads input from the buttons on the steering wheel 
Input   :   none
Returns :   none
Notes   :	not started yet
============================================================================*/
void read_buttons(void) {
	//do stuff...
}