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
#include <lvgl.h>
#include <stdio.h>

/*********************
 *      DEFINES
 *********************/

/*Sizes*/
#define QUTMS_SCREEN_HEADER_H (LV_VER_RES * 0.15)
#define QUTMS_BTNMATRIX_H (LV_VER_RES * 0.85)
#define QUTMS_ACCUM_BARS_H (LV_VER_RES * 0.18)
#define QUTMS_RTD_LABEL_H (LV_VER_RES * 0.1)

/*Colours*/
#define QUTMS_COLOR_ORANGE LV_COLOR_MAKE(0xFF, 0x87, 0x00)
#define QUTMS_COLOR_BLUE LV_COLOR_MAKE(0x00, 0x59, 0xA6)


/**********************
 *  STATIC PROTOTYPES
 **********************/
static void RTD_open(void);
static void event_select_open(void);
static void driver_select_open(void);


LV_EVENT_CB_DECLARE(page_open_cb);
LV_EVENT_CB_DECLARE(RTD_Selected_cb);
LV_EVENT_CB_DECLARE(driver_selected_cb);
LV_EVENT_CB_DECLARE(event_selected_cb);
/**********************
 *  STATIC VARIABLES
 **********************/

static lv_obj_t * bg_cont;
static lv_obj_t * header_cont;
static lv_obj_t * header_label;
static lv_obj_t * btnMatrix;
static const char * btn_map_home[] = {"RTD", "\n",
                                     "Driver Select", "\n", 
                                     "Event Select", "\n",
                                     "Car Config", "\n",
                                     "Advanced Config", ""};

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
static lv_style_t rtd_cont;
static lv_style_t rtd_slider;

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
    lv_style_set_bg_color(&heading, LV_STATE_DEFAULT, LV_COLOR_NAVY);
    lv_style_set_border_color(&heading, LV_STATE_DEFAULT, QUTMS_COLOR_ORANGE);
    lv_style_set_border_side(&heading, LV_STATE_DEFAULT, LV_BORDER_SIDE_BOTTOM);
    lv_style_set_border_width(&heading, LV_STATE_DEFAULT, 3);
    lv_style_set_radius(&heading, LV_STATE_DEFAULT, 0);

    //heading label
    lv_style_set_text_color(&label, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    
    //button matrix background
    lv_style_init(&matrixBG);
    lv_style_set_bg_color(&matrixBG, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_pad_all(&matrixBG, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_inner(&matrixBG, LV_STATE_DEFAULT, 0);
    lv_style_set_radius(&matrixBG, LV_STATE_DEFAULT, 0);
    
    //button matrix buttons
    lv_style_init(&buttons);
    lv_style_set_bg_color(&buttons, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_bg_color(&buttons, LV_STATE_CHECKED, LV_COLOR_BLACK);
    lv_style_set_text_color(&buttons, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_color(&buttons, LV_STATE_CHECKED, QUTMS_COLOR_ORANGE);
    lv_style_set_radius(&buttons, LV_STATE_DEFAULT, 0);

    //RTD containers
    lv_style_init(&rtd_cont);
    lv_style_set_bg_color(&rtd_cont, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_border_side(&rtd_cont, LV_STATE_DEFAULT, LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_BOTTOM);
    lv_style_set_border_color(&rtd_cont, LV_STATE_DEFAULT, LV_COLOR_NAVY);
    lv_style_set_border_width(&rtd_cont, LV_STATE_DEFAULT, 2);
    lv_style_set_radius(&rtd_cont, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_all(&rtd_cont, LV_STATE_DEFAULT, 2);

    //RTD sliders
    lv_style_init(&rtd_slider);
    lv_style_set_bg_color(&rtd_slider, LV_STATE_DEFAULT, QUTMS_COLOR_ORANGE);
    lv_style_set_radius(&rtd_slider, LV_STATE_DEFAULT, 0);

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
    lv_obj_set_state(header_cont, LV_STATE_DISABLED);

    /** 
     * Label for header
     */
    header_label = lv_label_create(bg_cont, NULL);
    lv_obj_add_style(header_label, LV_LABEL_PART_MAIN, &label);
    lv_obj_align(header_label, header_cont, LV_ALIGN_IN_LEFT_MID, 60, 0);
    lv_label_set_text(header_label, "Home");

    
    /** 
     * Button Matrix for home
     */
    btnMatrix = lv_btnmatrix_create(bg_cont, NULL);
    lv_btnmatrix_set_map(btnMatrix, btn_map_home);
    lv_obj_add_style(btnMatrix, LV_BTNMATRIX_PART_BG, &matrixBG);
    lv_obj_add_style(btnMatrix, LV_BTNMATRIX_PART_BTN, &buttons);
    lv_obj_set_size(btnMatrix, LV_HOR_RES, QUTMS_BTNMATRIX_H);
    lv_obj_align(btnMatrix, header_cont, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_btnmatrix_set_btn_ctrl_all(btnMatrix, LV_BTNMATRIX_CTRL_CLICK_TRIG);
    lv_obj_set_event_cb(btnMatrix, page_open_cb);
    
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static void RTD_open(void)
{
    lv_obj_del(btnMatrix);
    
    //Label
    lv_label_set_text(header_label, "RTD");
    lv_obj_align(header_label, header_cont, LV_ALIGN_IN_LEFT_MID, 65, 0);

    //Accumulator Delta bar
    lv_obj_t * accum_delta_bar = lv_bar_create(bg_cont, NULL);
    lv_obj_add_style(accum_delta_bar, LV_BAR_PART_BG, &rtd_cont);
    lv_obj_add_style(accum_delta_bar, LV_BAR_PART_INDIC, &rtd_slider);
    lv_obj_align(accum_delta_bar, header_cont, LV_ALIGN_OUT_BOTTOM_LEFT, 0, -3);
    lv_obj_set_size(accum_delta_bar, LV_HOR_RES, QUTMS_ACCUM_BARS_H);
    lv_bar_set_range(accum_delta_bar, 0, 50);
    lv_bar_set_value(accum_delta_bar, 25, LV_ANIM_OFF);


    //Accumulator Total bar
    lv_obj_t * accum_total_bar = lv_bar_create(bg_cont, NULL);
    lv_obj_add_style(accum_total_bar, LV_BAR_PART_BG, &rtd_cont);
    lv_obj_add_style(accum_total_bar, LV_BAR_PART_INDIC, &rtd_slider);
    lv_obj_align(accum_total_bar, accum_delta_bar, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_obj_set_size(accum_total_bar, LV_HOR_RES, QUTMS_ACCUM_BARS_H);
    lv_bar_set_range(accum_total_bar, 0, 100);

    //Accumulator Voltage cont + label
    lv_obj_t * accum_volt_label = lv_cont_create(bg_cont, NULL);
    lv_obj_add_style(accum_volt_label, LV_CONT_PART_MAIN, &rtd_cont);
    lv_obj_align(accum_volt_label, accum_total_bar, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_obj_set_size(accum_volt_label, LV_HOR_RES, QUTMS_RTD_LABEL_H);

    //Accumulator Current container + label
    lv_obj_t * accum_current_label = lv_cont_create(bg_cont, NULL);
    lv_obj_add_style(accum_current_label, LV_CONT_PART_MAIN, &rtd_cont);
    lv_obj_align(accum_current_label, accum_volt_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_obj_set_size(accum_current_label, LV_HOR_RES, QUTMS_RTD_LABEL_H);

    //Gearbox Current container + label
    lv_obj_t * gearbox_current_label = lv_cont_create(bg_cont, NULL);
    lv_obj_add_style(gearbox_current_label, LV_CONT_PART_MAIN, &rtd_cont);
    lv_obj_align(gearbox_current_label, accum_current_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_obj_set_size(gearbox_current_label, LV_HOR_RES, QUTMS_RTD_LABEL_H);

    //Inverter C container + label
    lv_obj_t * inverter_current_label = lv_cont_create(bg_cont, NULL);
    lv_obj_add_style(inverter_current_label, LV_CONT_PART_MAIN, &rtd_cont);
    lv_obj_align(inverter_current_label, gearbox_current_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_obj_set_size(inverter_current_label, LV_HOR_RES, QUTMS_RTD_LABEL_H);

    //Motor container + label
    lv_obj_t * motor_current_label = lv_cont_create(bg_cont, NULL);
    lv_obj_add_style(motor_current_label, LV_CONT_PART_MAIN, &rtd_cont);
    lv_obj_align(motor_current_label, inverter_current_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_obj_set_size(motor_current_label, LV_HOR_RES, QUTMS_RTD_LABEL_H);

}

static void driver_select_open(void)
{
    //Label
    lv_label_set_text(header_label, "Driver Selection");
    lv_obj_align(header_label, header_cont, LV_ALIGN_IN_LEFT_MID, 25, 0);

    //button matrix
    lv_btnmatrix_set_map(btnMatrix, btn_map_drivers);
    lv_obj_add_style(btnMatrix, LV_BTNMATRIX_PART_BG, &matrixBG);
    lv_obj_add_style(btnMatrix, LV_BTNMATRIX_PART_BTN, &buttons);
    lv_obj_set_size(btnMatrix, LV_HOR_RES, QUTMS_BTNMATRIX_H);
    lv_obj_align(btnMatrix, header_cont, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_btnmatrix_set_btn_ctrl_all(btnMatrix, LV_BTNMATRIX_CTRL_CLICK_TRIG);
    lv_obj_set_event_cb(btnMatrix, driver_selected_cb);
}

static void event_select_open(void)
{
    //Label
    lv_label_set_text(header_label, "Event Selection");
    lv_obj_align(header_label, header_cont, LV_ALIGN_IN_LEFT_MID, 25, 0);

    //button matrix
    lv_btnmatrix_set_map(btnMatrix, btn_map_events);
    lv_obj_add_style(btnMatrix, LV_BTNMATRIX_PART_BG, &matrixBG);
    lv_obj_add_style(btnMatrix, LV_BTNMATRIX_PART_BTN, &buttons);
    lv_obj_set_size(btnMatrix, LV_HOR_RES, QUTMS_BTNMATRIX_H);
    lv_obj_align(btnMatrix, header_cont, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_btnmatrix_set_btn_ctrl_all(btnMatrix, LV_BTNMATRIX_CTRL_CLICK_TRIG);
    lv_obj_set_event_cb(btnMatrix, event_selected_cb);
}

LV_EVENT_CB_DECLARE(page_open_cb)
{
    if( e != LV_EVENT_VALUE_CHANGED) return;
    const char* button_name = lv_btnmatrix_get_active_btn_text(btnMatrix);
    if (strcmp(button_name, "RTD") == 0)
    {
        printf("%s\n", button_name);
        RTD_open();
    }
    else if (strcmp(button_name, "Driver Select") == 0)
    {
        printf("%s\n", button_name);
        driver_select_open();
    }
    else if (strcmp(button_name, "Event Select") == 0)
    {
        printf("%s\n", button_name);
        event_select_open();
    }
    else if (strcmp(button_name, "Car Config") == 0)
    {
        printf("%s\n", button_name);
        //TODO make Car Config page
    }
    else if(strcmp(button_name, "Advanced Config") == 0)
    {
        printf("%s\n", button_name);
        //TODO make Advanced Config page
    }

       
}

LV_EVENT_CB_DECLARE(RTD_Selected_cb)
{
    //TODO
}

LV_EVENT_CB_DECLARE(driver_selected_cb)
{
    if( e != LV_EVENT_VALUE_CHANGED) return;
    const char* button_name = lv_btnmatrix_get_active_btn_text(btnMatrix);
    if (strcmp(button_name, "QUTMS") == 0)
    {
        printf("%s\n", button_name);
        lv_obj_clean(lv_scr_act());
        screen_create();
    }
    else if (strcmp(button_name, "Matt Grant") == 0)
    {
        printf("%s\n", button_name);
        lv_obj_clean(lv_scr_act());
        screen_create();
    }
    else if (strcmp(button_name, "Danny Ric") == 0)
    {
        printf("%s\n", button_name);
        lv_obj_clean(lv_scr_act());
        screen_create();
    }
    else if(strcmp(button_name, "Lewis Hamilton") == 0)
    {
        printf("%s\n", button_name);
        lv_obj_clean(lv_scr_act());
        screen_create();
    }    
}

LV_EVENT_CB_DECLARE(event_selected_cb)
{
    if( e != LV_EVENT_VALUE_CHANGED) return;
    const char* button_name = lv_btnmatrix_get_active_btn_text(btnMatrix);
    if (strcmp(button_name, "Endurance") == 0)
    {
        printf("%s\n", button_name);
        lv_obj_clean(lv_scr_act());
        screen_create();
    }
    else if (strcmp(button_name, "Acceleration") == 0)
    {
        printf("%s\n", button_name);
        lv_obj_clean(lv_scr_act());
        screen_create();
    }
    else if (strcmp(button_name, "SkidPad") == 0)
    {
        printf("%s\n", button_name);
        lv_obj_clean(lv_scr_act());
        screen_create();
    }
    else if(strcmp(button_name, "AutoCross") == 0)
    {
        printf("%s\n", button_name);
        lv_obj_clean(lv_scr_act());
        screen_create();
    }

       
}
