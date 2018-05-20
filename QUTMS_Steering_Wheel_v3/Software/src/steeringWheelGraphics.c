/*****************************************************************************
* @file    Software/src/steeringWheelGraphics.c 
* @author  Zoe Goodward
* @version V1.0.0
* @date    28-April-2018
* @brief   Functions for creating graphics on the OLED display
*****************************************************************************/

#include "steeringWheelGraphics.h"

/*============================================================================
Function:   draw_rectangle()
------------------------------------------------------------------------------
Purpose :   create the outline of a rectangle on the screen
Input   :   a - line width
			b - column address of start
			c - column address of end (total columns divided by 4)
			d - row address of start
			e - row address of end
Returns :   void
Notes   :
============================================================================*/
void draw_rectangle(unsigned char data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e)
{
	unsigned char i,j,k,l;

	k=a%4;
	if(k == 0)
	{
		l=(a/4)-1;
	}
	else
	{
		l=a/4;
	}

	set_column_address(Shift+b,Shift+c);
	set_row_address(d,(d+a-1));
	set_write_ram();
	for(i=0;i<(c-b+1);i++)
	{
		for(j=0;j<a;j++)
		{
			write_data(data);
			write_data(data);
		}
	}

	set_column_address(Shift+(c-l),Shift+c);
	set_row_address(d+a,e-a);
	set_write_ram();
	for(i=0;i<(e-d+1);i++)
	{
		for(j=0;j<(l+1);j++)
		{
			if(j == 0)
			{
				switch(k)
				{
					case 0:
					write_data(data);
					write_data(data);
					break;
					case 1:
					write_data(0x00);
					write_data(data&0x0F);
					break;
					case 2:
					write_data(0x00);
					write_data(data);
					break;
					case 3:
					write_data(data&0x0F);
					write_data(data);
					break;
				}
			}
			else
			{
				write_data(data);
				write_data(data);
			}
		}
	}

	set_column_address(Shift+b,Shift+c);
	set_row_address((e-a+1),e);
	set_write_ram();
	for(i=0;i<(c-b+1);i++)
	{
		for(j=0;j<a;j++)
		{
			write_data(data);
			write_data(data);
		}
	}

	set_column_address(Shift+b,Shift+(b+l));
	set_row_address(d+a,e-a);
	set_write_ram();
	for(i=0;i<(e-d+1);i++)
	{
		for(j=0;j<(l+1);j++)
		{
			if(j == l)
			{
				switch(k)
				{
					case 0:
					write_data(data);
					write_data(data);
					break;
					case 1:
					write_data(data&0xF0);
					write_data(0x00);
					break;
					case 2:
					write_data(data);
					write_data(0x00);
					break;
					case 3:
					write_data(data);
					write_data(data&0xF0);
					break;
				}
			}
			else
			{
				write_data(data);
				write_data(data);
			}
		}
	}
}

/*============================================================================
Function:   fill_ram()
------------------------------------------------------------------------------
Purpose :   fill the screen with the value of the variable: data
Input   :   unsigned char data - use macros CLEAR_SCREEN or FILL_SCREEN
Returns :   none
Notes   :
============================================================================*/
void fill_ram(unsigned char data)
{
	unsigned char i,j;

	set_column_address(0x00,0x77);
	set_row_address(0x00,0x7F);
	set_write_ram();

	for(i=0;i<128;i++) // i < SCREEN_WIDTH / 2
	{
		for(j=0;j<120;j++)
		{
			write_data(data);
			//write_data(data);
		}
	}
}

/*============================================================================
Function:   fill_block()
------------------------------------------------------------------------------
Purpose :   create a filled in rectangle on the screen
Input   :   data - 
			a - column address of start
			b - column address of end (total columns divided by 4)
			c - row address of start
			d - row address of end
Returns :   void
Notes   :
============================================================================*/
void fill_block(unsigned char data, unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
	unsigned char i,j;
	
	set_column_address(Shift+a,Shift+b);
	set_row_address(c,d);
	set_write_ram();

	for(i=0;i<(d-c+1);i++)
	{
		for(j=0;j<(b-a+1);j++)
		{
			write_data(data);
			write_data(data);
		}
	}
}

/*============================================================================
Function:   checkerboard()
------------------------------------------------------------------------------
Purpose :   display a checkerboard pattern across the screen
Input   :   none
Returns :   void
Notes   :
============================================================================*/
void checkerboard()
{
	unsigned char i,j;
	
	set_column_address(0x00,0x77);
	set_row_address(0x00,0x7F);
	set_write_ram();

	for(i=0;i<64;i++)
	{
		for(j=0;j<120;j++)
		{
			write_data(0xF0);
			write_data(0xF0);
		}
		for(j=0;j<120;j++)
		{
			write_data(0x0F);
			write_data(0x0F);
		}
	}
}

/*============================================================================
Function:   grayscale()
------------------------------------------------------------------------------
Purpose :   display a gray scale bar across the screen
Input   :   none
Returns :   void
Notes   :
============================================================================*/
void grayscale()
{
	// Level 16 => Column 1~16
	fill_block(0xFF,0x00,0x03,0x00,Max_Row);
	// Level 15 => Column 17~32
	fill_block(0xEE,0x04,0x07,0x00,Max_Row);
	// Level 14 => Column 33~48
	fill_block(0xDD,0x08,0x0B,0x00,Max_Row);
	// Level 13 => Column 49~64
	fill_block(0xCC,0x0C,0x0F,0x00,Max_Row);
	// Level 12 => Column 65~80
	fill_block(0xBB,0x10,0x13,0x00,Max_Row);
	// Level 11 => Column 81~96
	fill_block(0xAA,0x14,0x17,0x00,Max_Row);
	// Level 10 => Column 97~112
	fill_block(0x99,0x18,0x1B,0x00,Max_Row);
	// Level 9 => Column 113~128
	fill_block(0x88,0x1C,0x1F,0x00,Max_Row);
	// Level 8 => Column 129~144
	fill_block(0x77,0x20,0x23,0x00,Max_Row);
	// Level 7 => Column 145~160
	fill_block(0x66,0x24,0x27,0x00,Max_Row);
	// Level 6 => Column 161~176
	fill_block(0x55,0x28,0x2B,0x00,Max_Row);
	// Level 5 => Column 177~192
	fill_block(0x44,0x2C,0x2F,0x00,Max_Row);
	// Level 4 => Column 193~208
	fill_block(0x33,0x30,0x33,0x00,Max_Row);
	// Level 3 => Column 209~224
	fill_block(0x22,0x34,0x37,0x00,Max_Row);
	// Level 2 => Column 225~240
	fill_block(0x11,0x38,0x3B,0x00,Max_Row);
	// Level 1 => Column 241~256
	fill_block(0x00,0x3C,Max_Column,0x00,Max_Row);
}

/*============================================================================
Function:   show_font_5x7()
------------------------------------------------------------------------------
Purpose :   displays a 5x7 character
Input   :   a - database
			b - Ascii
			c - start x address
			d - start y address
Returns :   void
Notes   :
============================================================================*/
void show_font_5x7(unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
	if(b == ' ')
	{
		b = 0x60;
	}
	else
	{
		b-=0x20;
	}
		
	unsigned char *Src_Pointer;
	unsigned char i,Font,MSB_1,LSB_1,MSB_2,LSB_2;
	unsigned char derp = d + 16;

	switch(a)
	{
		case 1:
		Src_Pointer=(&(Ascii_2[(b-1)][0]));
		break;
		//case 2:
		//Src_Pointer=&Ascii_2[(b-1)][0];
		//break;
	}

	set_remap_format(0x15);
	for(i=0;i<=1;i++)
	{
		MSB_1= *Src_Pointer;
		Src_Pointer++;
		if(i == 1)
		{
			LSB_1=0x00;
			MSB_2=0x00;
			LSB_2=0x00;
		}
		else
		{
			LSB_1=*Src_Pointer;
			Src_Pointer++;
			MSB_2=*Src_Pointer;
			Src_Pointer++;
			LSB_2=*Src_Pointer;
			Src_Pointer++;
		}
		set_column_address(Shift+c,Shift+c);
		set_row_address(d,d+7);
		set_write_ram();
		
		Font=((MSB_1&0x01)<<4)|(LSB_1&0x01);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3);
		write_data(Font);
		Font=((MSB_2&0x01)<<4)|(LSB_2&0x01);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3);
		write_data(Font);
		
		Font=((MSB_1&0x02)<<3)|((LSB_1&0x02)>>1);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3);
		write_data(Font);
		Font=((MSB_2&0x02)<<3)|((LSB_2&0x02)>>1);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3);
		write_data(Font);
		
		Font=((MSB_1&0x04)<<2)|((LSB_1&0x04)>>2);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3);
		write_data(Font);
		Font=((MSB_2&0x04)<<2)|((LSB_2&0x04)>>2);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3);
		write_data(Font);
		
		Font=((MSB_1&0x08)<<1)|((LSB_1&0x08)>>3);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3);
		write_data(Font);
		Font=((MSB_2&0x08)<<1)|((LSB_2&0x08)>>3);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3);
		write_data(Font);
		
		Font=((MSB_1&0x10)<<3)|((LSB_1&0x10)>>1);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3);
		write_data(Font);
		Font=((MSB_2&0x10)<<3)|((LSB_2&0x10)>>1);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3);
		write_data(Font);

		Font=((MSB_1&0x20)<<2)|((LSB_1&0x20)>>2);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3);
		write_data(Font);
		Font=((MSB_2&0x20)<<2)|((LSB_2&0x20)>>2);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3);
		write_data(Font);

		Font=((MSB_1&0x40)<<1)|((LSB_1&0x40)>>3);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3);
		write_data(Font);
		Font=((MSB_2&0x40)<<1)|((LSB_2&0x40)>>3);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3);
		write_data(Font);

		Font=(MSB_1&0x80)|((LSB_1&0x80)>>4);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3);
		write_data(Font);
		Font=(MSB_2&0x80)|((LSB_2&0x80)>>4);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3);
		write_data(Font);
		
		//set_row_address(derp,derp+7);
		//set_write_ram();

		/*

		Font=((MSB_1&0x01)<<4)|(LSB_1&0x01);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3)|(Font<<4)|(Font<<5);
		write_data(Font);
		Font=((MSB_2&0x01)<<4)|(LSB_2&0x01);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3)|(Font<<4)|(Font<<5);
		write_data(Font);
		
		Font=((MSB_1&0x02)<<3)|((LSB_1&0x02)>>1);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3)|(Font<<4)|(Font<<5);
		write_data(Font);
		Font=((MSB_2&0x02)<<3)|((LSB_2&0x02)>>1);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3)|(Font<<4)|(Font<<5);
		write_data(Font);
		
		Font=((MSB_1&0x04)<<2)|((LSB_1&0x04)>>2);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3)|(Font<<4)|(Font<<5);
		write_data(Font);
		Font=((MSB_2&0x04)<<2)|((LSB_2&0x04)>>2);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3)|(Font<<4)|(Font<<5);
		write_data(Font);
		
		Font=((MSB_1&0x08)<<1)|((LSB_1&0x08)>>3);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3)|(Font<<4)|(Font<<5);
		write_data(Font);
		Font=((MSB_2&0x08)<<1)|((LSB_2&0x08)>>3);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3)|(Font<<4)|(Font<<5);
		write_data(Font);
		
		Font=((MSB_1&0x10)<<3)|((LSB_1&0x10)>>1);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3)|(Font>>4)|(Font>>5);
		write_data(Font);
		Font=((MSB_2&0x10)<<3)|((LSB_2&0x10)>>1);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3)|(Font>>4)|(Font>>5);
		write_data(Font);

		Font=((MSB_1&0x20)<<2)|((LSB_1&0x20)>>2);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3)|(Font>>4)|(Font>>5);
		write_data(Font);
		Font=((MSB_2&0x20)<<2)|((LSB_2&0x20)>>2);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3)|(Font>>4)|(Font>>5);
		write_data(Font);

		Font=((MSB_1&0x40)<<1)|((LSB_1&0x40)>>3);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3)|(Font>>4)|(Font>>5);
		write_data(Font);
		Font=((MSB_2&0x40)<<1)|((LSB_2&0x40)>>3);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3)|(Font>>4)|(Font>>5);
		write_data(Font);

		Font=(MSB_1&0x80)|((LSB_1&0x80)>>4);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3)|(Font>>4)|(Font>>5);
		write_data(Font);
		Font=(MSB_2&0x80)|((LSB_2&0x80)>>4);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3)|(Font>>4)|(Font>>5);
		write_data(Font);
		
		*/
		
		c++;
	}
	set_remap_format(0x14);
	
}

/*============================================================================
Function:   show_char()
------------------------------------------------------------------------------
Purpose :   displays character on screen
Input   :   a - database
			b - start x address
			c - start y address
Returns :   void
Notes   :	* Must write "0" in the end...
============================================================================*/
void show_char(unsigned char a, unsigned char data, unsigned char b, unsigned char c)
{
	//unsigned char *Src_Pointer;

	//Src_Pointer=data_pointer;
	show_font_5x7(1,96,b,c);			// No-Break Space
	//   Must be written first before the string start...

	show_font_5x7(a,data,b,c);
	//while(1)
	//{
		//show_font_5x7(a,*Src_Pointer,b,c);
		//Src_Pointer++;
		//b+=2;
		//if(*Src_Pointer == 0) break;
	//}
}

/*============================================================================
Function:   show_huge_char()
------------------------------------------------------------------------------
Purpose :   displays character on screen
Input   :   data - 
			a - column address of start
			b - column address of end (total columns divided by 4)
			c - row address of start
			d - row address of end

Returns :   void
Notes   :	* Must write "0" in the end...
============================================================================*/
void show_huge_char(unsigned char data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char LwrNibbleBrightness, unsigned char UppNibbleBrightness)
{
	set_column_address(Shift+a,Shift+b);
	set_row_address(c,c+38);
	set_write_ram();
	int counter = 0;
	int i = 0;
	
	for(i = 0; i < 38; i++)
	{
		//the main for loop counts through 0 to 5
		//we double each number we use here as it's all in pairs
		//so the first time we will look at (p*2) and (p*2)+1
		//if p is 0 initially these work out to 0 and 1
		//next time p will be 1 so (p*2) and (p*2)+1 will give us 2 and 3 and so on through to 10 and 11 when p = 5 at the last run
		for(int p=0;p<14;p++)
		{
			//temp variable we use each time to build each pair of pixels up
			//we set it to zero initially because until theres a pixel(s) to draw we dont want to
			uint8_t tempNibbles = 0;
			
			//here we test if we should be looking at the first byte in the row or the second
			//since the real number we are looking at is p*2 each time we test against 4 not 8
			if(p < 4)
			{
				
				//it got to here because p was less than 4
				//this means that we should be looking in the first byte of each row
				//so in the bit test blow you can see the second dimension is zero in both cases
				//so we shift a 1 n times along so we can then test to see if it's a 1 or a 0 in the font
				//for example the first time it runs p = 0
				//so the test shifts 1 to the left (((3-0)*2)+1) times or 7 times to the left
				//this then ANDS the first byte in the font row with 0b10000000
				//if the result is non zero it should be a pixel so tempNibbles gets adjusted accordingly
				if(((1 << (((3-p)*2)+1) ) & (pgm_read_byte(&HugeText[data-0x20][i*4]))) > 0)
				{
					tempNibbles += UppNibbleBrightness;
				}
				
				//this does the same test except it drops the plus 1
				//so the first test is ((3-p)*2) or 6 shifts left
				//it therefore compares the second byte in the font row with 0b01000000
				//it then sets the lower nibble on if there was a pixel match
				if(((1 << ((3-p)*2) & (pgm_read_byte(&HugeText[data-0x20][i*4]))) > 0))
				{
					tempNibbles += LwrNibbleBrightness;
				}
			}
			
			if((p > 3) && (p < 8))
			{
				if(((1 << (((7-p)*2)+1) & (pgm_read_byte(&HugeText[data-0x20][((i*4)+1)]))) > 0))
				{
					tempNibbles += UppNibbleBrightness;
				}
				
				//this does the same test except it drops the plus 1
				//so the first test is ((3-p)*2) or 6 shifts left
				//it therefore compares the second byte in the font row with 0b01000000
				//it then sets the lower nibble on if there was a pixel match
				if(((1 << ((7-p)*2) & (pgm_read_byte(&HugeText[data-0x20][((i*4)+1)]))) > 0))
				{
					tempNibbles += LwrNibbleBrightness;
				}
			}
			
			if((p > 7) && (p < 12))
			{
				if(((1 << (((11-p)*2)+1) & (pgm_read_byte(&HugeText[data-0x20][((i*4)+2)]))) > 0))
				{
					tempNibbles += UppNibbleBrightness;
				}
				
				//this does the same test except it drops the plus 1
				//so the first test is ((3-p)*2) or 6 shifts left
				//it therefore compares the second byte in the font row with 0b01000000
				//it then sets the lower nibble on if there was a pixel match
				if(((1 << ((11-p)*2) & (pgm_read_byte(&HugeText[data-0x20][((i*4)+2)]))) > 0))
				{
					tempNibbles += LwrNibbleBrightness;
				}
			}
			
			if((p > 11) && (p < 16))
			{
				if(((1 << (((15-p)*2)+1) & (pgm_read_byte(&HugeText[data-0x20][((i*4)+3)]))) > 0))
				{
					tempNibbles += UppNibbleBrightness;
				}
				
				//this does the same test except it drops the plus 1
				//so the first test is ((3-p)*2) or 6 shifts left
				//it therefore compares the second byte in the font row with 0b01000000
				//it then sets the lower nibble on if there was a pixel match
				if(((1 << ((15-p)*2) & (pgm_read_byte(&HugeText[data-0x20][((i*4)+3)]))) > 0))
				{
					tempNibbles += LwrNibbleBrightness;
				}
			}
			
			//Now we have a temporary 8 bit number with 2 pixels embedded in it we can send it to the screen
			write_data(tempNibbles);
		}
		set_column_address(Shift+a,Shift+b);
		set_row_address(c+i+1,c+38);
		set_write_ram();
	}
}
void show_bigger_char(unsigned char data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char LwrNibbleBrightness, unsigned char UppNibbleBrightness)
{
	set_column_address(Shift+a,Shift+b);
	set_row_address(c,c+25);
	set_write_ram();
	int counter = 0;
	int i = 0;
	
	for(i = 0; i < 25; i++)
	{
		//the main for loop counts through 0 to 5
		//we double each number we use here as it's all in pairs
		//so the first time we will look at (p*2) and (p*2)+1
		//if p is 0 initially these work out to 0 and 1
		//next time p will be 1 so (p*2) and (p*2)+1 will give us 2 and 3 and so on through to 10 and 11 when p = 5 at the last run
		for(int p=0;p<8;p++)
		{
			//temp variable we use each time to build each pair of pixels up
			//we set it to zero initially because until theres a pixel(s) to draw we dont want to
			uint8_t tempNibbles = 0;
			
			//here we test if we should be looking at the first byte in the row or the second
			//since the real number we are looking at is p*2 each time we test against 4 not 8
			if(p < 4)
			{
				
				//it got to here because p was less than 4
				//this means that we should be looking in the first byte of each row
				//so in the bit test blow you can see the second dimension is zero in both cases
				//so we shift a 1 n times along so we can then test to see if it's a 1 or a 0 in the font
				//for example the first time it runs p = 0
				//so the test shifts 1 to the left (((3-0)*2)+1) times or 7 times to the left
				//this then ANDS the first byte in the font row with 0b10000000
				//if the result is non zero it should be a pixel so tempNibbles gets adjusted accordingly
				if(((1 << (((3-p)*2)+1) ) & (pgm_read_byte(&BiggerText[data-0x20][i*2]))) > 0)
				{
					tempNibbles += UppNibbleBrightness;
				}
				
				//this does the same test except it drops the plus 1
				//so the first test is ((3-p)*2) or 6 shifts left
				//it therefore compares the second byte in the font row with 0b01000000
				//it then sets the lower nibble on if there was a pixel match
				if(((1 << ((3-p)*2) & (pgm_read_byte(&BiggerText[data-0x20][i*2]))) > 0))
				{
					tempNibbles += LwrNibbleBrightness;
				}
			}
			
			if((p > 3) && (p < 8))
			{
				if(((1 << (((7-p)*2)+1) & (pgm_read_byte(&BiggerText[data-0x20][((i*2)+1)]))) > 0))
				{
					tempNibbles += UppNibbleBrightness;
				}
				
				//this does the same test except it drops the plus 1
				//so the first test is ((3-p)*2) or 6 shifts left
				//it therefore compares the second byte in the font row with 0b01000000
				//it then sets the lower nibble on if there was a pixel match
				if(((1 << ((7-p)*2) & (pgm_read_byte(&BiggerText[data-0x20][((i*2)+1)]))) > 0))
				{
					tempNibbles += LwrNibbleBrightness;
				}
			}
			
			else
			{
				if(((1 << (((8-p)*2)+1) & (pgm_read_byte(&BiggerText[data-0x20][((i*2)+1)]))) > 0))
				{
					tempNibbles += UppNibbleBrightness;
				}
				
				//this does the same test except it drops the plus 1
				//so the first test is ((3-p)*2) or 6 shifts left
				//it therefore compares the second byte in the font row with 0b01000000
				//it then sets the lower nibble on if there was a pixel match
				if(((1 << ((8-p)*2) & (pgm_read_byte(&BiggerText[data-0x20][((i*2)+1)]))) > 0))
				{
					tempNibbles += LwrNibbleBrightness;
				}
			}
			
			//Now we have a temporary 8 bit number with 2 pixels embedded in it we can send it to the screen
			write_data(tempNibbles);
		}
		set_column_address(Shift+a,Shift+b);
		set_row_address(c+i+1,c+30);
		set_write_ram();
	}
}

void show_big_char(unsigned char data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char LwrNibbleBrightness, unsigned char UppNibbleBrightness)
{
	set_column_address(Shift+a,Shift+b);
	set_row_address(c,c+17);
	set_write_ram();
	int counter = 0;
	int i = 0;
	
	for(i = 0; i < 17; i++)
	{
		//the main for loop counts through 0 to 5
		//we double each number we use here as it's all in pairs
		//so the first time we will look at (p*2) and (p*2)+1
		//if p is 0 initially these work out to 0 and 1
		//next time p will be 1 so (p*2) and (p*2)+1 will give us 2 and 3 and so on through to 10 and 11 when p = 5 at the last run
		for(int p=0;p<7;p++)
		{
			//temp variable we use each time to build each pair of pixels up
			//we set it to zero initially because until theres a pixel(s) to draw we dont want to
			uint8_t tempNibbles = 0;
			
			//here we test if we should be looking at the first byte in the row or the second
			//since the real number we are looking at is p*2 each time we test against 4 not 8
			if(p < 4)
			{
				
				//it got to here because p was less than 4
				//this means that we should be looking in the first byte of each row
				//so in the bit test blow you can see the second dimension is zero in both cases
				//so we shift a 1 n times along so we can then test to see if it's a 1 or a 0 in the font
				//for example the first time it runs p = 0
				//so the test shifts 1 to the left (((3-0)*2)+1) times or 7 times to the left
				//this then ANDS the first byte in the font row with 0b10000000
				//if the result is non zero it should be a pixel so tempNibbles gets adjusted accordingly
				if(((1 << (((3-p)*2)+1) ) & (pgm_read_byte(&BigText[data-0x20][i*2]))) > 0)
				{
					tempNibbles += UppNibbleBrightness;
				}
				
				//this does the same test except it drops the plus 1
				//so the first test is ((3-p)*2) or 6 shifts left
				//it therefore compares the second byte in the font row with 0b01000000
				//it then sets the lower nibble on if there was a pixel match
				if(((1 << ((3-p)*2) & (pgm_read_byte(&BigText[data-0x20][i*2]))) > 0))
				{
					tempNibbles += LwrNibbleBrightness;
				}
			}
			
			//it comes here if p has gotten big enough that we are no longer looking at the first byte in the font row
			//since the indexing term (p*2) is now getting bigger than a byte, we have to adjust it accordingly
			//therefore in the array indexing of BigText the second dimension is now a 1 not a 0 as above
			//we offset by 7 now not 3 like we did above
			//this means that on the first pass through for example, when p would be 4
			//the left bit shift is (((7-4)*2)+1) which is 7 so we get to AND against 0b10000000 against
			//only this time on the second byte in the font row
			else
			{
				if(((1 << (((7-p)*2)+1) & (pgm_read_byte(&BigText[data-0x20][((i*2)+1)]))) > 0))
				{
					tempNibbles += UppNibbleBrightness;
				}
				
				//this does the same test except it drops the plus 1
				//so the first test is ((3-p)*2) or 6 shifts left
				//it therefore compares the second byte in the font row with 0b01000000
				//it then sets the lower nibble on if there was a pixel match
				if(((1 << ((7-p)*2) & (pgm_read_byte(&BigText[data-0x20][((i*2)+1)]))) > 0))
				{
					tempNibbles += LwrNibbleBrightness;
				}
			}
			
			//Now we have a temporary 8 bit number with 2 pixels embedded in it we can send it to the screen
			write_data(tempNibbles);
		}
		set_column_address(Shift+a,Shift+b);
		set_row_address(c+i+1,c+17);
		set_write_ram();
	}
}

/*============================================================================
Function:   show_string()
------------------------------------------------------------------------------
Purpose :   displays a string on screen
Input   :   a - database
			b - start x address
			c - start y address
Returns :   void
Notes   :	* Must write "0" in the end...
============================================================================*/
void show_string(unsigned char a, unsigned char *data_pointer, unsigned char b, unsigned char c)
{
	unsigned char *Src_Pointer;

	Src_Pointer=data_pointer;
	show_font_5x7(1,96,b,c);			// No-Break Space
	//   Must be written first before the string start...

	while(1)
	{
		show_font_5x7(a,*Src_Pointer,b,c);
		Src_Pointer++;
		b+=2;
		if(*Src_Pointer == 0) break;
	}
}

void show_huge_string(unsigned char *data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned int length,  unsigned char LwrNibbleBrightness, unsigned char UppNibbleBrightness)
{
	for(int i = 0; i < length; i++)
	{
		show_huge_char(*data,a,b,c,d, LwrNibbleBrightness, UppNibbleBrightness);
		data++;
		a+=7;
		b+=7;
	}
}

void show_bigger_string(unsigned char *data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned int length,  unsigned char LwrNibbleBrightness, unsigned char UppNibbleBrightness)
{
	for(int i = 0; i < length; i++)
	{
		show_bigger_char(*data,a,b,c,d, LwrNibbleBrightness, UppNibbleBrightness);
		data++;
		a+=4;
		b+=4;
	}
}

void show_big_string(unsigned char *data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned int length, unsigned char LwrNibbleBrightness, unsigned char UppNibbleBrightness)
{
	for(int i = 0; i < length; i++)
	{
		show_big_char(*data,a,b,c,d, LwrNibbleBrightness, UppNibbleBrightness);
		data++;
		a+=3;
		b+=3;
	}
}

/*============================================================================
Function:   show_pattern()
------------------------------------------------------------------------------
Purpose :   displays a 
Input   :   *data_pointer - 
			a - column address of start
			b - column address of end (total columns divided by 4)
			c - row address of start
			d - row address of end
Returns :   void
Notes   :	* Must write "0" in the end...
============================================================================*/
void show_pattern(unsigned char *data_pointer, unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
	unsigned char *src_pointer;
	unsigned char i,j;
	
	src_pointer=data_pointer;
	set_column_address(Shift+a,Shift+b);
	set_row_address(c,d);
	set_write_ram();

	for(i=0;i<(d-c+1);i++)
	{
		for(j=0;j<(b-a+1);j++)
		{
			write_data(*src_pointer);
			src_pointer++;
			write_data(*src_pointer);
			src_pointer++;
		}
	}
}

/*============================================================================
Function:   data_processing()
------------------------------------------------------------------------------
Purpose :   turns 1 byte of B/W data into 4 byte gray data
Input   :   unsigned char temp - 
Returns :   return
Notes   :
============================================================================*/
void data_processing(unsigned char temp)
{
	unsigned char temp1,temp2,temp3,temp4,temp5,temp6,temp7,temp8;
	unsigned char h11,h12,h13,h14,h15,h16,h17,h18,d1,d2,d3,d4;

	temp1=temp&0x80;
	temp2=(temp&0x40)>>3;
	temp3=(temp&0x20)<<2;
	temp4=(temp&0x10)>>1;
	temp5=(temp&0x08)<<4;
	temp6=(temp&0x04)<<1;
	temp7=(temp&0x02)<<6;
	temp8=(temp&0x01)<<3;
	h11=temp1|temp1>>1|temp1>>2|temp1>>3;
	h12=temp2|temp2>>1|temp2>>2|temp2>>3;
	h13=temp3|temp3>>1|temp3>>2|temp3>>3;
	h14=temp4|temp4>>1|temp4>>2|temp4>>3;
	h15=temp5|temp5>>1|temp5>>2|temp5>>3;
	h16=temp6|temp6>>1|temp6>>2|temp6>>3;
	h17=temp7|temp7>>1|temp7>>2|temp7>>3;
	h18=temp8|temp8>>1|temp8>>2|temp8>>3;
	d1=h11|h12;
	d2=h13|h14;
	d3=h15|h16;
	d4=h17|h18;

	write_data(d1);
	write_data(d2);
	write_data(d3);
	write_data(d4);
}

/*============================================================================
Function:   display_picture()
------------------------------------------------------------------------------
Purpose :   displays a bitmap image on the OLED screen
Input   :   unsigned char pic[] - an array of hexidecimal values
Returns :   return
Notes   :
============================================================================*/
void display_picture(unsigned char pic[])
{
	unsigned char i,j;
	set_column_address(Shift+0x00,Shift+0x77);
	set_row_address(0x00,0x7F);
	set_write_ram();

	for(i=0;i<64;i++)
	{
		for(j=0;j<60;j++)
		{
			data_processing(pgm_read_byte(&pic[i*32+j]));
		}
	}
	return;
}

/*============================================================================
Function:   vertical_scroll()
------------------------------------------------------------------------------
Purpose :   creates vertical scrolling effect
Input   :   a - scrolling direction (use macros UPWARD or DOWNWARD)
			b - set number of row scroll per step
			c - set time interval between each scroll step
Returns :   void
Notes   :
============================================================================*/
void vertical_scroll(unsigned char a, unsigned char b, unsigned char c)
{
	unsigned char i,j;

	set_partial_display(0x00,0x00,Max_Row);

	switch(a)
	{
		case 0:
		for(i=0;i<(Max_Row+1);i+=b)
		{
			set_display_offset(i+1);
			for(j=0;j<c;j++)
			{
				uDelay(1600);
			}
		}
		break;
		case 1:
		for(i=0;i<(Max_Row+1);i+=b)
		{
			set_display_offset(Max_Row-i);
			for(j=0;j<c;j++)
			{
				uDelay(1600);
			}
		}
		break;
	}
	set_partial_display(0x01,0x00,0x00);
}

/*============================================================================
Function:   fade_scroll()
------------------------------------------------------------------------------
Purpose :   creates vertical fade scrolling effect
Input   :   a - scrolling direction (use macros UPWARD or DOWNWARD (for inward
			direction) or UPWARD_OUT or _DOWNWARD_OUT)
			b - set number of row scroll per step
			c - set time interval between each scroll step
Returns :   void
Notes   :
============================================================================*/
void fade_scroll(unsigned char a, unsigned char b, unsigned char c)
{
	unsigned char i,j;

	set_partial_display(0x00,0x00,Max_Row);

	switch(a)
	{
		case 0:
		for(i=(Max_Row+1);i<128;i+=b)
		{
			set_start_line(i);
			for(j=0;j<c;j++)
			{
				uDelay(200);
			}
		}
		set_start_line(0x00);
		for(j=0;j<c;j++)
		{
			uDelay(200);
		}
		break;
		case 1:
		for(i=0;i<(Max_Row+1);i+=b)
		{
			set_start_line(Max_Row-i);
			for(j=0;j<c;j++)
			{
				uDelay(200);
			}
		}
		break;
		case 2:
		for(i=0;i<(Max_Row+1);i+=b)
		{
			set_start_line(i+1);
			for(j=0;j<c;j++)
			{
				uDelay(200);
			}
		}
		break;
		case 3:
		for(i=127;i>Max_Row;i-=b)
		{
			set_start_line(i);
			for(j=0;j<c;j++)
			{
				uDelay(200);
			}
		}
		break;
	}
	set_partial_display(0x01,0x00,0x00);
}

/*============================================================================
Function:   fade_in()
------------------------------------------------------------------------------
Purpose :   creates a fade in effect (full screen)
Input   :   none
Returns :   void
Notes   :
============================================================================*/
void fade_in()
{
	unsigned char i;

	set_display_on_off(0x01);
	for(i=0;i<(Brightness+1);i++)
	{
		set_master_current(i);
		uDelay(200);
		uDelay(200);
		uDelay(200);
	}
}

/*============================================================================
Function:   fade_out()
------------------------------------------------------------------------------
Purpose :   creates a fade out effect (full screen)
Input   :   none
Returns :   void
Notes   :
============================================================================*/
void fade_out()
{
	unsigned char i;

	for(i=(Brightness+1);i>0;i--)
	{
		set_master_current(i-1);
		uDelay(200);
		uDelay(200);
		uDelay(200);
	}
	set_display_on_off(0x00);
}

/*============================================================================
Function:   set_gray_scale_table()
------------------------------------------------------------------------------
Purpose :   Set gray scale table
Input   :   none
Returns :   void
Notes   :
============================================================================*/
void set_gray_scale_table()
{
	write_command(0xB8);			// Set Gray Scale Table
	write_data(0x0C);			//   Gray Scale Level 1
	write_data(0x18);			//   Gray Scale Level 2
	write_data(0x24);			//   Gray Scale Level 3
	write_data(0x30);			//   Gray Scale Level 4
	write_data(0x3C);			//   Gray Scale Level 5
	write_data(0x48);			//   Gray Scale Level 6
	write_data(0x54);			//   Gray Scale Level 7
	write_data(0x60);			//   Gray Scale Level 8
	write_data(0x6C);			//   Gray Scale Level 9
	write_data(0x78);			//   Gray Scale Level 10
	write_data(0x84);			//   Gray Scale Level 11
	write_data(0x90);			//   Gray Scale Level 12
	write_data(0x9C);			//   Gray Scale Level 13
	write_data(0xA8);			//   Gray Scale Level 14
	write_data(0xB4);			//   Gray Scale Level 15

	write_command(0x00);			// Enable Gray Scale Table
}

/*============================================================================
Function:   set_linear_gray_scale_table()
------------------------------------------------------------------------------
Purpose :   Set linear gray scale table
Input   :   none
Returns :   void
Notes   :
============================================================================*/
void set_linear_gray_scale_table()
{
	write_command(0xB9);			// Set Default Linear Gray Scale Table
}