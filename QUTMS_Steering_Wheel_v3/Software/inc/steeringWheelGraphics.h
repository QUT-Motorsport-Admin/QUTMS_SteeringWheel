/*****************************************************************************
* @file    Software/inc/steeringWheelGraphics.h 
* @author  Zoe Goodward
* @version V1.0.0
* @date    28-April-2018
* @brief   This file declares the variables and functions that are used to 
*          produce graphics on the OLED screen
*****************************************************************************/

#ifndef STEERINGWHEELGRAPHICS_H_
#define STEERINGWHEELGRAPHICS_H_



// Fill ram
#define CLEAR_SCREEN 0x00
#define FILL_SCREEN 0xff

// Vertical scroll
#define UPWARD 0x00
#define DOWNWARD 0x01

// Fade scroll
#define UPWARD_OUT 0x02
#define DOWNWARD_OUT 0x03

void fill_block(unsigned char data, unsigned char a, unsigned char b, unsigned char c, unsigned char d);
void checkerboard();
void grayscale();

void show_font_5x7(unsigned char a, unsigned char b, unsigned char c, unsigned char d);

void show_string(unsigned char a, unsigned char *data_pointer, unsigned char b, unsigned char c);
void show_huge_string(unsigned char *data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned int length,  unsigned char LwrNibbleBrightness, unsigned char UppNibbleBrightness);
void show_bigger_string(unsigned char *data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned int length,  unsigned char LwrNibbleBrightness, unsigned char UppNibbleBrightness);
void show_big_string(unsigned char *data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned int length, unsigned char LwrNibbleBrightness, unsigned char UppNibbleBrightness);

void show_char(unsigned char a, unsigned char data, unsigned char b, unsigned char c);
void show_huge_char(unsigned char data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char LwrNibbleBrightness, unsigned char UppNibbleBrightness);
void show_bigger_char(unsigned char data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char LwrNibbleBrightness, unsigned char UppNibbleBrightness);
void show_big_char(unsigned char data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char LwrNibbleBrightness, unsigned char UppNibbleBrightness);

void show_pattern(unsigned char *data_pointer, unsigned char a, unsigned char b, unsigned char c, unsigned char d);

void display_picture(const unsigned char pic[]);

void vertical_scroll(unsigned char a, unsigned char b, unsigned char c);
void fade_scroll(unsigned char a, unsigned char b, unsigned char c);
void fade_in();
void fade_out();

void set_gray_scale_table();
void set_linear_gray_scale_table();

void draw_rectangle(unsigned char data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e);

#endif /* STEERINGWHEELGRAPHICS_H_ */