/*
 * KL25Z_SHIELD_INTERFACE.c
 *
 *  Created on: Aug 14, 2014
 *      Author: Ulises Larios
 *              Alonso Vallejo
 *              Fernando Alvarez
 */

#include "KL25Z_SHIELD_INTERFACE.h"
#include "derivative.h"

//**************************************************
//					SHIELD INITIALIZATION DRIVERS
//**************************************************
 
void INIT_SHIELD_DRIVERS(void) // init the ADC0
{
	//POT ADC´s Drivers
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;	// enable the ADC0 port.
	ADC0_CFG1 |= ADC_CFG1_MODE( Ten ) | ADC_CFG1_ADIV(0); // ADC set of bits to a certain value and dividing the frequency conversion
	ADC0_SC1A = ADC_SC1_ADCH(31);
	ADC0_CFG2 |= ADC_CFG2_ADHSC_MASK; // High speed conversion.
	ADC0_CFG2 |= ADC_CFG2_ADLSTS_MASK; // 2 extra ASDCK cycles
	
	//Push buttons Drivers
	SIM_SCGC5 |=  SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTE_MASK;
	PORTC_PCR13 = PORT_PCR_MUX(1); 
	PORTC_PCR17 = PORT_PCR_MUX(1); 
	
	//DIP Switch Drivers
	PORTE_PCR2 = PORT_PCR_MUX(1); 
	PORTE_PCR3 = PORT_PCR_MUX(1); 
	PORTE_PCR4 = PORT_PCR_MUX(1); 
	PORTE_PCR5 = PORT_PCR_MUX(1); 
	
	//Ports for LEDs
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB_PCR8 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;   
	PORTB_PCR9 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;   
	PORTB_PCR10 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;   
	PORTB_PCR11 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;  
	GPIOB_PDDR |= (1<<8);
	GPIOB_PDDR |= (1<<9);
	GPIOB_PDDR |= (1<<10);
	GPIOB_PDDR |= (1<<11);
}

//*********************************************************************
//					POT 1 FUNCTION FOR READ THE ADC
//*********************************************************************

unsigned int POT1_ADC_VALUE(void) // function for read the ADC.
{
	ADC0_SC1A = ADC_SC1_ADCH(PTB3); // select the  reading port.
	while(ADC0_SC2 & ADC_SC2_ADACT_MASK);  	// while the conversion is process.	
	while(!(ADC0_SC1A & ADC_SC1_COCO_MASK));// when the conversion ends.		
	return ADC0_RA; // return the conversion value.
}


//*********************************************************************
//					POT 2 FUNCTION FOR READ THE ADC
//*********************************************************************

unsigned int POT2_ADC_VALUE(void) 
{
	
	ADC0_SC1A = ADC_SC1_ADCH(PTB2); // select the  reading port.
	while(ADC0_SC2 & ADC_SC2_ADACT_MASK);  	// while the conversion is process.	
	while(!(ADC0_SC1A & ADC_SC1_COCO_MASK));// when the conversion ends.		
	return ADC0_RA; // return the conversion value.
}


//*********************************************************************
//					 FUNCTION FOR READ DIP SWITCH 
//*********************************************************************

unsigned int DIP_SW_VAL(void)
{
	unsigned short DIP_Val=0;
	DIP_Val = (GPIOE_PDIR>>2) & 0xF;
	return DIP_Val;		
}


//*********************************************************************
//					 FUNCTION FOR READ PUSH BUTTON 1 
//*********************************************************************

unsigned int PUSH_BUTTON_1_STATE(void) 
{
	unsigned short SW1_Val=0;
	SW1_Val = (GPIOC_PDIR>>13) & 0x1;
	return SW1_Val;		
}


//*********************************************************************
//					 FUNCTION FOR READ PUSH BUTTON 2 
//*********************************************************************

unsigned int PUSH_BUTTON_2_STATE(void)
{
	unsigned short SW2_Val=0;
	SW2_Val = (GPIOC_PDIR>>17) & 0x1;
	return SW2_Val;		
}
