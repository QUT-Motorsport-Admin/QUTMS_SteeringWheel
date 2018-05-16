/*****************************************************************************
* @file    Software/inc/steeringWheelCAN.h 
* @author  Zoe Goodward
* @version V1.0.0
* @date    28-April-2018
* @brief   This file declares the functions to be used for communication with
*          the CAN controller
*****************************************************************************/
 
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#ifndef STEERINGWHEELCAN_H_
#define STEERINGWHEELCAN_H_

#include <util/delay.h>
#include <avr/io.h>

int8_t CAN_sendTest();
uint8_t can_init();
int8_t CAN_findFreeTXMOB();
void can_rx_init(int8_t mob, uint8_t numBytes, uint32_t IDmsk, uint32_t ID);
void can_tx_mob(int8_t mob, uint8_t numBytes, uint8_t * data, uint32_t ID, uint8_t ms_loop_until_TXOK);

#endif /* STEERINGWHEELCAN_H_ */