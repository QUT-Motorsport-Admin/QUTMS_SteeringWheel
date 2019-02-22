/*****************************************************************************
* @file    Software/src/steeringWheelInput.c 
* @author  Zoe Goodward
* @version V1.0.0
* @date    28-April-2018
* @brief   
*****************************************************************************/

//External libraries
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <math.h>

#include "steeringWheelInput.h"
#include "steeringWheelBitOpr.h"
#include "steeringWheelADC.h"
#include "steeringWheelCAN.h"
//#include "steeringWheelFunctions.h"


/*
//12-position rotary switch
const int firstRotaryPin = 7;
const int lastRotaryPin = 26;

void read_rotary_switch(void) {
	// Switch 1 == right  sw1
	// Swtitch 2 == left  sw2
	// rotary pins left  17-26,33
	// rotary pins right  7-16
	for( int i=firstRotaryPin; i<= lastRotaryPin; i++) { 
    int val = digitalRead( i ); // look at a rotary switch input adc_read()
    if( val == LOW ) { // it's selected
      return (i - firstRotaryPin + 1); // return a value that ranges 1 - 5
    }
  }
  return 0; // error case
}

// main
	int rotary_pos = read_rotary_switch();
	if (rotary_pos == 7) {

	}

*/


/*============================================================================
Function:   button_init()
------------------------------------------------------------------------------
Purpose :   sets DDR's to enable input from the buttons on the steering wheel 
Input   :   none
Returns :   none
Notes   :	not tested
============================================================================*/
void button_init(void) {
	// Buttton one (left maybe? test later) set port to input
	CLEAR_BIT(DDRC, 0);
	// Button two (right maybe?)
    CLEAR_BIT(DDRC, 1);
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
	// when button one pressed
	if (BIT_IS_SET(PINC, 0)) {
		// do stuff
	}
	// when button two pressed
	if (BIT_IS_SET(PINC, 1)) {
		// do stuff
	}
	//do stuff...
}