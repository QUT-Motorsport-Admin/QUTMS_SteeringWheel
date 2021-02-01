/*
 *	File: gui.h
 *  Created on: 12 Nov 2020
 *  Author: James Hoskin
 */
#pragma once
#include <car_settings.h>
#include <stdint.h>
#include <stdbool.h>
#include "fonts.h"
#include "car_settings.h"

// Dimensions
#define screen_width 320
#define screen_margin 14
#define menu_line_height 58
#define reading_line_height 47

// Colors
#define primary_text_color LCD_COLOR_WHITE
#define selection_color LCD_COLOR_ORANGE
#define primary_back_color LCD_COLOR_BLUE
#define warning_color LCD_COLOR_RED
#define caution_color LCD_COLOR_YELLOW
#define nominal_color LCD_COLOR_GREEN

typedef enum UI_Screen
{
    SM_SCREEN,
    RTD_SCREEN,
    DRIVER_SELECTION_SCREEN,
    EVENT_SELECTION_SCREEN,
    CAR_CONFIGURATION_SCREEN,
    ADVANCED_SCREEN
} UI_Screen;

/* General UI functions */
void clearArea(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void initaliseDrivers();
void initaliseEventModes();

/* UI Pages */
void drawScreen(UI_Screen ui_screen);
void drawStartupScreen();

void drawRTDScreen();
void updateRTDScreen();

void drawSMScreen();
void updateSMScreen();

void drawDriverSelectionScreen();
void updateDriverSelectionScreen();

void drawEventSelectionScreen();
void updateEventSelectionScreen();

void drawCarConfigurationScreen();
void updateCarConfigurationScreen();

void drawAdvancedScreen();
void updateAdvancedScreen();

/* Returns true if the selected menu option has changed */
bool updateMenuScroll();

void updateCarConfiguration();

/* UI Components */
void drawPrimaryHeaderBar(Drive_Mode current_drive_mode);
void drawSecondaryHeaderBar(uint8_t* label);

void drawMenuItemWithValue(uint16_t y, uint8_t* label, uint8_t value, bool selected, bool draw_borders);
void updateMenuItemWithValue(uint16_t y, uint8_t* label, uint8_t value, bool selected);

void drawReading(uint16_t y, uint8_t* label, uint8_t* value, bool draw_borders);
void updateReading(uint16_t y, uint8_t* value, uint32_t text_color);

void drawMenuItem(uint16_t y, uint8_t* label, bool selected, bool draw_borders);
void updateMenuItem(uint16_t y, uint8_t* label, bool selected);

void drawAccumulatorTotalBar(uint16_t yPos);
void updateAccumulatorTotalBar(uint16_t yPos);

void drawAccumulatorDeltaBar(uint16_t yPos);
void updateAccumulatorDeltaBar(uint16_t yPos);

void drawWarning();
