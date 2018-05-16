/*****************************************************************************
* @file    Software/inc/steeringWheelADC.h 
* @author  Zoe Goodward
* @version V1.0.0
* @date    28-April-2018
* @brief   This file declares the functions to be used for analog to digital 
*          conversion
*****************************************************************************/

#ifndef STEERINGWHEELADC_H_ 
#define STEERINGWHEELADC_H_ 

#include <stdlib.h> 
#include <avr/io.h> 

void adc_init();
uint16_t adc_read(uint8_t channel);

#endif /* STEERINGWHEELADC_H_ */