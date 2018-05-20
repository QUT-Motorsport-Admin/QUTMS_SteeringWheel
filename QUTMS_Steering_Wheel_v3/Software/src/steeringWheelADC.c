/*****************************************************************************
* @file    Software/src/steeringWheelADC.c
* @author  Zoe Goodward
* @version V1.0.0
* @date    28-April-2018
* @brief   Analog to digital conversion
*****************************************************************************/

#include "steeringWheelADC.h"

/*============================================================================
Function:   adc_init()
------------------------------------------------------------------------------
Purpose :   initializes the analog to digital conversion by first selecting 
			the voltage reference and then setting the ADC Enable bit, ADEN 
			in ADCSRA
Input   :   none
Returns :   void
Notes   :
============================================================================*/
void adc_init() 
{
	ADMUX = (1 << REFS0)|(1 << AREFEN); /* AVcc with external capacitor */
	CLEAR_BIT(ADMUX, ADLAR); /* make sure bits are right adjusted */ 
	ADCSRA = (1 << ADEN)|(1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0); /* prescale div factor = 128, 125kHz */
}

/*============================================================================
Function:   adc_read()
------------------------------------------------------------------------------
Purpose :   reads an analog input voltage and converts it to a 10-bit digital
			value through successive approximation
Input   :   uint8_t channel - selected analog input channel
Returns :   result - pass the 10 bit ADC number to requesting function
Notes   :
============================================================================*/
uint16_t adc_read(uint8_t channel) 
{   
    channel = (ADMUX & 0xe0)|(channel & 0x1F); /* only change ADMUX bits signalling which channel to use */
	ADMUX = channel;
	SET_BIT(ADCSRA, ADSC); /* start conversion process */
	while(!(CHECK_BIT(ADCSRA, ADIF))); /* loop while the conversion is taking place */
	uint16_t result = 0;	
	result = ADCL; /* read ADCL, then ADCH --> order is important! */							
	result |= ((3 & ADCH) << 8);
	//--> also not sure if this code is correct. other ADC examples return 'ADC' instead. //
	SET_BIT(ADCSRA, ADIF); /* clear 'complete' status */
	return result;
}