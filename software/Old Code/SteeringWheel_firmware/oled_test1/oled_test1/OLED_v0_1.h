/*
 * OLED_v0_1.h
 *
 * Created: 12/1/2015 1:13:44 PM
 *  Author: julius
 */ 
#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#ifndef OLED_V0_1_H_
#define OLED_V0_1_H_

#include <avr/io.h>
#include <util/delay.h>

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Global Defs
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#define	Shift		0x1C
#define Max_Column	0x3F			// 256/4-1
#define Max_Row		0x3F			// 64-1
#define	Brightness	0x0F


#define OLED_D0 PINB7
#define OLED_D1 PINB0
#define OLED_D2 PINB1
#define OLED_D3 PIND7
#define OLED_D4 PINB2
#define OLED_D5 PINC5
#define OLED_D6 PINC6
#define OLED_D7 PINB3

#define OLED_PBMASK 0b01110000
#define OLED_PCMASK 0b10011111
#define OLED_PDMASK 0b01111111

#define OLED_RD PINB4
#define OLED_WR PINC7
#define OLED_DC PINB5
#define OLED_RESET PINB6
#define OLED_RESETP PORTB
#define OLED_CS PIND0

#define OLED_CONTRAST 0xA0

void OLED_Initialise(void);

#endif /* OLED_V0_1_H_ */