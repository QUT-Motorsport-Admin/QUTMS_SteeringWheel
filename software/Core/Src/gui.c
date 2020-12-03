/*
 *	File: gui.c
 *  Created on: 12 Nov 2020
 *  Author: James Hoskin
 */
#include "gui.h"
#include "stm32_adafruit_lcd.h"
#include <stdint.h>
#include <string.h>
#include "car_settings.h"
#include <stdlib.h>
#include <stdio.h>


extern Driver_Profile drivers[4];
extern Driver_Profile current_driver;
extern Event_Profile events[4];
extern uint8_t selected_menu_option;
extern uint8_t max_menu_option;
extern float accumul_volts, accumul_temp, gearbox_temp, inverter_temp, motor_temp, accumul_charge, accumul_delta;
extern uint8_t total_laps, current_lap;
char* menus[4] = { "Driver Select", "Event Select", "Car Config", "Advanced" };
bool menu_pot_incremented = false;
bool menu_pot_decremented = false;

/* General UI functions */
void clearArea(uint16_t x, uint16_t y, uint16_t width, uint16_t height){
    // Save existing color settings
    uint32_t text_color_backup = BSP_LCD_GetTextColor();
    uint32_t back_color_backup = BSP_LCD_GetBackColor();

    /* Draw a rectangle with background color */
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_FillRect(x, y, width, height);

    // Restore existing color settings
    BSP_LCD_SetTextColor(text_color_backup);
    BSP_LCD_SetBackColor(back_color_backup);
}
void drawScreen(UI_Screen ui_screen) {
    BSP_LCD_Clear(LCD_COLOR_BLACK);
    selected_menu_option = 0; // Reset menu selection
    switch (ui_screen) {
        case SM_SCREEN:
            drawSMScreen();
			max_menu_option = 4;
            break;
        case DRIVER_SELECTION_SCREEN:
            drawDriverSelectionScreen();
			max_menu_option = sizeof(drivers) / sizeof(drivers[0]);
            break;
        case EVENT_SELECTION_SCREEN:
            drawEventSelectionScreen();
			max_menu_option = sizeof(events) / sizeof(events[0]);
            break;
        case CAR_CONFIGURATION_SCREEN:
            drawCarConfigurationScreen();
			max_menu_option = 4;
            break;
        default:
            drawRTDScreen();
			max_menu_option = 0;
            break;
    }
}
void updateMenuScroll() {
	if (menu_pot_incremented && selected_menu_option < max_menu_option) {
		selected_menu_option++;
	}
	else if (menu_pot_decremented && selected_menu_option > 0) {
		selected_menu_option--;
	}
}

/* UI Pages */
void drawStartupScreen() {
    BSP_LCD_SetTextColor(primary_back_color);
    BSP_LCD_SetFont(&Font_RobotoMedium24);
    BSP_LCD_DisplayStringAt(screen_margin - 10, 24 + ((31 / 2) - 12), "START SCREEN", CENTER_MODE);
}

void drawRTDScreen() {
	// Draw header bar
	drawHeaderBar(READY_TO_DRIVE);

	// Draw accumulator bars
	drawAccumulatorDeltaBar(38);
	drawAccumulatorTotalBar(138);

	// Draw divider bar
	BSP_LCD_SetTextColor(primary_back_color);
	BSP_LCD_FillRect(0, 240, 320, 5);

	char* reading[5] = { "Accumul (v)", "Accumul (C)", "Gearbox (C)", "Inverter (C)", "Motor   (C)"};
	uint16_t startingXPos = 245;
	drawReading(startingXPos, reading[0], "?", false);
	for (int i = 1; i < 4; i++) {
		drawReading(startingXPos + (reading_line_height * i), reading[i], "?", true);
	}
	drawReading(startingXPos + (reading_line_height * 4), reading[4], "?", false);
}
void updateRTDScreen() {
    // Update accumulator bars
    updateAccumulatorDeltaBar(38);
    updateAccumulatorTotalBar(138);

    // Update critical readings
    uint32_t reading_value;
    uint16_t startingYPos = 245;

    // Accumul. (v)
    clearArea(screen_width - 60, startingYPos + (reading_line_height * 0) + ((reading_line_height / 2) - 12), 90, 24);
    sprintf(reading_value, "%.0f", accumul_volts);
    if (accumul_volts > 50) {
        updateReading(startingYPos + (reading_line_height * 0), reading_value, warning_color);
    }
    else if (accumul_volts <= 50 && accumul_volts > 30) {
        updateReading(startingYPos + (reading_line_height * 0), reading_value, caution_color);
    }
    else {
        updateReading(startingYPos + (reading_line_height * 0), reading_value, nominal_color);
    }

    // Accumul. (C)
    clearArea(screen_width - 60, startingYPos + (reading_line_height * 1) + ((reading_line_height / 2) - 12), 90, 24);
    sprintf(reading_value, "%.0f", accumul_temp);
    if (accumul_temp > 50) {
		updateReading(startingYPos + (reading_line_height * 1), reading_value, warning_color);
	}
	else if (accumul_temp <= 50 && accumul_temp > 30) {
		updateReading(startingYPos + (reading_line_height * 1), reading_value, caution_color);
	}
	else {
		updateReading(startingYPos + (reading_line_height * 1), reading_value, nominal_color);
	}

    // Gearbox  (C)
    clearArea(screen_width - 60, startingYPos + (reading_line_height * 2) + ((reading_line_height / 2) - 12), 90, 24);
	sprintf(reading_value, "%.0f", gearbox_temp);
	if (gearbox_temp > 50) {
		updateReading(startingYPos + (reading_line_height * 2), reading_value, warning_color);
	}
	else if (gearbox_temp <= 50 && gearbox_temp > 30) {
		updateReading(startingYPos + (reading_line_height * 2), reading_value, caution_color);
	}
	else {
		updateReading(startingYPos + (reading_line_height * 2), reading_value, nominal_color);
	}

    // Inverter (C)
	clearArea(screen_width - 60, startingYPos + (reading_line_height * 3) + ((reading_line_height / 2) - 12), 90, 24);
	sprintf(reading_value, "%.0f", inverter_temp);
	if (inverter_temp > 50) {
		updateReading(startingYPos + (reading_line_height * 3), reading_value, warning_color);
	}
	else if (inverter_temp <= 50 && inverter_temp > 30) {
		updateReading(startingYPos + (reading_line_height * 3), reading_value, caution_color);
	}
	else {
		updateReading(startingYPos + (reading_line_height * 3), reading_value, nominal_color);
	}

    // Motor    (C)
	clearArea(screen_width - 60, startingYPos + (reading_line_height * 4) + ((reading_line_height / 2) - 12), 90, 24);
	sprintf(reading_value, "%.0f", motor_temp);
	if (inverter_temp > 50) {
		updateReading(startingYPos + (reading_line_height * 4), reading_value, warning_color);
	}
	else if (motor_temp <= 50 && motor_temp > 30) {
		updateReading(startingYPos + (reading_line_height * 4), reading_value, caution_color);
	}
	else {
		updateReading(startingYPos + (reading_line_height * 4), reading_value, nominal_color);
	}

}

void drawSMScreen() {
	// Draw header bar
	drawHeaderBar(STATIC_MODE);

	// Draw accumulator bars
	drawAccumulatorDeltaBar(38);
	drawAccumulatorTotalBar(138);

	// Draw divider bar
	BSP_LCD_SetTextColor(primary_back_color);
	BSP_LCD_FillRect(0, 240, screen_width, 5);

	// Draw configuration options
	uint16_t startingXPos = 245;
	drawMenuItem(startingXPos, menus[0], false);
	for (int i = 1; i < 3; i++) {
		drawMenuItem(startingXPos + (menu_line_height * i), menus[i], true);
	}
	drawMenuItem(startingXPos + (menu_line_height * 3), menus[3], false);
}
void updateSMScreen() {
    // Update accumulator bar
    updateAccumulatorTotalBar(138);

    // Draw configuration options
    uint16_t startingXPos = 245;
    for (int i = 0; i < 4; i++) {
        if (i == selected_menu_option) {
            clearArea(0, startingXPos + (menu_line_height * i) + ((menu_line_height / 2) - 12), screen_width, 24);
            updateMenuItem(startingXPos + (menu_line_height * i), menus[i], true); // Highlight selected item
        }
        else updateMenuItem(startingXPos + (menu_line_height * i), menus[i], false);
    }
}

void drawDriverSelectionScreen() {
	// Draw header bar
	drawHeaderBar(STATIC_MODE);
	BSP_LCD_SetTextColor(primary_back_color);
	BSP_LCD_FillRect(0, 27, screen_width, 27);

	// Draw page label
	BSP_LCD_SetBackColor(primary_back_color);
	BSP_LCD_SetTextColor(primary_text_color);
	BSP_LCD_SetFont(&Font_RobotoMedium24);
	BSP_LCD_DisplayStringAt(0, 24 + ((31 / 2) - 12), "Driver Select", CENTER_MODE);

	// Draw settings
	uint8_t startingYPosition = 54;
	drawMenuItem(startingYPosition, drivers[0].driver_name, false);
	for (int i = 1; i < 4; i++) {
		drawMenuItem(startingYPosition + (menu_line_height * i), drivers[i].driver_name, true);
	}
}
void updateDriverSelectionScreen(){
	uint8_t startingYPosition = 54;

    // Draw settings
    for (int i = 0; i < 4; i++) {
        updateMenuItem(startingYPosition + (menu_line_height * i), drivers[i].driver_name, false);
    }

    // Redraw selected item
    updateMenuItem(startingYPosition + (menu_line_height * selected_menu_option), drivers[selected_menu_option].driver_name, true);
}

void drawEventSelectionScreen() {
	// Draw header bar
	drawHeaderBar(STATIC_MODE);
	BSP_LCD_SetTextColor(primary_back_color);
	BSP_LCD_FillRect(0, 24, screen_width, 31);

	// Draw page label
	BSP_LCD_SetBackColor(primary_back_color);
	BSP_LCD_SetTextColor(primary_text_color);
	BSP_LCD_SetFont(&Font_RobotoMedium24);
	BSP_LCD_DisplayStringAt(screen_margin - 10, 24 + ((31 / 2) - 12), "Event Select", CENTER_MODE);

	// Draw settings
	drawMenuItem(55, events[0].event_name, false);
	for (int i = 1; i < 4; i++) {
		drawMenuItem(55 + (menu_line_height * i), events[i].event_name, true);
	}
}
void updateEventSelectionScreen() {
    // Draw settings
    for (int i = 0; i < 4; i++) {
        updateMenuItem(55 + (menu_line_height * i), events[i].event_name, false);
    }

    // Redraw selected item
    updateMenuItem(55 + (menu_line_height * selected_menu_option), events[selected_menu_option].event_name, true);
}

void drawCarConfigurationScreen() {
	// Draw header bar
	drawHeaderBar(STATIC_MODE);
	BSP_LCD_SetTextColor(primary_back_color);
	BSP_LCD_FillRect(0, 27, screen_width, 27);

	// Draw page label
	BSP_LCD_SetBackColor(primary_back_color);
	BSP_LCD_SetTextColor(primary_text_color);
	BSP_LCD_SetFont(&Font_RobotoMedium24);
	BSP_LCD_DisplayStringAt(screen_margin - 10, 24 + ((31 / 2) - 12), "Car Config", CENTER_MODE);

	// Draw settings
	uint8_t startingYPos = 54;
	drawMenuItemWithValue(startingYPos + (menu_line_height * 0), "Reg Braking", current_driver.car_configuration.regen_braking, false);
    drawMenuItemWithValue(startingYPos + (menu_line_height * 1), "Torq Vector", current_driver.car_configuration.torque_vectoring, true);
    drawMenuItemWithValue(startingYPos + (menu_line_height * 2), "Dash Bright", current_driver.car_configuration.dash_led_brightness, true);
    drawMenuItemWithValue(startingYPos + (menu_line_height * 3), "Scrn Bright", current_driver.car_configuration.screen_brightness, true);
}
void updateCarConfigurationScreen() {
	// Draw settings
	uint8_t startingYPos = 54;
    updateMenuItemWithValue(startingYPos + (menu_line_height * 0), "Reg Brake", current_driver.car_configuration.regen_braking, false);
    updateMenuItemWithValue(startingYPos + (menu_line_height * 1), "Torq Vect", current_driver.car_configuration.torque_vectoring, false);
    updateMenuItemWithValue(startingYPos + (menu_line_height * 2), "Dash Bright", current_driver.car_configuration.dash_led_brightness, false);
    updateMenuItemWithValue(startingYPos + (menu_line_height * 3), "Scrn Bright", current_driver.car_configuration.screen_brightness, false);
}

/* UI Components */
void drawHeaderBar(Drive_Mode drive_mode){
	// Draw background bar
	BSP_LCD_SetBackColor(primary_back_color);
	BSP_LCD_SetTextColor(primary_back_color);
	BSP_LCD_FillRect(0, 0, screen_width, 27);

	// Draw drive mode
	char* drive_mode_label = "SM"; // Defaults to Static Mode
	if (drive_mode == READY_TO_DRIVE) {
		drive_mode_label = "RTD";
	}

	BSP_LCD_SetTextColor(primary_text_color);
	BSP_LCD_SetFont(&Font_RobotoMedium24);
	BSP_LCD_DisplayStringAt(0, 0, drive_mode_label, CENTER_MODE);
}

void drawAccumulatorDeltaBar(uint16_t yPos) {
	uint8_t barHeight = 90;
	uint16_t barWidth = screen_width - (screen_margin * 2);
	uint16_t maxDelta = (barWidth / 2) - 2; // -2 is for center line and end border
	uint16_t currentDelta = abs(maxDelta * accumul_delta);

	// Draw outline
	BSP_LCD_SetTextColor(primary_back_color);
	BSP_LCD_DrawRect(screen_margin, yPos, barWidth, barHeight); // Outer stroke
	BSP_LCD_DrawRect(screen_margin + 1, yPos + 1, barWidth - 2, barHeight - 2); // Inner stroke

	// Draw center line
	BSP_LCD_SetTextColor(primary_back_color);
	BSP_LCD_DrawLine(screen_margin + (barWidth/2) - 1, yPos, screen_margin + (barWidth/2) - 1, yPos + barHeight - 1);
	BSP_LCD_DrawLine(screen_margin + (barWidth/2), yPos, screen_margin + (barWidth/2), yPos + barHeight - 1);
	BSP_LCD_DrawLine(screen_margin + (barWidth/2) + 1, yPos, screen_margin + (barWidth/2) + 1, yPos + barHeight - 1);


	// Draw inner fill and percentage text
	BSP_LCD_SetFont(&Font_RobotoBlack36);
	uint8_t displayText[5];
	sprintf(displayText, "%.2f", accumul_delta);
	if (accumul_delta < 0) {
		BSP_LCD_SetTextColor(warning_color);
		BSP_LCD_FillRect((screen_margin + (barWidth / 2)) - currentDelta - 2, yPos + 2, currentDelta, barHeight - 4);

		BSP_LCD_SetTextColor(primary_text_color);
		BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
		BSP_LCD_DisplayStringAt(0, yPos + (barHeight / 2) - 21, displayText, CENTER_MODE);
	}
	else {
		BSP_LCD_SetTextColor(nominal_color);
		BSP_LCD_FillRect(screen_margin + (barWidth / 2) + 2, yPos + 2, currentDelta + 1, barHeight - 4);

		BSP_LCD_SetTextColor(primary_text_color);
		BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
		BSP_LCD_DisplayStringAt(0, yPos + (barHeight / 2) -  21, displayText, CENTER_MODE);
	}

	

}

void updateAccumulatorDeltaBar(uint16_t yPos) {
	uint8_t barHeight = 90;
	uint16_t barWidth = screen_width - (screen_margin * 2);
	uint16_t maxDelta = (barWidth / 2) - 2; // -2 is for center line and end border
	uint16_t currentDelta = abs(maxDelta * accumul_delta);

	// Draw inner fill and percentage text
	BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
	BSP_LCD_SetFont(&Font_RobotoBlack36);
	uint8_t displayText[5];
	sprintf(displayText, "%.2f", accumul_delta);
	if (accumul_delta < 0) {
		BSP_LCD_SetTextColor(warning_color);
		BSP_LCD_FillRect((screen_margin + (barWidth / 2)) - currentDelta - 2, yPos + 2, currentDelta, barHeight - 4);

		BSP_LCD_SetTextColor(primary_text_color);
		BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
		BSP_LCD_DisplayStringAt(0, yPos + (barHeight / 2) - 21, displayText, CENTER_MODE);
	}
	else {
		BSP_LCD_SetTextColor(nominal_color);
		BSP_LCD_FillRect(screen_margin + (barWidth / 2) + 2, yPos + 2, currentDelta + 1, barHeight - 4);

		BSP_LCD_SetTextColor(primary_text_color);
		BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
		BSP_LCD_DisplayStringAt(0, yPos + (barHeight / 2) -  21, displayText, CENTER_MODE);
	}
}

void drawAccumulatorTotalBar(uint16_t yPos) {
	uint16_t currentCharge = accumul_charge;
	uint8_t barHeight = 90;
	uint16_t barWidth = screen_width - (screen_margin * 2);

	// Draw outline
	BSP_LCD_SetTextColor(primary_back_color);
	BSP_LCD_DrawRect(screen_margin, yPos, barWidth, barHeight); // Outer stroke
	BSP_LCD_DrawRect(screen_margin + 1, yPos + 1, barWidth - 2, barHeight - 2); // Inner stroke

	if(accumul_charge < 0){
		accumul_charge = 0;
	}

	// Draw inner fill
	BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
	BSP_LCD_FillRect(screen_margin + 2, yPos + 2, currentCharge, barHeight - 4);

	// Draw percentage text
	uint8_t displayText[5];
	sprintf(displayText, "%.0f", accumul_charge);

	BSP_LCD_SetTextColor(primary_text_color);
	BSP_LCD_SetFont(&Font_RobotoBlack36);
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAt(0, yPos + (barHeight/2) - 21, displayText, CENTER_MODE);
}
void updateAccumulatorTotalBar(uint16_t yPos) {
	uint8_t barHeight = 90;
	uint16_t barWidth = screen_width - (screen_margin * 2);

	// Clear inner fill
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_FillRect(screen_margin + 2, yPos + 2, barWidth - 4, barHeight - 4);

	if(accumul_charge < 0){
		accumul_charge = 0;
	}

	// Draw inner fill
	BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
	BSP_LCD_FillRect(screen_margin + 2, yPos + 2, accumul_charge, barHeight - 4);

	// Draw percentage text
	uint8_t displayText[5];
	sprintf(displayText, "%.0f", accumul_charge);

	BSP_LCD_SetTextColor(primary_text_color);
	BSP_LCD_SetFont(&Font_RobotoBlack36);
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAt(0, yPos + (barHeight / 2) - 21, displayText, CENTER_MODE);
}

void drawReading(uint16_t y, uint8_t* label, uint8_t* value, bool draw_borders) {
	// Draw outline
	BSP_LCD_SetTextColor(primary_back_color);
	if (draw_borders) {
		// Top line
		BSP_LCD_DrawHLine(0, y, screen_width);
		// Bottom line
		BSP_LCD_DrawHLine(0, y + reading_line_height, screen_width);
	}
	// Value vertical divider
	BSP_LCD_DrawLine(244, y, 244, y + reading_line_height);

	// Draw label text
	BSP_LCD_SetTextColor(primary_text_color);
	BSP_LCD_SetFont(&Font_RobotoMedium26);
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAt(15, y + ((reading_line_height / 2) - 13), label, LEFT_MODE);

	// Draw value text
	BSP_LCD_SetFont(&Font_RobotoMedium28);
	BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
	BSP_LCD_DisplayStringAt(screen_width / 2 - 28, y + ((reading_line_height / 2) - 16), value, CENTER_MODE);
}
void updateReading(uint16_t y, uint8_t* value, uint32_t text_color) {
    // Update value text
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(text_color);
    BSP_LCD_SetFont(&Font_RobotoMedium28);
    BSP_LCD_DisplayStringAt(screen_width / 2 - 28, y + ((reading_line_height / 2) - 16), value, CENTER_MODE);
}

void drawMenuItem(uint16_t y, uint8_t* label, bool draw_borders) {
	// Draw outline
	BSP_LCD_SetTextColor(primary_back_color);
	if (draw_borders) {
		// Top line
		BSP_LCD_DrawHLine(0, y, screen_width);
		// Bottom line
		BSP_LCD_DrawHLine(0, y + menu_line_height, screen_width);
	}

	// Draw label text
	BSP_LCD_SetTextColor(primary_text_color);
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_LCD_SetFont(&Font_RobotoMedium26);
	BSP_LCD_DisplayStringAt(0, y + ((menu_line_height / 2) - 13), label, CENTER_MODE);
}
void updateMenuItem(uint16_t y, uint8_t* label, bool selected) {
    // Draw label text
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(primary_text_color);
    if(selected) BSP_LCD_SetTextColor(selection_color);
    BSP_LCD_SetFont(&Font_RobotoMedium26);
    BSP_LCD_DisplayStringAt(0, y + ((menu_line_height / 2) - 13), label, CENTER_MODE);
}

void drawMenuItemWithValue(uint16_t y, uint8_t* label, uint8_t value, bool draw_borders) {
    // Draw outline
    BSP_LCD_SetTextColor(primary_back_color);
    if (draw_borders) {
        // Top line
        BSP_LCD_DrawHLine(0, y, screen_width);
        // Bottom line
        BSP_LCD_DrawHLine(0, y + menu_line_height, screen_width);
    }
    // Value vertical divider
    BSP_LCD_DrawLine(244, y, 244, y + menu_line_height);

    // Draw label text
    BSP_LCD_SetTextColor(primary_text_color);
    BSP_LCD_SetFont(&Font_RobotoMedium26);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_DisplayStringAt(15, y + ((menu_line_height / 2) - 13), label, LEFT_MODE);

    // Draw value text
	uint8_t displayValue[5];
	sprintf(displayValue, "%.0f", value);
    BSP_LCD_SetFont(&Font_RobotoBlack26);
    BSP_LCD_DisplayStringAt(screen_width / 2 - 28, y + ((menu_line_height / 2) - 13), displayValue, CENTER_MODE);
}
void updateMenuItemWithValue(uint16_t y, uint8_t* label, uint8_t value, bool selected) {
    // Highlights both label and value of menu item if selected
    if(selected) BSP_LCD_SetTextColor(selection_color);
    else BSP_LCD_SetTextColor(primary_text_color);

    // Draw label text
    BSP_LCD_SetFont(&Font_RobotoBlack26);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_DisplayStringAt(15, y + ((menu_line_height / 2) - 12), label, LEFT_MODE);

    // Draw value text
    BSP_LCD_DisplayStringAt(screen_width / 2 - 28, y + ((menu_line_height / 2) - 13), value, CENTER_MODE);
}

void drawWarning() {
	// Draw divider bar
	BSP_LCD_SetTextColor(warning_color);
	BSP_LCD_FillRect(0, 217, screen_width, 31);

	// Write warning text
	BSP_LCD_SetTextColor(primary_text_color);
	//BSP_LCD_DisplayStringAt(screen_width / 2 - 35, y + ((menu_line_height / 2) - 12), value, CENTER_MODE);

	// Draw surrounding borders

}
