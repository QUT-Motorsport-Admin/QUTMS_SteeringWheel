/*
 *	File: screen.c
 *  Created on: 02 FEB 2021
 *  Author: Matt Grant
 * 
 *  Desc: Prototype for screen GUI to go in QEV3.
 *        Built with LVGL
 */

/*********************
 *      INCLUDES
 *********************/
#include "../lvgl/lvgl.h"
#include <stdio.h>

/*********************
 *      DEFINES
 *********************/

/*Sizes*/
#define QUTMS_SCREEN_HEADER_H (LV_VER_RES / 5)

/*Colours*/
#define QUTMS_COLOR_ORANGE LV_COLOR_MAKE(0xFF, 0x87, 0x00)
#define QUTMS_COLOR_BLUE LV_COLOR_MAKE(0x00, 0x59, 0xA6)


/**********************
 *  STATIC PROTOTYPES
 **********************/
void event_select_open(void);
void driver_select_open(void);
LV_EVENT_CB_DECLARE(driver_select_open_cb);
/**********************
 *  STATIC VARIABLES
 **********************/

static lv_obj_t * bg_cont;
static lv_obj_t * header_cont;
static lv_obj_t * header_label;
static lv_obj_t * btnMatrix;
static const char * btn_map_events[] = {"Endurance", "\n", 
                                     "Acceleration", "\n",
                                     "SkidPad", "\n",
                                     "AutoCross", ""};
static const char * btn_map_drivers[] = {"QUTMS", "\n", 
                                     "Matt Grant", "\n",
                                     "Danny Ric", "\n",
                                     "Lewis Hamilton", ""};

/*Styles*/
static lv_style_t background;
static lv_style_t heading;
static lv_style_t label;
static lv_style_t matrixBG;
static lv_style_t buttons;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void screen_create(void)
{
    /** 
     * Styles (to be deprecated here and moved to own file screen_theme.c)
     */    
    //Background   
    lv_style_init(&background);
    lv_style_set_bg_color(&background, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_radius(&background, LV_STATE_DEFAULT, 0);
    lv_style_set_margin_all(&background, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_all(&background, LV_STATE_DEFAULT, 0);

    //heading
    lv_style_init(&heading);
    lv_style_set_bg_color(&heading, LV_STATE_DEFAULT, QUTMS_COLOR_BLUE);
    lv_style_set_border_color(&heading, LV_STATE_DEFAULT, QUTMS_COLOR_ORANGE);
    lv_style_set_border_width(&heading, LV_STATE_DEFAULT, 2);

    //label
    lv_style_set_text_color(&label, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    

    //button matrix background
    lv_style_init(&matrixBG);
    lv_style_set_bg_color(&matrixBG, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    
    //button matrix buttons
    lv_style_init(&buttons);
    lv_style_set_bg_color(&buttons, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_bg_color(&buttons, LV_STATE_CHECKED, LV_COLOR_BLACK);
    lv_style_set_text_color(&buttons, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_color(&buttons, LV_STATE_CHECKED, QUTMS_COLOR_ORANGE);

    /** 
     * Container for background.
     */
    
    bg_cont = lv_cont_create(lv_scr_act(), NULL);
    lv_obj_add_style(bg_cont, LV_CONT_PART_MAIN, &background);
    lv_obj_set_size(bg_cont, LV_HOR_RES, LV_VER_RES);


    /** 
     * Container for header
     */
    header_cont = lv_cont_create(bg_cont, NULL);
    lv_obj_add_style(header_cont, LV_CONT_PART_MAIN, &heading);
    lv_obj_set_size(header_cont, LV_HOR_RES, QUTMS_SCREEN_HEADER_H);

    /** 
     * Label for header
     */
    header_label = lv_label_create(bg_cont, NULL);
    lv_obj_add_style(header_label, LV_LABEL_PART_MAIN, &label);
    lv_obj_align(header_label, header_cont, LV_ALIGN_IN_LEFT_MID, 25, 0);

    event_select_open();
    
    
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
void event_select_open(void)
{
    /** 
     * Clear old shit
     */
    //TODO

    /** 
     * Draw new shit
     */
    //Label
    lv_label_set_text(header_label, "Event Selection");

    //button matrix
    btnMatrix = lv_btnmatrix_create(bg_cont, NULL);
    lv_btnmatrix_set_map(btnMatrix, btn_map_events);
    lv_obj_add_style(btnMatrix, LV_BTNMATRIX_PART_BG, &matrixBG);
    lv_obj_add_style(btnMatrix, LV_BTNMATRIX_PART_BTN, &buttons);
    lv_obj_set_size(btnMatrix, LV_HOR_RES, LV_VER_RES * 0.8);
    lv_obj_align(btnMatrix, header_cont, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_btnmatrix_set_btn_ctrl_all(btnMatrix, LV_BTNMATRIX_CTRL_NO_REPEAT);
    // lv_btnmatrix_set_one_check(btnMatrix, true);
    lv_obj_set_event_cb(btnMatrix, driver_select_open_cb);

}

void driver_select_open(void)
{
    /** 
     * Clear old shit
     */
    //TODO
    lv_obj_del(btnMatrix);

    /** 
     * Draw new shit
     */
    //Label
    lv_label_set_text(header_label, "Driver Selection");

    //button matrix
    btnMatrix = lv_btnmatrix_create(bg_cont, NULL);
    lv_btnmatrix_set_map(btnMatrix, btn_map_drivers);
    lv_obj_add_style(btnMatrix, LV_BTNMATRIX_PART_BG, &matrixBG);
    lv_obj_add_style(btnMatrix, LV_BTNMATRIX_PART_BTN, &buttons);
    lv_obj_set_size(btnMatrix, LV_HOR_RES, LV_VER_RES * 0.8);
    lv_obj_align(btnMatrix, header_cont, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    // lv_btnmatrix_set_btn_ctrl_all(btnMatrix, LV_BTNMATRIX_CTRL_CLICK_TRIG);
    // lv_btnmatrix_set_one_check(btnMatrix, true);
    lv_obj_set_event_cb(btnMatrix, driver_select_open_cb);

}

LV_EVENT_CB_DECLARE(driver_select_open_cb)
{
    if( e != LV_EVENT_VALUE_CHANGED) return;
    const char* button_name = lv_btnmatrix_get_active_btn_text(btnMatrix);
    if (strcmp(button_name, "Endurance") == 0)
    {
        printf("1\n");
        driver_select_open();
    }
    else if(strcmp(button_name, "Matt Grant") == 0)
    {
        printf("2\n");
        event_select_open();
    }

       
}
