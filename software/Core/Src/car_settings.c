/*
 *  File: car_settings.c
 *  Created on: 18 Nov 2020
 *  Author: James Hoskin
 */
#include "car_settings.h"

extern Driver_Profile current_driver;
extern Event_Profile current_event;

Driver_Profile drivers[4] = {
    { "Default", 45.8, 3, 100, 76 },
    { "James", 45.8, 3, 100, 76 },
    { "Elon Musk", 20, 10, 10, 10 },
    { "Issac Baldry", 14, 2, 55, 100 },
};

Event_Profile events[4] = {
    { "Endurance", 45.8, 3, 17, 76 },
    { "Acceleration", 20, 10, 12, 10 },
    { "Skidpad", 11, 3, 55, 50 },
    { "Autocross", 45, 1, 57, 100 }
};

void changeDriver(Driver_Profile driver_profile){
	current_driver = driver_profile;
}

void changeEvent(Event_Profile event_profile){
	current_event = event_profile;
}
