/*
 * KL25Z_SHIELD_INTERFACE.h
 *
 *  Created on: Aug 14, 2014
 *      Author: Ulises
 */


#ifndef KL25Z_SHIELD_INTERFACE_H_
#define KL25Z_SHIELD_INTERFACE_H_

/********************************************************************************
						libraries used.
********************************************************************************/ 
#include "..\mcl\ADC.h"
#include "derivative.h"

#define LED_SENS0_HIGH	  GPIOB_PSOR |= (1<<8)
#define LED_SENS0_LOW	  GPIOB_PCOR |= (1<<8)
#define LED_SENS1_HIGH	  GPIOB_PSOR |= (1<<9)
#define LED_SENS1_LOW	  GPIOB_PCOR |= (1<<9)
#define LEDS_VEL_STATE_0  GPIOB_PCOR |= (0xC00)
#define LEDS_VEL_STATE_1  GPIOB_PDOR |= (0x400)
#define LEDS_VEL_STATE_2  GPIOB_PDOR |= (0x800)
#define LEDS_VEL_STATE_3  GPIOB_PDOR |= (0xC00)
/********************************************************************************
						Functions.
********************************************************************************/ 
void INIT_SHIELD_DRIVERS(void);
unsigned int POT1_ADC_VALUE(void);
unsigned int POT2_ADC_VALUE(void);
unsigned int DIP_SW_VAL(void);
unsigned int PUSH_BUTTON_1_STATE(void);
unsigned int PUSH_BUTTON_2_STATE(void);

#endif /* KL25Z_SHIELD_INTERFACE_H_ */
