/*****************************************************************************
* @file    Software/inc/steeringWheelFunctions.h
* @author  Zoe Goodward
* @version V1.0.0
* @date    28-April-2018
* @brief   This file declares the functions and variables that will be used
*          for the general opperation of the steering wheel
*****************************************************************************/

#ifndef STEERINGWHEELFUNCTIONS_H_ 
#define STEERINGWHEELFUNCTIONS_H_ 

// Includes


#define N_PARROT 10 // number of frames for party parrot

// Functions
void steering_wheel_init();
void splash_screen();
void party_mode();
void emergency();
void update_screen();

void endurance();
void acceleration();
void skidpad();
void auto_cross();

void sudo_update_screen();
void main_menu();



#endif /* STEERINGWHEELFUNCTIONS_H_ */