/*****************************************************************************
* @file    Software/inc/steeringWheelSPI.h 
* @author  Zoe Goodward
* @version V1.0.0
* @date    28-April-2018
* @brief   This file declares the serial peripheral interface functions used
*          for communicating with the OLED screen
*****************************************************************************/

#ifndef STEERINGWHEELSPI_H_ 
#define STEERINGWHEELSPI_H_

#include <stdlib.h> 
#include <avr/io.h>

void spi_init();
uint8_t spi_send_byte(uint8_t c);

#endif /* STEERINGWHEELSPI_H_ */