/*
 * SERVO.c
 *
 *  Created on: Jun 23, 2014
 *       Created by: Alonso Vallejo
 *      			 Ulises Larios
 *      			 Fernando Alvarez
 */


#include "SERVO.h"


/********************************************************************************
						Definitions used to the servo control
********************************************************************************/ 
#define Ts			  	(0.011) // Sampling time
#define error_max		(0.30)
#define error_min		(-0.30)
#define gain_angle_Kp	(1.5)
#define gain_angle_Ki 	(0.00833) 
#define gain_angle_Kd 	(1.2)

/********************************************************************************
						Variables used to the servo control
********************************************************************************/ 
float Position, DutyCycle, car_angle; 
float Ei, Ei_ant, Ed, Ed_ant[4], E_ant, Ud,  Ui, Up;
unsigned char k= 4;
/********************************************************************************
			Principal function that contain the Servo PID control 
********************************************************************************/ 
void SERVO_DIRECTION_CONTROL(void)
{
	//Action of control
	//PTA13_TEST_HIGH;
	if((((error - E_ant) > error_max)||((error - E_ant) < error_min))) // error takes values between (-0.3,0.3)
	{
		error = E_ant;
	}
	
	if(error < -0.25) error = error_min;
	else if(error > 0.25) error = error_max;
	
	if((DIP_SW_VAL() & 0x2) != 2 ) error=0.0;
	// Proportional action
	Up = gain_angle_Kp * error;
	
	
	//Integral action
	Ei = (Ts * (error + Ei_ant)) / 2;  //trapezoidal integration
	Ei_ant = Ei;
	Ui = gain_angle_Ki * Ei;
	
	//Derivative action
	//Taylor expansions series derivation. 
	Ed_ant[4]=error;
	Ed = Ts * ((8 * Ed_ant[k] - Ed_ant[k-1] - 8 * Ed_ant[k-2] + Ed_ant[k-3]) / 12 * Ts);
	Ed_ant[1]=Ed_ant[2];
	Ed_ant[2]=Ed_ant[3];
	Ed_ant[3]=Ed_ant[4];
	Ud = gain_angle_Kd * Ed;
	
	// simple derivation
	/*Ed=(error - Ed_ant) / Ts;
	Ed_ant = error;
	Ud = gain_angle_Kd * Ed;*/

	//Control Action
	car_angle = Up + Ui + Ud;
	
	E_ant = error; 
	STEERING_SERVO(car_angle);  
}

/********************************************************************************
						Function to control the steering servo
********************************************************************************/ 

void STEERING_SERVO(float Radians) //radians takes values between (-0.541 , 0.541)
{
	
	if(Radians > Angle_Max)
	{
		Radians = Angle_Max;
	}
	else if(Radians < ((-1) * Angle_Max))
	{
		Radians = ((-1) * Angle_Max);
	}
	Position = Radians / Angle_Max;
	DutyCycle=((((Position + 1.0)/2)*(SERVO_MAX_DUTY_CYCLE - SERVO_MIN_DUTY_CYCLE))+SERVO_MIN_DUTY_CYCLE); // dutycycle takes values between (-1,1)
	SERVO_POSITION(DutyCycle);
}

