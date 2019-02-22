
#include "OLED_v0_1.h"

void OLED_DataBus(uint8_t data)
{
	uint8_t TempByte = (((data&1)>>0)<<OLED_D0) | (((data&2)>>1)<<OLED_D1) | (((data&4)>>2)<<OLED_D2) | (((data&16)>>4)<<OLED_D4) | (((data&128)>>7)<<OLED_D7);
	PORTB = PORTB&OLED_PBMASK;
	PORTB |= TempByte;
	
	TempByte = (((data&8)>>3)<<OLED_D3);
	PORTD = PORTD&OLED_PDMASK;
	PORTD |= TempByte;
	
	TempByte = (((data&32)>>5)<<OLED_D5) | (data&64<<OLED_D6);
	PORTC = PORTC&OLED_PCMASK;
	PORTC |= TempByte;
}

void OLED_WriteInstruction(uint8_t instruction)
{
	PORTB &= ~(1<<OLED_DC);
	PORTB &= ~(1<<OLED_RESET);
	PORTD &= ~(1<<OLED_CS);
	PORTC &=~ (1<<OLED_WR);
	OLED_DataBus(instruction);
	PORTC |= (1<<OLED_WR);
	PORTD |= (1<<OLED_CS);
	return;
}

void OLED_WriteData(uint8_t data)
{
	
	PORTB |= (1<<OLED_RESET);
	PORTD &= ~(1<<OLED_CS);
	PORTC &=~ (1<<OLED_WR);
	OLED_DataBus(data);
	PORTC |= (1<<OLED_WR);
	PORTD |= (1<<OLED_CS);
	
	return;
}
void OLED_ClearRam()
{	
	uint8_t x,y;
	OLED_WriteInstruction(0x15);
	OLED_WriteData(0x00);
	OLED_WriteData(0x77);
	OLED_WriteInstruction(0x75);
	OLED_WriteData(0x00);
	OLED_WriteData(0x7f);
	OLED_WriteInstruction(0x5C);
	for(y=0;y<128;y++)
	{ for(x=0;x<120;x++)
		{   
			OLED_WriteData(0x00);
		}
	}
}

void OLED_Initialise()
{
	
		OLED_RESETP |= (1<<OLED_RESET);
		_delay_us(2000);
		OLED_RESETP &= ~(1<<OLED_RESET);
		_delay_us(2000);
		OLED_RESETP |= (1<<OLED_RESET);
		OLED_WriteInstruction(0xFD); //Set Command Lock

		
		OLED_WriteInstruction(0xFD); /*SET COMMAND LOCK*/
		OLED_WriteData(0x12); /* UNLOCK */
		OLED_WriteInstruction(0xAE); /*DISPLAY OFF*/
		OLED_WriteInstruction(0xB3);/*DISPLAYDIVIDE CLOCKRADIO/OSCILLATAR FREQUANCY*/
		OLED_WriteData(0x91); OLED_WriteInstruction(0xCA); /*multiplex ratio*/
		OLED_WriteData(0x3F); /*duty = 1/64*/
		OLED_WriteInstruction(0xA2); /*set offset*/
		OLED_WriteData(0x00);
		OLED_WriteInstruction(0xA1); /*start line*/
		OLED_WriteData(0x00);
		OLED_WriteInstruction(0xA0); /*set remap*/
		OLED_WriteData(0x14);
		OLED_WriteData(0x11);
		/*OLED_WriteInstruction(0xB5); //GPIO OLED_WriteInstruction(0x00); */
		OLED_WriteInstruction(0xAB); /*funtion selection*/
		OLED_WriteData(0x01); /* selection external vdd */
		OLED_WriteInstruction(0xB4); /* */
		OLED_WriteData(0xA0);
		OLED_WriteData(0xfd);
		OLED_WriteInstruction(0xC1); /*set contrast current */
		OLED_WriteData(OLED_CONTRAST);
		OLED_WriteInstruction(0xC7); /*master contrast current control*/
		OLED_WriteData(0x0f);
		/*OLED_WriteInstruction(0xB9); GRAY TABLE*/
		OLED_WriteInstruction(0xB1); /*SET PHASE LENGTH*/
		OLED_WriteData(0xE2);
		OLED_WriteInstruction(0xD1); /**/
		OLED_WriteData(0x82);
		OLED_WriteData(0x20);
		OLED_WriteInstruction(0xBB); /*SET PRE-CHANGE VOLTAGE*/
		OLED_WriteData(0x1F);
		OLED_WriteInstruction(0xB6); /*SET SECOND PRE-CHARGE PERIOD*/
		OLED_WriteData(0x08);
		OLED_WriteInstruction(0xBE); /* SET VCOMH */
		OLED_WriteData(0x07);
		OLED_WriteInstruction(0xA6); /*normal display*/
		OLED_ClearRam();
		OLED_WriteInstruction(0xA5); /*display ON*/
}