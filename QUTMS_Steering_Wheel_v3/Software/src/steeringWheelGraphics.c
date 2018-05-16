/*****************************************************************************
* @file    Software/src/steeringWheelGraphics.c 
* @author  Zoe Goodward
* @version V1.0.0
* @date    28-April-2018
* @brief   
*****************************************************************************/

#include "steeringWheelGraphics.h"

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Graphic Acceleration (Partial or Full Screen)
//
//    a: Line Width
//    b: Column Address of Start
//    c: Column Address of End
//    d: Row Address of Start
//	  e: Row Address of End
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void Draw_Rectangle(unsigned char Data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e)
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

	Set_Column_Address(Shift+b,Shift+c);
	Set_Row_Address(d,(d+a-1));
	Set_Write_RAM();
	for(i=0;i<(c-b+1);i++)
	{
		for(j=0;j<a;j++)
		{
			Write_Data(Data);
			Write_Data(Data);
		}
	}

	Set_Column_Address(Shift+(c-l),Shift+c);
	Set_Row_Address(d+a,e-a);
	Set_Write_RAM();
	for(i=0;i<(e-d+1);i++)
	{
		for(j=0;j<(l+1);j++)
		{
			if(j == 0)
			{
				switch(k)
				{
					case 0:
					Write_Data(Data);
					Write_Data(Data);
					break;
					case 1:
					Write_Data(0x00);
					Write_Data(Data&0x0F);
					break;
					case 2:
					Write_Data(0x00);
					Write_Data(Data);
					break;
					case 3:
					Write_Data(Data&0x0F);
					Write_Data(Data);
					break;
				}
			}
			else
			{
				Write_Data(Data);
				Write_Data(Data);
			}
		}
	}

	Set_Column_Address(Shift+b,Shift+c);
	Set_Row_Address((e-a+1),e);
	Set_Write_RAM();
	for(i=0;i<(c-b+1);i++)
	{
		for(j=0;j<a;j++)
		{
			Write_Data(Data);
			Write_Data(Data);
		}
	}

	Set_Column_Address(Shift+b,Shift+(b+l));
	Set_Row_Address(d+a,e-a);
	Set_Write_RAM();
	for(i=0;i<(e-d+1);i++)
	{
		for(j=0;j<(l+1);j++)
		{
			if(j == l)
			{
				switch(k)
				{
					case 0:
					Write_Data(Data);
					Write_Data(Data);
					break;
					case 1:
					Write_Data(Data&0xF0);
					Write_Data(0x00);
					break;
					case 2:
					Write_Data(Data);
					Write_Data(0x00);
					break;
					case 3:
					Write_Data(Data);
					Write_Data(Data&0xF0);
					break;
				}
			}
			else
			{
				Write_Data(Data);
				Write_Data(Data);
			}
		}
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Regular Pattern (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void fill_ram(unsigned char Data)
{
	unsigned char i,j;

	Set_Column_Address(0x00,0x77);
	Set_Row_Address(0x00,0x7F);
	Set_Write_RAM();

	for(i=0;i<128;i++)
	{
		for(j=0;j<120;j++)
		{
			Write_Data(Data);
			//Write_Data(Data);
		}
	}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Regular Pattern (Partial or Full Screen)
//
//    a: Column Address of Start
//    b: Column Address of End (Total Columns Devided by 4)
//    c: Row Address of Start
//    d: Row Address of End
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fill_Block(unsigned char Data, unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
	unsigned char i,j;
	
	Set_Column_Address(Shift+a,Shift+b);
	Set_Row_Address(c,d);
	Set_Write_RAM();

	for(i=0;i<(d-c+1);i++)
	{
		for(j=0;j<(b-a+1);j++)
		{
			Write_Data(Data);
			Write_Data(Data);
		}
	}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Checkboard (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Checkerboard()
{
	unsigned char i,j;
	
	Set_Column_Address(0x00,0x77);
	Set_Row_Address(0x00,0x7F);
	Set_Write_RAM();

	for(i=0;i<64;i++)
	{
		for(j=0;j<120;j++)
		{
			Write_Data(0xF0);
			Write_Data(0xF0);
		}
		for(j=0;j<120;j++)
		{
			Write_Data(0x0F);
			Write_Data(0x0F);
		}
	}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Gray Scale Bar (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Grayscale()
{
	// Level 16 => Column 1~16
	Fill_Block(0xFF,0x00,0x03,0x00,Max_Row);

	// Level 15 => Column 17~32
	Fill_Block(0xEE,0x04,0x07,0x00,Max_Row);

	// Level 14 => Column 33~48
	Fill_Block(0xDD,0x08,0x0B,0x00,Max_Row);

	// Level 13 => Column 49~64
	Fill_Block(0xCC,0x0C,0x0F,0x00,Max_Row);

	// Level 12 => Column 65~80
	Fill_Block(0xBB,0x10,0x13,0x00,Max_Row);

	// Level 11 => Column 81~96
	Fill_Block(0xAA,0x14,0x17,0x00,Max_Row);

	// Level 10 => Column 97~112
	Fill_Block(0x99,0x18,0x1B,0x00,Max_Row);

	// Level 9 => Column 113~128
	Fill_Block(0x88,0x1C,0x1F,0x00,Max_Row);

	// Level 8 => Column 129~144
	Fill_Block(0x77,0x20,0x23,0x00,Max_Row);

	// Level 7 => Column 145~160
	Fill_Block(0x66,0x24,0x27,0x00,Max_Row);

	// Level 6 => Column 161~176
	Fill_Block(0x55,0x28,0x2B,0x00,Max_Row);

	// Level 5 => Column 177~192
	Fill_Block(0x44,0x2C,0x2F,0x00,Max_Row);

	// Level 4 => Column 193~208
	Fill_Block(0x33,0x30,0x33,0x00,Max_Row);

	// Level 3 => Column 209~224
	Fill_Block(0x22,0x34,0x37,0x00,Max_Row);

	// Level 2 => Column 225~240
	Fill_Block(0x11,0x38,0x3B,0x00,Max_Row);

	// Level 1 => Column 241~256
	Fill_Block(0x00,0x3C,Max_Column,0x00,Max_Row);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Character (5x7)
//
//    a: Database
//    b: Ascii
//    c: Start X Address
//    d: Start Y Address
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_Font57(unsigned char a, unsigned char b, unsigned char c, unsigned char d)
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

	Set_Remap_Format(0x15);
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
		Set_Column_Address(Shift+c,Shift+c);
		Set_Row_Address(d,d+7);
		Set_Write_RAM();
		
		Font=((MSB_1&0x01)<<4)|(LSB_1&0x01);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3);
		Write_Data(Font);
		Font=((MSB_2&0x01)<<4)|(LSB_2&0x01);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3);
		Write_Data(Font);
		
		Font=((MSB_1&0x02)<<3)|((LSB_1&0x02)>>1);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3);
		Write_Data(Font);
		Font=((MSB_2&0x02)<<3)|((LSB_2&0x02)>>1);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3);
		Write_Data(Font);
		
		Font=((MSB_1&0x04)<<2)|((LSB_1&0x04)>>2);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3);
		Write_Data(Font);
		Font=((MSB_2&0x04)<<2)|((LSB_2&0x04)>>2);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3);
		Write_Data(Font);
		
		Font=((MSB_1&0x08)<<1)|((LSB_1&0x08)>>3);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3);
		Write_Data(Font);
		Font=((MSB_2&0x08)<<1)|((LSB_2&0x08)>>3);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3);
		Write_Data(Font);
		
		Font=((MSB_1&0x10)<<3)|((LSB_1&0x10)>>1);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3);
		Write_Data(Font);
		Font=((MSB_2&0x10)<<3)|((LSB_2&0x10)>>1);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3);
		Write_Data(Font);

		Font=((MSB_1&0x20)<<2)|((LSB_1&0x20)>>2);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3);
		Write_Data(Font);
		Font=((MSB_2&0x20)<<2)|((LSB_2&0x20)>>2);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3);
		Write_Data(Font);

		Font=((MSB_1&0x40)<<1)|((LSB_1&0x40)>>3);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3);
		Write_Data(Font);
		Font=((MSB_2&0x40)<<1)|((LSB_2&0x40)>>3);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3);
		Write_Data(Font);

		Font=(MSB_1&0x80)|((LSB_1&0x80)>>4);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3);
		Write_Data(Font);
		Font=(MSB_2&0x80)|((LSB_2&0x80)>>4);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3);
		Write_Data(Font);
		
		//Set_Row_Address(derp,derp+7);
		//Set_Write_RAM();

		/*

		Font=((MSB_1&0x01)<<4)|(LSB_1&0x01);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3)|(Font<<4)|(Font<<5);
		Write_Data(Font);
		Font=((MSB_2&0x01)<<4)|(LSB_2&0x01);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3)|(Font<<4)|(Font<<5);
		Write_Data(Font);
		
		Font=((MSB_1&0x02)<<3)|((LSB_1&0x02)>>1);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3)|(Font<<4)|(Font<<5);
		Write_Data(Font);
		Font=((MSB_2&0x02)<<3)|((LSB_2&0x02)>>1);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3)|(Font<<4)|(Font<<5);
		Write_Data(Font);
		
		Font=((MSB_1&0x04)<<2)|((LSB_1&0x04)>>2);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3)|(Font<<4)|(Font<<5);
		Write_Data(Font);
		Font=((MSB_2&0x04)<<2)|((LSB_2&0x04)>>2);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3)|(Font<<4)|(Font<<5);
		Write_Data(Font);
		
		Font=((MSB_1&0x08)<<1)|((LSB_1&0x08)>>3);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3)|(Font<<4)|(Font<<5);
		Write_Data(Font);
		Font=((MSB_2&0x08)<<1)|((LSB_2&0x08)>>3);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3)|(Font<<4)|(Font<<5);
		Write_Data(Font);
		
		Font=((MSB_1&0x10)<<3)|((LSB_1&0x10)>>1);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3)|(Font>>4)|(Font>>5);
		Write_Data(Font);
		Font=((MSB_2&0x10)<<3)|((LSB_2&0x10)>>1);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3)|(Font>>4)|(Font>>5);
		Write_Data(Font);

		Font=((MSB_1&0x20)<<2)|((LSB_1&0x20)>>2);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3)|(Font>>4)|(Font>>5);
		Write_Data(Font);
		Font=((MSB_2&0x20)<<2)|((LSB_2&0x20)>>2);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3)|(Font>>4)|(Font>>5);
		Write_Data(Font);

		Font=((MSB_1&0x40)<<1)|((LSB_1&0x40)>>3);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3)|(Font>>4)|(Font>>5);
		Write_Data(Font);
		Font=((MSB_2&0x40)<<1)|((LSB_2&0x40)>>3);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3)|(Font>>4)|(Font>>5);
		Write_Data(Font);

		Font=(MSB_1&0x80)|((LSB_1&0x80)>>4);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3)|(Font>>4)|(Font>>5);
		Write_Data(Font);
		Font=(MSB_2&0x80)|((LSB_2&0x80)>>4);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3)|(Font>>4)|(Font>>5);
		Write_Data(Font);
		
		*/
		
		c++;
	}
	Set_Remap_Format(0x14);
	
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Character
//
//    a: Database
//    b: Start X Address
//    c: Start Y Address
//    * Must write "0" in the end...
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_Char(unsigned char a, unsigned char Data, unsigned char b, unsigned char c)
{
	//unsigned char *Src_Pointer;

	//Src_Pointer=Data_Pointer;
	Show_Font57(1,96,b,c);			// No-Break Space
	//   Must be written first before the string start...

	Show_Font57(a,Data,b,c);
	//while(1)
	//{
		//Show_Font57(a,*Src_Pointer,b,c);
		//Src_Pointer++;
		//b+=2;
		//if(*Src_Pointer == 0) break;
	//}
}

void Show_Huge_Char(unsigned char data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char LwrNibbleBrightness, unsigned char UppNibbleBrightness)
{
	Set_Column_Address(Shift+a,Shift+b);
	Set_Row_Address(c,c+38);
	Set_Write_RAM();
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
			Write_Data(tempNibbles);
		}
		Set_Column_Address(Shift+a,Shift+b);
		Set_Row_Address(c+i+1,c+38);
		Set_Write_RAM();
	}
}
void Show_Bigger_Char(unsigned char data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char LwrNibbleBrightness, unsigned char UppNibbleBrightness)
{
	Set_Column_Address(Shift+a,Shift+b);
	Set_Row_Address(c,c+25);
	Set_Write_RAM();
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
			Write_Data(tempNibbles);
		}
		Set_Column_Address(Shift+a,Shift+b);
		Set_Row_Address(c+i+1,c+30);
		Set_Write_RAM();
	}
}

void Show_Big_Char(unsigned char data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char LwrNibbleBrightness, unsigned char UppNibbleBrightness)
{
	Set_Column_Address(Shift+a,Shift+b);
	Set_Row_Address(c,c+17);
	Set_Write_RAM();
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
			Write_Data(tempNibbles);
		}
		Set_Column_Address(Shift+a,Shift+b);
		Set_Row_Address(c+i+1,c+17);
		Set_Write_RAM();
	}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show String
//
//    a: Database
//    b: Start X Address
//    c: Start Y Address
//    * Must write "0" in the end...
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_String(unsigned char a, unsigned char *Data_Pointer, unsigned char b, unsigned char c)
{
	unsigned char *Src_Pointer;

	Src_Pointer=Data_Pointer;
	Show_Font57(1,96,b,c);			// No-Break Space
	//   Must be written first before the string start...

	while(1)
	{
		Show_Font57(a,*Src_Pointer,b,c);
		Src_Pointer++;
		b+=2;
		if(*Src_Pointer == 0) break;
	}
}

void Show_Huge_String(unsigned char *Data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned int length,  unsigned char LwrNibbleBrightness, unsigned char UppNibbleBrightness)
{
	for(int i = 0; i < length; i++)
	{
		Show_Huge_Char(*Data,a,b,c,d, LwrNibbleBrightness, UppNibbleBrightness);
		Data++;
		a+=7;
		b+=7;
	}
}

void Show_Bigger_String(unsigned char *Data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned int length,  unsigned char LwrNibbleBrightness, unsigned char UppNibbleBrightness)
{
	for(int i = 0; i < length; i++)
	{
		Show_Bigger_Char(*Data,a,b,c,d, LwrNibbleBrightness, UppNibbleBrightness);
		Data++;
		a+=4;
		b+=4;
	}
}

void Show_Big_String(unsigned char *Data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned int length, unsigned char LwrNibbleBrightness, unsigned char UppNibbleBrightness)
{
	for(int i = 0; i < length; i++)
	{
		Show_Big_Char(*Data,a,b,c,d, LwrNibbleBrightness, UppNibbleBrightness);
		Data++;
		a+=3;
		b+=3;
	}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Pattern (Partial or Full Screen)
//
//    a: Column Address of Start
//    b: Column Address of End (Total Columns Devided by 4)
//    c: Row Address of Start
//    d: Row Address of End
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_Pattern(unsigned char *Data_Pointer, unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
	unsigned char *Src_Pointer;
	unsigned char i,j;
	
	Src_Pointer=Data_Pointer;
	Set_Column_Address(Shift+a,Shift+b);
	Set_Row_Address(c,d);
	Set_Write_RAM();

	for(i=0;i<(d-c+1);i++)
	{
		for(j=0;j<(b-a+1);j++)
		{
			Write_Data(*Src_Pointer);
			Src_Pointer++;
			Write_Data(*Src_Pointer);
			Src_Pointer++;
		}
	}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  DATA PROCESSING
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Data_processing(unsigned char temp)  //turns 1byte B/W data to 4 bye gray data
{unsigned char temp1,temp2,temp3,temp4,temp5,temp6,temp7,temp8;
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

	Write_Data(d1);
	Write_Data(d2);
	Write_Data(d3);
	Write_Data(d4);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  DISPLAY PICTURE
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Display_Picture(unsigned char pic[])
{
	unsigned char i,j;
	Set_Column_Address(Shift+0x00,Shift+0x77);
	Set_Row_Address(0x00,0x7F);
	Set_Write_RAM();

	for(i=0;i<64;i++)
	{
		for(j=0;j<60;j++)
		{
			Data_processing(pgm_read_byte(&pic[i*32+j]));
		}
	}
	return;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Vertical Scrolling (Full Screen)
//
//    a: Scrolling Direction
//       "0x00" (Upward)
//       "0x01" (Downward)
//    b: Set Numbers of Row Scroll per Step
//    c: Set Time Interval between Each Scroll Step
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Vertical_Scroll(unsigned char a, unsigned char b, unsigned char c)
{
	unsigned char i,j;

	Set_Partial_Display(0x00,0x00,Max_Row);

	switch(a)
	{
		case 0:
		for(i=0;i<(Max_Row+1);i+=b)
		{
			Set_Display_Offset(i+1);
			for(j=0;j<c;j++)
			{
				uDelay(1600);
			}
		}
		break;
		case 1:
		for(i=0;i<(Max_Row+1);i+=b)
		{
			Set_Display_Offset(Max_Row-i);
			for(j=0;j<c;j++)
			{
				uDelay(1600);
			}
		}
		break;
	}
	Set_Partial_Display(0x01,0x00,0x00);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Vertical Fade Scrolling (Full Screen)
//
//    a: Scrolling Direction
//       "0x00" (Upward - In)
//       "0x01" (Downward - In)
//       "0x02" (Upward - Out)
//       "0x03" (Downward - Out)
//    b: Set Numbers of Row Scroll per Step
//    c: Set Time Interval between Each Scroll Step
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fade_Scroll(unsigned char a, unsigned char b, unsigned char c)
{
	unsigned char i,j;

	Set_Partial_Display(0x00,0x00,Max_Row);

	switch(a)
	{
		case 0:
		for(i=(Max_Row+1);i<128;i+=b)
		{
			Set_Start_Line(i);
			for(j=0;j<c;j++)
			{
				uDelay(200);
			}
		}
		Set_Start_Line(0x00);
		for(j=0;j<c;j++)
		{
			uDelay(200);
		}
		break;
		case 1:
		for(i=0;i<(Max_Row+1);i+=b)
		{
			Set_Start_Line(Max_Row-i);
			for(j=0;j<c;j++)
			{
				uDelay(200);
			}
		}
		break;
		case 2:
		for(i=0;i<(Max_Row+1);i+=b)
		{
			Set_Start_Line(i+1);
			for(j=0;j<c;j++)
			{
				uDelay(200);
			}
		}
		break;
		case 3:
		for(i=127;i>Max_Row;i-=b)
		{
			Set_Start_Line(i);
			for(j=0;j<c;j++)
			{
				uDelay(200);
			}
		}
		break;
	}
	Set_Partial_Display(0x01,0x00,0x00);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Fade In (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fade_In()
{
	unsigned char i;

	Set_Display_On_Off(0x01);
	for(i=0;i<(Brightness+1);i++)
	{
		Set_Master_Current(i);
		uDelay(200);
		uDelay(200);
		uDelay(200);
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Fade Out (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fade_Out()
{
	unsigned char i;

	for(i=(Brightness+1);i>0;i--)
	{
		Set_Master_Current(i-1);
		uDelay(200);
		uDelay(200);
		uDelay(200);
	}
	Set_Display_On_Off(0x00);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Sleep Mode
//
//    "0x01" Enter Sleep Mode
//    "0x00" Exit Sleep Mode
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Sleep(unsigned char a)
{
	switch(a)
	{
		case 0:
		Set_Display_On_Off(0x00);
		Set_Display_Mode(0x01);
		break;
		case 1:
		Set_Display_Mode(0x02);
		Set_Display_On_Off(0x01);
		break;
	}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Connection Test
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Test()
{
	unsigned char i;

	RESET_low;
	Delay(300);
	RESET_high;

	Set_Display_Mode(0x01);			// Entire Display On Mode (0x00/0x01/0x02/0x03)

	while(1)
	{
		Set_Display_On_Off(0x01);	// Display On (0x00/0x01)
		Delay(32);
		Set_Display_On_Off(0x00);	// Display Off (0x00/0x01)
		Delay(32);
	}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Gray Scale Table Setting (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Set_Gray_Scale_Table()
{
	Write_Command(0xB8);			// Set Gray Scale Table
	Write_Data(0x0C);			//   Gray Scale Level 1
	Write_Data(0x18);			//   Gray Scale Level 2
	Write_Data(0x24);			//   Gray Scale Level 3
	Write_Data(0x30);			//   Gray Scale Level 4
	Write_Data(0x3C);			//   Gray Scale Level 5
	Write_Data(0x48);			//   Gray Scale Level 6
	Write_Data(0x54);			//   Gray Scale Level 7
	Write_Data(0x60);			//   Gray Scale Level 8
	Write_Data(0x6C);			//   Gray Scale Level 9
	Write_Data(0x78);			//   Gray Scale Level 10
	Write_Data(0x84);			//   Gray Scale Level 11
	Write_Data(0x90);			//   Gray Scale Level 12
	Write_Data(0x9C);			//   Gray Scale Level 13
	Write_Data(0xA8);			//   Gray Scale Level 14
	Write_Data(0xB4);			//   Gray Scale Level 15

	Write_Command(0x00);			// Enable Gray Scale Table
}


void Set_Linear_Gray_Scale_Table()
{
	Write_Command(0xB9);			// Set Default Linear Gray Scale Table
}