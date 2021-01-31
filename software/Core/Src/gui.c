/*
 *	File: gui.c
 *  Created on: 12 Nov 2020
 *  Author: James Hoskin
 */
#include "gpio.h"
#include "gui.h"
#include "stm32_adafruit_lcd.h"
#include <stdint.h>
#include <string.h>
#include "car_settings.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "QUTMS.h"

extern Driver_Profile drivers[4], current_driver;
extern Event_Profile events[4];
extern uint8_t selected_menu_option, max_menu_option;
extern float accumul_volts, accumul_temp, gearbox_temp, inverter_temp, motor_temp, accumul_charge, accumul_delta;
extern float drawn_accumul_volts, drawn_accumul_temp, drawn_gearbox_temp, drawn_inverter_temp, drawn_motor_temp, drawn_accumul_charge, drawn_accumul_delta;
extern uint8_t total_laps, current_lap;
char* menus[4] = { "Driver Select", "Event Select", "Car Config", "Advanced" };
bool menu_pot_incremented, menu_pot_decremented, menu_pot_pressed;
extern UI_Screen current_screen;

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
            current_screen = SM_SCREEN;
			max_menu_option = 4;
            break;
        case DRIVER_SELECTION_SCREEN:
            drawDriverSelectionScreen();
            current_screen = DRIVER_SELECTION_SCREEN;
			max_menu_option = sizeof(drivers) / sizeof(drivers[0]);
            break;
        case EVENT_SELECTION_SCREEN:
            drawEventSelectionScreen();
            current_screen = EVENT_SELECTION_SCREEN;
			max_menu_option = sizeof(events) / sizeof(events[0]);
            break;
        case CAR_CONFIGURATION_SCREEN:
            drawCarConfigurationScreen();
            current_screen = CAR_CONFIGURATION_SCREEN;
			max_menu_option = 4;
            break;
        default:
            drawRTDScreen();
            current_screen = RTD_SCREEN;
			max_menu_option = 0;
            break;
    }
}

bool updateMenuScroll() {
	if (menu_pot_incremented && selected_menu_option < max_menu_option) {
		selected_menu_option++;
		return true;
	}
	else if (menu_pot_decremented && selected_menu_option > 0) {
		selected_menu_option--;
		return true;
	}
	return false;
}

void updateCarConfiguration(){
	if(menu_pot_incremented){
		switch(selected_menu_option){
			case 0:
				current_driver.car_configuration.regen_braking += 1;
				break;
			case 1:
				current_driver.car_configuration.torque_vectoring += 1;
				break;
			case 2:
				current_driver.car_configuration.dash_led_brightness += 1;
				break;
			case 3:
				current_driver.car_configuration.screen_brightness += 1;
				break;
		}
	}
	else if(menu_pot_decremented){
		switch(selected_menu_option){
			case 0:
				current_driver.car_configuration.regen_braking -= 1;
				break;
			case 1:
				current_driver.car_configuration.torque_vectoring -= 1;
				break;
			case 2:
				current_driver.car_configuration.dash_led_brightness -= 1;
				break;
			case 3:
				current_driver.car_configuration.screen_brightness -= 1;
				break;
		}
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
	drawPrimaryHeaderBar(READY_TO_DRIVE);

	// Draw accumulator bars
	drawAccumulatorDeltaBar(38);
	drawAccumulatorTotalBar(138);

	// Draw divider bar
	BSP_LCD_SetTextColor(primary_back_color);
	BSP_LCD_FillRect(0, 240, 320, 5);

	// Draw readings
	char* reading[5] = { "Accumul (V)", "Accumul (C)", "Gearbox (C)", "Invertr (C)", "Motor   (C)"};
	uint16_t startingXPos = 245;
	for (int i = 0; i < 5; i++) {
		drawReading(startingXPos + (reading_line_height * i), reading[i], "?", i != 0 && i != 4 ? true : false);
	}
}

void updateRTDScreen() {
    // Update accumulator bars
    updateAccumulatorDeltaBar(38);
    updateAccumulatorTotalBar(138);

    // Update readings
    uint8_t reading_value[3];
    uint16_t startingYPos = 245;

    // Accumul. (v)
    if(accumul_volts != drawn_accumul_volts){
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

    	drawn_accumul_volts = accumul_volts;
    }


    // Accumul. (C)
    if(accumul_temp != drawn_accumul_temp){
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
		drawn_accumul_temp = accumul_temp;
    }

    // Gearbox  (C)
	if(gearbox_temp != drawn_gearbox_temp){
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
		drawn_gearbox_temp = gearbox_temp;
	}

    // Inverter (C)
	if(inverter_temp != drawn_inverter_temp){
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
		drawn_inverter_temp = inverter_temp;
	}

    // Motor    (C)
	if(motor_temp != drawn_motor_temp){
		clearArea(screen_width - 60, startingYPos + (reading_line_height * 4) + ((reading_line_height / 2) - 12), 90, 24);
		sprintf(reading_value, "%.0f", motor_temp);
		if (motor_temp > 50) {
			updateReading(startingYPos + (reading_line_height * 4), reading_value, warning_color);
		}
		else if (motor_temp <= 50 && motor_temp > 30) {
			updateReading(startingYPos + (reading_line_height * 4), reading_value, caution_color);
		}
		else {
			updateReading(startingYPos + (reading_line_height * 4), reading_value, nominal_color);
		}
		drawn_motor_temp = motor_temp;
	}

}

void drawSMScreen() {
	// Draw header bar
	drawPrimaryHeaderBar(STATIC_MODE);

	// Draw accumulator bars
	drawAccumulatorDeltaBar(38);
	drawAccumulatorTotalBar(138);

	// Draw divider bar
	BSP_LCD_SetTextColor(primary_back_color);
	BSP_LCD_FillRect(0, 240, screen_width, 5);

	// Draw configuration options
	uint16_t startingXPos = 245;
	for (int i = 0; i < 4; i++) {
		drawMenuItem(startingXPos + (menu_line_height * i), menus[i], selected_menu_option == i ? true : false, i != 0 && i != 3 ? true : false);
	}
}

void updateSMScreen() {
    // Update accumulator bar
    updateAccumulatorTotalBar(138);

    bool update_menu = updateMenuScroll();

    if(update_menu){
    	// Draw menu of screens
		uint16_t startingXPos = 245;
		for (int i = 0; i < 4; i++) {
			clearArea(0, startingXPos + (menu_line_height * i) + ((menu_line_height / 2) - 12), screen_width, 24);
			if (i == selected_menu_option) {
				updateMenuItem(startingXPos + (menu_line_height * i), menus[i], true);
			}
			else {
				updateMenuItem(startingXPos + (menu_line_height * i), menus[i], false);
			}
		}
    }
}

void drawDriverSelectionScreen() {
	// Draw header bar
	drawPrimaryHeaderBar(STATIC_MODE);
	drawSecondaryHeaderBar("Driver Select");

	// Draw settings
	uint8_t startingYPosition = 62;
	for (int i = 0; i < 4; i++) {
		drawMenuItem(startingYPosition + (menu_line_height * i), drivers[i].driver_name, selected_menu_option == i ? true : false, i != 0 ? true : false);
	}
}
void updateDriverSelectionScreen(){
	uint8_t startingYPosition = 62;

	bool update_menu = updateMenuScroll();

	if(update_menu){
		// Draw driver options
		for (int i = 0; i < 4; i++) {
			updateMenuItem(startingYPosition + (menu_line_height * i), drivers[i].driver_name, selected_menu_option == i ? true : false);
		}
	}
}

void drawEventSelectionScreen() {
	// Draw header bar
	drawPrimaryHeaderBar(STATIC_MODE);
	drawSecondaryHeaderBar("Event Select");

	// Draw settings
	uint8_t startingYPosition = 62;
	for (int i = 0; i < 4; i++) {
		drawMenuItem(startingYPosition + (menu_line_height * i), events[i].event_name, selected_menu_option == i ? true : false, i != 0 ? true : false);
	}
}
void updateEventSelectionScreen() {
	uint8_t startingYPosition = 62;

	bool update_menu = updateMenuScroll();

	if(update_menu){
		// Draw event options
		for (int i = 0; i < 4; i++) {
			updateMenuItem(startingYPosition + (menu_line_height * i), events[i].event_name, selected_menu_option == i ? true : false);
		}
	}
}

void drawCarConfigurationScreen() {
	selected_menu_option = 2;
	// Draw header bar
	drawPrimaryHeaderBar(STATIC_MODE);
	drawSecondaryHeaderBar("Car Config");

	// Draw settings
	uint8_t startingYPos = 62;
	uint8_t yOffset = menu_line_height + 46;
	drawMenuItemWithValue(startingYPos + (yOffset * 0), "Regen Braking", current_driver.car_configuration.regen_braking, selected_menu_option == 0, false);
    drawMenuItemWithValue(startingYPos + (yOffset * 1), "Torque Vector", current_driver.car_configuration.torque_vectoring, selected_menu_option == 1, true);
    drawMenuItemWithValue(startingYPos + (yOffset * 2), "Dash Bright", current_driver.car_configuration.dash_led_brightness, selected_menu_option == 2, true);
    drawMenuItemWithValue(startingYPos + (yOffset * 3), "Screen Bright", current_driver.car_configuration.screen_brightness, selected_menu_option == 3, true);
}
void updateCarConfigurationScreen() {
	// Draw settings
	bool update_menu = updateMenuScroll() /*|| pot_incremented*/;

	uint8_t startingYPos = 62;
	uint8_t yOffset = menu_line_height + 46;
	if(update_menu){
		updateMenuItemWithValue(startingYPos + (yOffset * 0), "Regen Braking", current_driver.car_configuration.regen_braking, selected_menu_option == 0);
		updateMenuItemWithValue(startingYPos + (yOffset * 1), "Torque Vector", current_driver.car_configuration.torque_vectoring, selected_menu_option == 1);
		updateMenuItemWithValue(startingYPos + (yOffset * 2), "Dash Bright", current_driver.car_configuration.dash_led_brightness, selected_menu_option == 2);
		updateMenuItemWithValue(startingYPos + (yOffset * 3), "Screen Bright", current_driver.car_configuration.screen_brightness, selected_menu_option == 3);
	}
}

/* UI Components */
void drawPrimaryHeaderBar(Drive_Mode drive_mode){
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

void drawSecondaryHeaderBar(uint8_t* label){
	BSP_LCD_SetTextColor(primary_back_color);
	BSP_LCD_FillRect(0, 24, screen_width, 38);

	// Draw page label
	BSP_LCD_SetBackColor(primary_back_color);
	BSP_LCD_SetTextColor(primary_text_color);
	BSP_LCD_SetFont(&Font24);
	BSP_LCD_DisplayStringAt(0, 24 + ((38 / 2) - 12), label, CENTER_MODE);
}

void drawAccumulatorDeltaBar(uint16_t yPos) {
	uint8_t barHeight = 90;
	uint16_t barWidth = screen_width - (screen_margin * 2);
	uint16_t maxDelta = (barWidth / 2) - 5; // -2 is for center line and end border

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
	uint16_t target_fill = abs(maxDelta * accumul_delta);
	uint16_t centerXPos = (screen_margin + (barWidth / 2));

	BSP_LCD_SetFont(&Font_RobotoBlack36);
	uint8_t displayText[5];
	sprintf(displayText, "%.2f", accumul_delta);

	if(accumul_delta > 1 || accumul_delta < -1) return;

	if (accumul_delta < 0) {
		// Add missing fill
		uint16_t startingXPos = (centerXPos - 2) - target_fill;
		BSP_LCD_SetTextColor(warning_color);
		BSP_LCD_FillRect(startingXPos, yPos + 2, target_fill, barHeight - 4);
	}
	else {
		// Add missing fill
		uint16_t startingXPos = centerXPos + 2;
		BSP_LCD_SetTextColor(nominal_color);
		BSP_LCD_FillRect(startingXPos, yPos + 2, target_fill, barHeight - 4);
	}

	BSP_LCD_SetTextColor(primary_text_color);
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAt(0, yPos + (barHeight / 2) -  21, displayText, CENTER_MODE);

	drawn_accumul_delta = accumul_delta;

}

void updateAccumulatorDeltaBar(uint16_t yPos) {
	uint8_t barHeight = 90;
	uint16_t barWidth = screen_width - (screen_margin * 2);
	uint16_t maxDelta = (barWidth / 2) - 5; // -2 is for center line and end border

	// Draw inner fill and percentage text
	uint16_t target_fill = abs(maxDelta * accumul_delta);
	uint16_t current_fill = abs(drawn_accumul_delta * maxDelta);
	uint16_t centerXPos = (screen_margin + (barWidth / 2));

	BSP_LCD_SetFont(&Font_RobotoBlack36);
	uint8_t displayText[5];

	// Reduce liklihood of float addition inaccuracies eg. 1.000012
	accumul_delta = trunc(accumul_delta * 100) / 100;
	sprintf(displayText, "%.2f", accumul_delta);

	//TODO accumul_delta suffers from rounding error
	if(accumul_delta > 1 || accumul_delta < -1) return;

	// Clear drawn fill if switch from a negative delta to positive, vice versa
	if(accumul_delta < 0 && drawn_accumul_delta > 0){
		// Remove excess fill
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		uint16_t startingXPos = (centerXPos + 2);
		BSP_LCD_FillRect(startingXPos, yPos + 2, current_fill, barHeight - 4);
	}
	if(accumul_delta > 0 && drawn_accumul_delta < 0){
		// Remove excess fill
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		uint16_t startingXPos = (centerXPos - 2) - current_fill;
		BSP_LCD_FillRect(startingXPos, yPos + 2, current_fill, barHeight - 4);
	}

	if (accumul_delta < 0) {
		if(target_fill > current_fill){
			// Add missing fill
			BSP_LCD_SetTextColor(warning_color);
			uint16_t startingXPos = (centerXPos - 2) - current_fill - (target_fill - current_fill);
			BSP_LCD_FillRect(startingXPos, yPos + 2, target_fill - current_fill, barHeight - 4);
		}
		else{
			// Remove excess fill
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
			uint16_t startingXPos = ((centerXPos - current_fill) - 2);
			BSP_LCD_FillRect(startingXPos, yPos + 2, current_fill - target_fill, barHeight - 4);
		}
	}
	else {
		if(target_fill > current_fill){
			// Add missing fill
			BSP_LCD_SetTextColor(nominal_color);
			uint16_t startingXPos = (centerXPos + current_fill) + 2;
			BSP_LCD_FillRect(startingXPos, yPos + 2, target_fill - current_fill, barHeight - 4);
		}
		else{
			// Remove excess fill
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
			uint16_t startingXPos = ((centerXPos + current_fill) - (current_fill - target_fill)) + 2;
			BSP_LCD_FillRect(startingXPos, yPos + 2, current_fill - target_fill, barHeight - 4);
		}
	}

	BSP_LCD_SetTextColor(primary_text_color);
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAt(0, yPos + (barHeight / 2) -  21, displayText, CENTER_MODE);

	drawn_accumul_delta = accumul_delta;
}

void drawAccumulatorTotalBar(uint16_t yPos) {
	uint8_t barHeight = 90;
	uint16_t barWidth = (screen_width - (screen_margin * 2));
	uint16_t target_fill_width = (barWidth * accumul_charge) - 5;

	// Draw outline
	BSP_LCD_SetTextColor(primary_back_color);
	BSP_LCD_DrawRect(screen_margin, yPos, barWidth, barHeight); // Outer stroke
	BSP_LCD_DrawRect(screen_margin + 1, yPos + 1, barWidth - 2, barHeight - 2); // Inner stroke

	if(accumul_charge < 0 || accumul_charge > 1) return;

	// Draw inner fill
	BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
	BSP_LCD_FillRect(screen_margin + 2, yPos + 2, target_fill_width, barHeight - 4);

	// Draw percentage text
	uint8_t displayText[5];
	sprintf(displayText, "%.0f", 100 * accumul_charge);

	BSP_LCD_SetTextColor(primary_text_color);
	BSP_LCD_SetFont(&Font_RobotoBlack36);
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAt(0, yPos + (barHeight/2) - 21, displayText, CENTER_MODE);

	drawn_accumul_charge = accumul_charge;
}
void updateAccumulatorTotalBar(uint16_t yPos) {
	if(accumul_charge != drawn_accumul_charge){
		if(accumul_charge < 0 || accumul_charge > 1) return;
		uint8_t barHeight = 90;
		uint16_t barWidth = (screen_width - (screen_margin * 2));
		uint16_t current_fill_width = (barWidth * drawn_accumul_charge) - 5;
		uint16_t target_fill_width = (barWidth * accumul_charge) - 5;

		if(target_fill_width > current_fill_width){
			// Draw missing fill
			BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
			uint16_t startXPos = (screen_margin + 2) + current_fill_width;
			BSP_LCD_FillRect(startXPos, yPos + 2, (target_fill_width - current_fill_width), barHeight - 4);
		}
		else if(target_fill_width < current_fill_width){
			// Remove fill difference
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
			uint16_t startXPos = ((screen_margin + 2) + current_fill_width) - (current_fill_width - target_fill_width);
			BSP_LCD_FillRect(startXPos, yPos + 2, current_fill_width - target_fill_width, barHeight - 4);
		}

		// Draw percentage text
		uint8_t displayText[5];
		sprintf(displayText, "%.0f", 100 * accumul_charge);
		BSP_LCD_SetTextColor(primary_text_color);
		BSP_LCD_SetFont(&Font_RobotoBlack36);
		BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
		BSP_LCD_DisplayStringAt(0, yPos + (barHeight / 2) - 21, displayText, CENTER_MODE);

		drawn_accumul_charge = accumul_charge;
	}
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
	BSP_LCD_SetFont(&Font32);
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAt(15, y + ((reading_line_height / 2) - 17), label, LEFT_MODE);

	// Draw value text
	BSP_LCD_SetFont(&Font_RobotoMedium32);
	BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
	BSP_LCD_DisplayStringAt(screen_width / 2 - 36, y + ((reading_line_height / 2) - 17), value, CENTER_MODE);
}
void updateReading(uint16_t y, uint8_t* value, uint32_t text_color) {
    // Update value text
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(text_color);
    BSP_LCD_SetFont(&Font_RobotoMedium32);
    BSP_LCD_DisplayStringAt(screen_width / 2 - 36, y + ((reading_line_height / 2) - 17), value, CENTER_MODE);
}

void drawMenuItem(uint16_t y, uint8_t* label, bool selected, bool draw_borders) {
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
	if(selected) BSP_LCD_SetTextColor(selection_color);
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_LCD_SetFont(&Font32);
	BSP_LCD_DisplayStringAt(0, y + ((menu_line_height / 2) - 17), label, CENTER_MODE);
}
void updateMenuItem(uint16_t y, uint8_t* label, bool selected) {
    // Draw label text
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(primary_text_color);
    if(selected) BSP_LCD_SetTextColor(selection_color);
    BSP_LCD_SetFont(&Font_RobotoMedium26);
    BSP_LCD_DisplayStringAt(0, y + ((menu_line_height / 2) - 17), label, CENTER_MODE);
}

void drawMenuItemWithValue(uint16_t y, uint8_t* label, uint8_t value, bool selected, bool draw_borders) {
    // Draw outline
    BSP_LCD_SetTextColor(primary_back_color);
    if (draw_borders) {
        BSP_LCD_DrawHLine(0, y, screen_width);
    }

    // Draw label text
    BSP_LCD_SetTextColor(primary_text_color);
    if(selected) BSP_LCD_SetTextColor(selection_color);

    BSP_LCD_SetFont(&Font32);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_DisplayStringAt(0, y + ((menu_line_height / 2) - 16), label, CENTER_MODE);

    // Draw value text
	uint8_t displayValue[5];
	uint8_t valueYOffset = 42;
	sprintf(displayValue, "%d", value);
    BSP_LCD_SetFont(&Font_RobotoBlack32);
    BSP_LCD_DisplayStringAt(0, y + valueYOffset + ((menu_line_height / 2) - 16), displayValue, CENTER_MODE);
}
void updateMenuItemWithValue(uint16_t y, uint8_t* label, uint8_t value, bool selected) {
    // Highlights both label and value of menu item if selected
	BSP_LCD_SetTextColor(primary_text_color);
    if(selected) BSP_LCD_SetTextColor(selection_color);

    // Draw label text
    BSP_LCD_SetFont(&Font32);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_DisplayStringAt(0, y + ((menu_line_height / 2) - 16), label, CENTER_MODE);

    // Draw value text
    uint8_t display_value[5];
    uint8_t valueYOffset = 42;
    sprintf(display_value, "%d", value);
    BSP_LCD_SetFont(&Font_RobotoBlack32);
    BSP_LCD_DisplayStringAt(0, y + valueYOffset + (((menu_line_height * 2) / 2) - 16), display_value, CENTER_MODE);
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
