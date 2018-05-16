/*****************************************************************************
* @file    Software/src/steeringWheelOLED.c
* @author  Zoe Goodward
* @version V1.0.0
* @date    28-April-2018
* @brief   
*****************************************************************************/

 #include "steeringWheelOLED.h"

/*============================================================================
Function:   oled_init()
------------------------------------------------------------------------------
Purpose :   initialises the OLED screen
Input   :   none
Returns :   void
Notes   :
============================================================================*/
void oled_init()
{
	unsigned char i;

	RESET_low;
	Delay(10);
	
	RESET_high;

	Set_Command_Lock(0x12);			// Unlock Basic Commands (0x12/0x16)
	Set_Display_On_Off(0x00);		// Display Off (0x00/0x01)
	Set_Display_Clock(0x91);		// Set Clock as 80 Frames/Sec
	Set_Multiplex_Ratio(0x3F);		// 1/64 Duty (0x0F~0x3F)
	Set_Display_Offset(0x00);		// Shift Mapping RAM Counter (0x00~0x3F)
	Set_Start_Line(0x00);			// Set Mapping RAM Display Start Line (0x00~0x7F)
	Set_Remap_Format(0x14);			// Set Horizontal Address Increment
	//     Column Address 0 Mapped to SEG0
	//     Disable Nibble Remap
	//     Scan from COM[N-1] to COM0
	//     Disable COM Split Odd Even
	//     Enable Dual COM Line Mode
	Set_GPIO(0x00);				            // Disable GPIO Pins Input
	Set_Function_Selection(0x01);		    // Enable Internal VDD Regulator
	Set_Display_Enhancement_A(0xA0,0xFD);	// Enable External VSL
	// Set Low Gray Scale Enhancement
	Set_Contrast_Current(0x7F);		    // Set Segment Output Current
	Set_Master_Current(Brightness);		// Set Scale Factor of Segment Output Current Control
	//Set_Gray_Scale_Table();			// Set Pulse Width for Gray Scale Table
	Set_Linear_Gray_Scale_Table();
	Set_Phase_Length(0xE2);			        // Set Phase 1 as 5 Clocks & Phase 2 as 14 Clocks
	Set_Display_Enhancement_B(0x20);	    // Enhance Driving Scheme Capability (0x00/0x20)
	Set_Precharge_Voltage(0x1F);		    // Set Pre-Charge Voltage Level as 0.60*VCC
	Set_Precharge_Period(0x08);		        // Set Second Pre-Charge Period as 8 Clocks
	Set_VCOMH(0x07);			            // Set Common Pins Deselect Voltage Level as 0.86*VCC
	Set_Display_Mode(0x02);			        // Normal Display Mode (0x00/0x01/0x02/0x03)
	Set_Partial_Display(0x01,0x00,0x00);	// Disable Partial Display

	Fill_RAM(0x00);				// Clear Screen

	Set_Display_On_Off(0x01);	// Display On (0x00/0x01)
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Delay Time
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void uDelay(unsigned int l)
{
	while(l--);
}

void Delay(unsigned int n)
{
	unsigned int k;

	for(k=0;k<n;k++)
	{
		uDelay(1120);
	}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Read/Write Sequence
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void write_command(unsigned char Data)
{
	unsigned char i;
	DC_instruction;
	CS_low;
	spi_send_byte(Data);
	/* soft SPI
	for (i=0; i<8; i++)
	{
		SPI_clock_low;
		if((Data&0x80) > 0)SPI_data_high;
		else SPI_data_low;
		Data = Data << 1;
		//uDelay(1);
		SPI_clock_high;
		//uDelay(1);
	}
	SPI_clock_high;
	*/	
	DC_data;
	CS_high;
}

void write_data(unsigned char Data)
{
	CS_low;
	DC_data;
	spi_send_byte(Data);	
	DC_data;
	CS_high;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Instruction Setting
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Set_Column_Address(unsigned char a, unsigned char b)
{
	write_command(0x15);			// Set Column Address
	write_data(a);				//   Default => 0x00
	write_data(b);				//   Default => 0x77
}

void Set_Row_Address(unsigned char a, unsigned char b)
{
	write_command(0x75);			// Set Row Address
	write_data(a);				//   Default => 0x00
	write_data(b);				//   Default => 0x7F
}

void Set_Write_RAM()
{
	write_command(0x5C);			// Enable MCU to Write into RAM
}

void Set_Read_RAM()
{
	write_command(0x5D);			// Enable MCU to Read from RAM
}

void Set_Remap_Format(unsigned char d)
{
	write_command(0xA0);			// Set Re-Map / Dual COM Line Mode
	write_data(d);				//   Default => 0x40
	//     Horizontal Address Increment
	//     Column Address 0 Mapped to SEG0
	//     Disable Nibble Remap
	//     Scan from COM0 to COM[N-1]
	//     Disable COM Split Odd Even
	write_data(0x11);			//   Default => 0x01 (Disable Dual COM Mode)
}

void Set_Start_Line(unsigned char d)
{
	write_command(0xA1);			// Set Vertical Scroll by RAM
	write_data(d);				//   Default => 0x00
}

void Set_Display_Offset(unsigned char d)
{
	write_command(0xA2);			// Set Vertical Scroll by Row
	write_data(d);				//   Default => 0x00
}

void Set_Display_Mode(unsigned char d)
{
	write_command(0xA4|d);			// Set Display Mode
	//   Default => 0xA4
	//     0xA4 (0x00) => Entire Display Off, All Pixels Turn Off
	//     0xA5 (0x01) => Entire Display On, All Pixels Turn On at GS Level 15
	//     0xA6 (0x02) => Normal Display
	//     0xA7 (0x03) => Inverse Display
}

void Set_Partial_Display(unsigned char a, unsigned char b, unsigned char c)
{
	write_command(0xA8|a);
	// Default => 0x8F
	//   Select Internal Booster at Display On
	if(a == 0x00)
	{
		write_data(b);
		write_data(c);
	}
}

void Set_Function_Selection(unsigned char d)
{
	write_command(0xAB);			// Function Selection
	write_data(d);				//   Default => 0x01
	//     Enable Internal VDD Regulator
}

void Set_Display_On_Off(unsigned char d)
{
	write_command(0xAE|d);			// Set Display On/Off
	//   Default => 0xAE
    //#define ON 0x00 or 0xAE
    //#define OFF 0x01 or 0xAF
	//     0xAE (0x00) => Display Off (Sleep Mode On)
	//     0xAF (0x01) => Display On (Sleep Mode Off)
}

void Set_Phase_Length(unsigned char d)
{
	write_command(0xB1);			// Phase 1 (Reset) & Phase 2 (Pre-Charge) Period Adjustment
	write_data(d);				//   Default => 0x74 (7 Display Clocks [Phase 2] / 9 Display Clocks [Phase 1])
	//     D[3:0] => Phase 1 Period in 5~31 Display Clocks
	//     D[7:4] => Phase 2 Period in 3~15 Display Clocks
}

void Set_Display_Clock(unsigned char d)
{
	write_command(0xB3);			// Set Display Clock Divider / Oscillator Frequency
	write_data(d);				//   Default => 0xD0
	//     A[3:0] => Display Clock Divider
	//     A[7:4] => Oscillator Frequency
}

void Set_Display_Enhancement_A(unsigned char a, unsigned char b)
{
	write_command(0xB4);			// Display Enhancement
	write_data(0xA0|a);			//   Default => 0xA2
	//     0xA0 (0x00) => Enable External VSL
	//     0xA2 (0x02) => Enable Internal VSL (Kept VSL Pin N.C.)
	write_data(0x05|b);			//   Default => 0xB5
	//     0xB5 (0xB0) => Normal
	//     0xFD (0xF8) => Enhance Low Gray Scale Display Quality
}

void Set_GPIO(unsigned char d)
{
	write_command(0xB5);			// General Purpose IO
	write_data(d);				//   Default => 0x0A (GPIO Pins output Low Level.)
}

void Set_Precharge_Period(unsigned char d)
{
	write_command(0xB6);			// Set Second Pre-Charge Period
	write_data(d);				//   Default => 0x08 (8 Display Clocks)
}

void Set_Precharge_Voltage(unsigned char d)
{
	write_command(0xBB);			// Set Pre-Charge Voltage Level
	write_data(d);				//   Default => 0x17 (0.50*VCC)
}

void Set_VCOMH(unsigned char d)
{
	write_command(0xBE);			// Set COM Deselect Voltage Level
	write_data(d);				//   Default => 0x04 (0.80*VCC)
}

void Set_Contrast_Current(unsigned char d)
{
	write_command(0xC1);			// Set Contrast Current
	write_data(d);				//   Default => 0x7F
}

void Set_Master_Current(unsigned char d)
{
	write_command(0xC7);			// Master Contrast Current Control
	write_data(d);				//   Default => 0x0f (Maximum)
}

void Set_Multiplex_Ratio(unsigned char d)
{
	write_command(0xCA);			// Set Multiplex Ratio
	write_data(d);				//   Default => 0x7F (1/128 Duty)
}

void Set_Display_Enhancement_B(unsigned char d)
{
	write_command(0xD1);			// Display Enhancement
	write_data(0x82|d);			//   Default => 0xA2
	//     0x82 (0x00) => Reserved
	//     0xA2 (0x20) => Normal
	write_data(0x20);
}

void Set_Command_Lock(unsigned char d)
{
	write_command(0xFD);			// Set Command Lock
	write_data(0x12|d);			//   Default => 0x12
	//     0x12 => Driver IC interface is unlocked from entering command.
	//     0x16 => All Commands are locked except 0xFD.
}