#include "tpm.h"
#include "ADC.h"
#include "..\sdl\KL25Z_SHIELD_INTERFACE.h"


//**************************************************
//				DEFINE USES FOR THE TPM MODULES
//**************************************************

//source clock select.
#define TPM_PLLFLL 		1
#define TPM_OSCERCLK 	2
#define TPM_MCGIRCLK	3

#define	TPM_CNT_DIS	0
#define	TPM_CLK		1
#define	TPM_EXT_CLK	2

//preescaler definitions.
#define PS_1	0
#define PS_2	1
#define PS_4	2
#define PS_8	3
#define PS_16	4
#define PS_32	5
#define PS_64	6
#define PS_128	7

//confuralbles bits for the TPM modules.
#define TPM_OC_TOGGLE		TPM_CnSC_MSA_MASK|TPM_CnSC_ELSA_MASK
#define TPM_OC_CLR			TPM_CnSC_MSA_MASK|TPM_CnSC_ELSB_MASK
#define TPM_OC_SET			TPM_CnSC_MSA_MASK|TPM_CnSC_ELSA_MASK|TPM_CnSC_ELSB_MASK
#define TPM_OC_OUTL			TPM_CnSC_MSB_MASK|TPM_CnSC_MSA_MASK|TPM_CnSC_ELSB_MASK
#define TPM_OC_OUTH			TPM_CnSC_MSB_MASK|TPM_CnSC_MSA_MASK|TPM_CnSC_ELSA_MASK

#define TPM_PWM_H 			TPM_CnSC_MSB_MASK|TPM_CnSC_ELSB_MASK // PWM  with HIGH TRUE pulse.
#define TPM_PWM_L			TPM_CnSC_MSB_MASK|TPM_CnSC_ELSA_MASK // PWM  with LOW  TRUE pulse.

#define EDGE_PWM			0 
#define CENTER_PWM			1

//Servo and dc motors to PWM control definitions
//Based in the FRDM-TFC SHIELD 2014 schematics

#define SERVO_MOTOR_VALUE 	TPM1_C0V // PTB0
#define LEDS_CAMERA_PWM		TPM1_C1V // PTB1
#define MOTOR1_LEFT_VALUE	TPM0_C0V // PTC1 B1
#define MOTOR2_LEFT_VALUE	TPM0_C1V // PTC2 B2
#define MOTOR1_RIGHT_VALUE	TPM0_C2V // PTC3 A1
#define MOTOR2_RIGHT_VALUE	TPM0_C3V // PTC4 A2

#define STOP_MOTOR_H		0
#define STOP_MOTOR_L		255

#define TPM0_PWM_MOD		255						   // TPM_MOD for the DC motors
#define TPM1_PWM_MOD	    48000000/(1<<(PS_64+1))/50 // TPM_MOD for the servo

#define RISING_CAPTURE		(TPM_CnSC_ELSA_MASK)		// TPM2_INPUT_CAPTURE_MODE
#define FALLING_CAPTURE		(TPM_CnSC_ELSB_MASK)	    // TPM2_INPUT_CAPTURE_MODE

/*********************************************************************
* 				INIT_SERVO()
*				Function for init the servo motor to use with the
*				FRDM and the TFC shield.
*					Parameters:
*					--
*					
*					return:
*					--
*********************************************************************/

void INIT_SERVO(void)
{
		SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK; // enable port B.
		PORTB_PCR0 = PORT_PCR_MUX(3); // configure the PTB0 port as TPM.
		
		SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK; 
		SIM_SOPT2 &= ~(SIM_SOPT2_TPMSRC_MASK);
		SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);
		SIM_SCGC6 |= SIM_SCGC6_TPM1_MASK; // enable TPM1 module.
		
		TPM1_MOD = TPM1_PWM_MOD; //PERIOD FOR THE TPM1 7500.
		TPM1_SC	|= TPM_SC_CMOD(TPM_CLK) | TPM_SC_PS(PS_64); //PRESCALER SETTING
		TPM1_SC |= TPM_SC_CPWMS_MASK; // select the CPWMS mode.
		TPM1_C0SC = TPM_PWM_H ; // PWM stars with HIGH pulse.
}

/*********************************************************************
* 				INIT_PWM_LEDS_CAMERA()
*				Function for init PWM to control the leds light at the
*				camera
*					Parameters:
*					--
*					
*					return:
*					--
*********************************************************************/
void INIT_PWM_LEDS_CAMERA(void)
{
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK; // enable port B.
	PORTB_PCR1 = PORT_PCR_MUX(3); // configure the PTB1 port as TPM.
			
	SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK; 
	SIM_SOPT2 &= ~(SIM_SOPT2_TPMSRC_MASK);
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);
	SIM_SCGC6 |= SIM_SCGC6_TPM1_MASK; // enable TPM1 module.
	
	TPM1_MOD = TPM1_PWM_MOD; //PERIOD FOR THE TPM1 (7500).
	TPM1_SC	|= TPM_SC_CMOD(TPM_CLK) | TPM_SC_PS(PS_64); //PRESCALER SETTING
	TPM1_SC |= TPM_SC_CPWMS_MASK; // select the CPWMS mode.
	TPM1_C1SC = TPM_PWM_H ; // PWM stars with HIGH pulse.
}
/*********************************************************************
* 			LEDS_CAMERA_VALUE
*			Function to write the TPM counter value that controls
*			the LEDS light.
*			Parameters:
*					TAOS_LEDS_VALUE	value to the PWM to control the LEDS light
*									(0 to 255)
*					
*			return:
*					--
*********************************************************************/

void LEDS_CAMERA_VALUE(unsigned int TAOS_LEDS_VALUE )
{
	LEDS_CAMERA_PWM = TAOS_LEDS_VALUE;
}

/*********************************************************************
 * 			SERVO_POSITION
*			Function to write the TPM counter value that controls
*			the servo position.
*			Parameters:
*					DutyCycle	value to the PWM to control the servo
*								(0 to TPM0_PWM_MOD)
*					
*			return:
*					--
*********************************************************************/

void SERVO_POSITION(float DutyCycle)
{
 // indicate the TPM1 PWM duty cycle
	
	SERVO_MOTOR_VALUE = DutyCycle;

}

/*********************************************************************
* 			INIT_PWMR_MOTOR_1, INIT_PWMR_MOTOR_2
*			The functions bellow are uses to init the PWM of the
*			right dc motors, each terminal has a independent PWM
*			signal.
*			
*			Parameters:
*					--
*								
*			return:
*					--
*********************************************************************/

void INIT_PWMR_MOTOR_1 (void)
{
			SIM_SCGC5  |= SIM_SCGC5_PORTC_MASK; // enable port C.
			PORTC_PCR3 = PORT_PCR_MUX(4); // configure the PTC3 port as TPM, (H-BRIDGE-A-IN1)
			
			SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK; 
			SIM_SOPT2 &= ~(SIM_SOPT2_TPMSRC_MASK);
			SIM_SOPT2	|= SIM_SOPT2_TPMSRC(1);
			SIM_SCGC6	|= SIM_SCGC6_TPM0_MASK; // enable TPM0 module.
			
			TPM0_MOD = TPM0_PWM_MOD; //PERIOD FOR THE TPM0
			TPM0_SC	|= TPM_SC_CMOD(TPM_CLK) | TPM_SC_PS(PS_64); //PRESCALER SETTING
			TPM0_SC |= TPM_SC_CPWMS_MASK; // select the CPWMS mode.
			TPM0_C2SC = TPM_PWM_H ; // PWM with HIGH - TRUE pulses.
}

void INIT_PWMR_MOTOR_2 (void)
{
			SIM_SCGC5  |= SIM_SCGC5_PORTC_MASK; // enable port C.
			PORTC_PCR4 = PORT_PCR_MUX(4); // configure the PTC4 port as TPM, (H-BRIDGE-A-IN2)
			
			SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK; 
			SIM_SOPT2 &= ~(SIM_SOPT2_TPMSRC_MASK);
			SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);
			SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK; // enable TPM0 module.
			
			TPM0_MOD = TPM0_PWM_MOD; //PERIOD FOR THE TPM0
			TPM0_SC	|= TPM_SC_CMOD(TPM_CLK) | TPM_SC_PS(PS_64); //PRESCALER SETTING
			TPM0_SC |= TPM_SC_CPWMS_MASK; // select the CPWMS mode.
			TPM0_C3SC = TPM_PWM_L ; // PWM with LOW - TRUE pulses.
			TPM0_C3V = STOP_MOTOR_L;
}

/*********************************************************************
 * 			INIT_PWML_MOTOR_1, INIT_PWML_MOTOR_2
*			The functions bellow are uses to init the PWM of the
*			left dc motors, each terminal has a independent PWM
*			signal.
*			
*			Parameters:
*					--
*								
*			return:
*					--
*********************************************************************/
void INIT_PWML_MOTOR_1 (void)
{
				SIM_SCGC5  |= SIM_SCGC5_PORTC_MASK; // enable port C.
				PORTC_PCR1 = PORT_PCR_MUX(4); // configure the PTC1 port as TPM. (H-BRIDGE-B-IN1)
				
				SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK; 
				SIM_SOPT2 &= ~(SIM_SOPT2_TPMSRC_MASK);
				SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);
				SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK; // enable TPM0 module.
				
				TPM0_MOD = TPM0_PWM_MOD;; //PERIOD FOR THE TPM0
				TPM0_SC	|= TPM_SC_CMOD(TPM_CLK) | TPM_SC_PS(PS_64); //PRESCALER SETTING
				TPM0_SC |= TPM_SC_CPWMS_MASK; // select the CPWMS mode.
				TPM0_C0SC = TPM_PWM_H ; // PWM stars with HIGH pulse.
}

void INIT_PWML_MOTOR_2 (void)
{
				SIM_SCGC5  |= SIM_SCGC5_PORTC_MASK; // enable port C.
				PORTC_PCR2 = PORT_PCR_MUX(4); // configure the PTC2 port as TPM. (H-BRIDGE-B-IN2)
				
				SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK; 
				SIM_SOPT2 &= ~(SIM_SOPT2_TPMSRC_MASK);
				SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);
				SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK; // enable TPM0 module.
				
				TPM0_MOD = TPM0_PWM_MOD; //PERIOD FOR THE TPM0
				TPM0_SC	|= TPM_SC_CMOD(TPM_CLK) | TPM_SC_PS(PS_64); //PRESCALER SETTING
				TPM0_SC |= TPM_SC_CPWMS_MASK; // select the CPWMS mode.
				TPM0_C1SC = TPM_PWM_L ; // PWM with LOW - TRUE pulses.
				TPM0_C1V = STOP_MOTOR_L;
}

/*********************************************************************
 * 			MOTOR1_L_SPEED, MOTOR2_L_SPEED
*			The functions bellow are uses to control the PWM of the
*			left dc motors.
*			
*			Parameters:
*					PWM_MOTOR 	value of the PWM the control the 
*								tires velocity.
*								
*			return:
*					--
*********************************************************************/

void MOTOR1_L_SPEED(unsigned int PWM_MOTOR)
{
	MOTOR1_LEFT_VALUE = PWM_MOTOR;
}

void MOTOR2_L_SPEED(unsigned int PWM_MOTOR)
{
	MOTOR2_LEFT_VALUE = PWM_MOTOR;
}

/*********************************************************************
 * 			MOTOR1_R_SPEED, MOTOR2_R_SPEED
*			The functions bellow are uses to control the PWM of the
*			right dc motors.
*			
*			Parameters:
*					PWM_MOTOR 	value of the PWM the control the 
*								tires velocity.
*								
*			return:
*					--
*********************************************************************/

void MOTOR1_R_SPEED(unsigned int PWM_MOTOR)
{
	MOTOR1_RIGHT_VALUE = PWM_MOTOR;
}

void MOTOR2_R_SPEED(unsigned int PWM_MOTOR)
{
	MOTOR2_RIGHT_VALUE = PWM_MOTOR;
}

