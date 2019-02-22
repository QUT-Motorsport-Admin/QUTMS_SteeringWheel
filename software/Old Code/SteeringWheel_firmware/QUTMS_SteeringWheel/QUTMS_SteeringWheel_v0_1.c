/*
 * QUTMS_SteeringWheel.c
 *
 * Created: 11/30/2015 10:17:22 PM
 *  Author: julius
 */ 
#include "QUTMS_SteeringWheel_v0_1.h"

int main(void)
{
	DDRB = 0b11111111;
	DDRC = 0b11100000;
	DDRD = 0b10000001;
	
    while(1)
    {
		OLED_Initialise();
		_delay_ms(500);
		//PORTB ^= 1<<PINB7;
        //TODO:: Please write your application code 
    }
}