/*****************************************************************************
* @file    Software/src/steeringWheelSPI.c
* @author  Zoe Goodward
* @version V1.0.0
* @date    28-April-2018
* @brief   Communication with the OLED screen
*****************************************************************************/

#include "steeringWheelSPI.h"
#include "steeringWheelBitOpr.h"

/*============================================================================
Function:   spi_init()
------------------------------------------------------------------------------
Purpose :   initialise the spi
Input   :   none
Returns :   
Notes   :
============================================================================*/
void spi_init()
{
     //make sure we are getting spi not on the _A (alternative) spi pins.
	CLEAR_BIT(MCUCR, SPIPS);  
    //set interrupt, enable SPI, MSB mode, set as master, low is idle, read on leading edge, Set to speed(fosc/4) SPR0=1, SPR1=0
	SPCR = (0<<SPIE)|(1<<SPE)|(0<<DORD)|(1<<MSTR)|(0<<CPOL)|(0<<CPHA)|(0<<SPR0)|(0<<SPR1);  
    //SPSR = (1<<SPI2X);	//set whether we want 2x speed or not (1=2x speed).
	OVERRIDE_BITS(SPSR, SPI2X); 
    //ensure data register has nothing in it 
	SPDR = 0x00;		
}

/*============================================================================
Function:  spi_send_byte()
------------------------------------------------------------------------------
Purpose	:  initiate the transmission of data into the SPI Data Register
Input	:  uint8_t c - data to be shifted
Returns	:  SPDR - data that was shifted into this register upon transmission
Notes	:
============================================================================*/
uint8_t spi_send_byte(uint8_t c)
{
	SPDR = c;	/* initiate transmission */			
	while(!(CHECK_BIT(SPSR, SPIF))); /* end of transmission flag */
	return SPDR;				
}
