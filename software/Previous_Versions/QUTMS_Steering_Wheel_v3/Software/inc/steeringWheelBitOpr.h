/*****************************************************************************
* @file    Software/inc/steeringWheelBitOpr.h 
* @author  Zoe Goodward
* @version V1.0.0
* @date    28-April-2018
* @brief   Macros for bitwise operations
*****************************************************************************/

#ifndef STEERINGWHEELBITOPR_H_
#define STEERINGWHEELBITOPR_H_

/* Bitwise operations */
#define SET_BIT(reg, pin)			(reg) |= (1 << (pin))
#define CLEAR_BIT(reg, pin)			(reg) &= ~(1 << (pin))
#define FLIP_BIT(reg, pin)          (reg) ^= (1 << (pin))
#define WRITE_BIT(reg, pin, value)	(reg) = (((reg) & ~(1 << (pin))) | ((value) << (pin)))
#define CHECK_BIT(reg, pin)         (reg) & (1 << (pin))
#define BIT_VALUE(reg, pin)			(((reg) >> (pin)) & 1)
#define BIT_IS_SET(reg, pin)		(BIT_VALUE(reg,pin)==1)
#define OVERRIDE_BITS(reg, pin)		(reg) = (1 << (pin))

#define SHIFT_BITS(pin, x)          ((x) << (pin))

/* Bit masking operations */
#define SET_BIT_MASK(x, y) 			(x |= (y))
#define CLEAR_BIT_MASK(x, y)		(x &= (~y))
#define FLIP_BIT_MASK(x, y)			(x ^= (y))
#define CHECK_BIT_MASK(x, y)		(x & (y))

#endif /* STEERINGWHEELBITOPR_H_ */