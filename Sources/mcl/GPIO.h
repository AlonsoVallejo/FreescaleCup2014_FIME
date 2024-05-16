/*
 * GPIO.h
 *
 *  Created on: Aug 15, 2014
 *      Author: Vallejo
 */

#ifndef GPIO_H_
#define GPIO_H_


#include "derivative.h"

//*********************************************************************
//					Line Scan Camera definitions
//*********************************************************************

#define CAMERA_CLK_HIGH  	GPIOE_PSOR |= (1<<1) 	  //pin PTE1 for the HIGH CLK signal to the line scan camera.  
#define CAMERA_CLK_LOW   	GPIOE_PCOR |= (1<<1)	  //pin PTE1 for the LOW CLK signal to the line scan camera.  
#define CAMERA_SI_HIGH  	GPIOD_PSOR |= (1<<7)	  //pin PTD7 for the HIGH SI signal to the line scan camera. 
#define CAMERA_SI_LOW    	GPIOD_PCOR |= (1<<7) 	  //pin PTD7 for the LOW SI signal to the line scan camera. 
#define ENABLE_H_BRIGDE	    GPIOE_PSOR |= (1 << 21)   // enable H - bridge
#define DISABLE_H_BRIGDE	GPIOE_PCOR |= (1 << 21)   // disable H - bridge

#define PTA12_TEST_HIGH		GPIOA_PSOR |= (1<<12)
#define PTA12_TEST_LOW		GPIOA_PCOR |= (1<<12)

#define PTA13_TEST_HIGH		GPIOA_PSOR |= (1<<13)
#define PTA13_TEST_LOW		GPIOA_PCOR |= (1<<13)
#define GPIO_PIN(x)		    (1<<x)	

void Init_Camera (void);
void INIT_GPIO_MOTORS (void);
void INIT_SPEED_SENSORS (void);
unsigned int STATE_SPEEDSENSOR_0(void);
unsigned int STATE_SPEEDSENSOR_1(void) ;

#endif /* GPIO_H_ */
