/*
 * 	File: car_settings.c
 *  Created on: 17 Nov 2020
 *  Author: James Hoskin
 */

#pragma once
#include <stdint.h>

typedef enum Drive_Mode
{
    STATIC_MODE,
    READY_TO_DRIVE
} Drive_Mode;

typedef struct Car_Configuration
{
    uint8_t regen_braking;
    uint8_t torque_vectoring;
    uint8_t dash_led_brightness;
    uint8_t screen_brightness;
} Car_Configuration;

typedef struct Driver_Profile
{
    uint8_t* driver_name;
    Car_Configuration car_configuration;
} Driver_Profile;

typedef struct Event_Profile
{
    uint8_t* event_name;
    Car_Configuration car_configuration;
} Event_Profile;

void changeDriver(Driver_Profile driver_profile);
void changeEvent(Event_Profile event_profile);
void updateCarConfiguration();
