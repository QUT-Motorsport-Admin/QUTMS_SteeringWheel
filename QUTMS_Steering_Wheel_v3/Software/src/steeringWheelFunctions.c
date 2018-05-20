/*****************************************************************************
* @file    Software/src/steeringWheelFunctions.c
* @author  Zoe Goodward
* @version V1.0.0
* @date    28-April-2018
* @brief   Functions for general opperation of steering wheel
*****************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "steeringWheelFunctions.h"

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
	can_init();
	can_rx_init(5,8,0x400000, 0x400000);
	adc_init();
	spi_init();
	oled_init();
	sei(); /* enable interrupts */
	splash_screen();
}

/*============================================================================
Function:   splash_screen()
------------------------------------------------------------------------------
Purpose :   displays the QUT Motorsport logo when the steering wheel system
			is first initialised
Input   :   none
Returns :   void
Notes   :
============================================================================*/
void splash_screen()
{
	fill_ram(CLEAR_SCREEN);
	display_picture(MOTORSPORT);
	Delay(3000);
	fill_ram(CLEAR_SCREEN);
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
	for ( int i = 0; i < N_PARROT; i++ ) {
		char parrotFrame[N_PARROT+1];
		sprintf(parrotFrame, "PARROT_00%d", i);
		display_picture(parrotFrame);
		Delay(100); /* temp value, will test later */
		if ( i == (N_PARROT - 1) ) {
			i = 0;
		}
	}
}