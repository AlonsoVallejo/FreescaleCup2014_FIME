/*
 * DC_Motors_Control.c
 *
 *  Created on: Apr 15, 2015
 *      Author: Vallejo
 */

#include "DC_Motors_Control.h"

/********************************************************************************
						Variables uses to the dc motors control
********************************************************************************/ 
float  DC_error,
	   speedup,
	   speeddown,
	   sp_pwm_l,
	   sp_pwm_r,
	   PWM_MAX_MOTORS_H; 

 float DC_PWM_R_1, //Values for the right and left dc motors.
       DC_PWM_L_1,
       DC_PWM_R_2, 
       DC_PWM_L_2,
       DC_PWM_R,
       DC_PWM_L,
       A,B,C; 

unsigned int brake_delay = 0;
             
unsigned int wait_next_sample,
			 max,
			 L_ppp,
			 R_ppp;
			 
float diff_pwm_r,
	  diff_pwm_l,
	  pwm_r_accel,
	  pwm_l_accel,
	  pwm_r_brake,
	  pwm_l_brake;             
	
#define ERROR_MAX			1.0
#define ERROR_MIN			-1.0
#define MOTORS1_MIN_SPEED	0
#define MOTORS2_MIN_SPEED	255
#define MOTORS1_MAX_SPEED	255
#define MOTORS2_MAX_SPEED	0

#define min_zeros_track		35
#define max_zeros_track		50

/********************************************************************************
						This function has the algorithm control to
						control the dc motors.
********************************************************************************/ 
void DC_MOTORS_CONTROL(void)
{
	DC_DP_parameters();
	//DC_DP_parameters_test();
	PUSH_BUTTON_TASK();
	if(error < -0.25) error = ERROR_MIN;
	else if(error > 0.25) error = ERROR_MAX;
	
	DC_error=error*3.333;
	
	if(DC_error > ERROR_MAX)      DC_error = ERROR_MAX;
	else if(DC_error < ERROR_MIN) DC_error = ERROR_MIN;
	
	if(DC_error <= 0)   //spin to right
	{
		sp_pwm_r = PWM_MAX_MOTORS_H + (PWM_MAX_MOTORS_H*DC_error*speeddown);    //--
		sp_pwm_l = PWM_MAX_MOTORS_H + (PWM_MAX_MOTORS_H*DC_error*speedup*(-1));      //++
	}
	else                           //spin to left
	{
		sp_pwm_r = PWM_MAX_MOTORS_H + (PWM_MAX_MOTORS_H*DC_error*speedup);      //++
		sp_pwm_l = PWM_MAX_MOTORS_H + ((-1)*PWM_MAX_MOTORS_H*DC_error*speeddown);    //--
	}
	
	
		
	DC_MOTORS();
	//PTA13_TEST_LOW;
}

/********************************************************************************
						This function contains the different behavior
						of the car.
********************************************************************************/ 

void DC_MOTORS(void)
{
	if((DC_error >= -0.1)&&(DC_error <= 0.1))
	{
		sp_pwm_r = max;
		sp_pwm_l = max;     
	}
	
	diff_pwm_r = sp_pwm_r - right_tire_value;
	diff_pwm_l = sp_pwm_l - left_tire_value;
	
	if(finish_flag== 1) // stop the car with the camera
	{
		LEDS_CAMERA_VALUE(3000);
		if(brake_delay > 40) stop_car();
		brake_delay = brake_delay + 1;
	}
	else if (count_buttonA == 0) stop_car();
	
	else if(count_buttonA == 1)
	{
		if(sampling_speed >= samples_number_ss)
		{
			if(diff_pwm_r >= 0.0 ) // right tire needs accel
			{
				pwm_r_accel = sp_pwm_r + diff_pwm_r;
				DC_PWM_R = pwm_r_accel;
			}
			else if(diff_pwm_r < 0.0) // right tire needs brake
			{
				pwm_r_brake = 255 - (-1*diff_pwm_r);
				DC_PWM_R = sp_pwm_r + diff_pwm_r;
			}
	
			if(diff_pwm_l >= 0.0 ) // left tire needs accel
			{
				pwm_l_accel = sp_pwm_l + diff_pwm_l;
				DC_PWM_L = pwm_l_accel;		
			}
			else if(diff_pwm_l < 0.0) // left tire needs brake
			{
				pwm_l_brake = 255 - (-1*diff_pwm_l);
				DC_PWM_L = sp_pwm_l + diff_pwm_l;
			}
		}
	}
	DC_MOTORS_SPEED();
	reset_values();
}



/********************************************************************************
						This function receives the final velocity 
						parameters to control the tires of the car.
********************************************************************************/ 

void DC_MOTORS_SPEED(void)
{
	DC_PWM_L_1 = (unsigned int)DC_PWM_L;
	DC_PWM_R_1 = (unsigned int)DC_PWM_R;
	DC_PWM_R_2 = (unsigned int)pwm_r_brake;
	DC_PWM_L_2 = (unsigned int)pwm_l_brake;
	
	
	if (DC_PWM_R_1>255) DC_PWM_R_1 =255;
	if (DC_PWM_L_1>255) DC_PWM_L_1 =255;
	
	//non-short circuit
	if(DC_PWM_R_2 <= DC_PWM_R_1) DC_PWM_R_2=DC_PWM_R_1 + 2;
	if(DC_PWM_R_2 > 255) DC_PWM_R_2=255;
	if(DC_PWM_L_2 <= DC_PWM_L_1) DC_PWM_L_2=DC_PWM_L_1 + 2;
	if(DC_PWM_L_2 > 255) DC_PWM_L_2=255;
	
	//DC_DP_parameters_test();
	
	MOTOR1_R_SPEED(DC_PWM_R_1);
	MOTOR1_L_SPEED(DC_PWM_L_1);

	MOTOR2_R_SPEED(DC_PWM_R_2);
	MOTOR2_L_SPEED(DC_PWM_L_2);
	
	
	pwm_r_brake = 255;
	pwm_l_brake = 255;
}

/********************************************************************************
						This function contains the different velocities
						of the car.
********************************************************************************/ 

void DC_DP_parameters(void)
{
        switch ((DIP_SW_VAL() & 0xC) >> 2) 
        {
               //////////////////////////////////////
        		case 0: // funcional
                    LEDS_VEL_STATE_0;
                    PWM_MAX_MOTORS_H =(100); //0.2
                    speeddown=0;
                    speedup = 0;
                    max=110;
                    break;                                  
                 ///////////////////////////////////// 
                case 1:
                	LEDS_VEL_STATE_1;
                	PWM_MAX_MOTORS_H =(110);
                	speeddown = 0.2;
                	speedup = 0.2;
                	max=120;
                	break;                         
                case 2:
                	LEDS_VEL_STATE_2;
                	PWM_MAX_MOTORS_H =(120);
                	speeddown=0.4;
                	speedup= 0.4;
                	max=130;
                	break;                        
                case 3:
                	LEDS_VEL_STATE_3;
                	PWM_MAX_MOTORS_H =(130);
                	speeddown = 0.6;
                	speedup = 0.6;
                	max=140;
                	break;                      
                default:
                	break;
        }
 }

/********************************************************************************
						stop dc motors
********************************************************************************/ 
void stop_car(void)
{
	MOTOR1_R_SPEED(MOTORS1_MIN_SPEED);
	MOTOR1_L_SPEED(MOTORS1_MIN_SPEED);

	MOTOR2_R_SPEED(MOTORS2_MIN_SPEED);
	MOTOR2_L_SPEED(MOTORS2_MIN_SPEED);
	DC_PWM_L=0;
	DC_PWM_R=0;
}

/********************************************************************************
						brake dc motors
********************************************************************************/ 
void stop_car_brake(void)
{
	MOTOR1_R_SPEED(MOTORS1_MAX_SPEED);
	MOTOR1_L_SPEED(MOTORS1_MAX_SPEED);

	MOTOR2_R_SPEED(MOTORS2_MAX_SPEED);
	MOTOR2_L_SPEED(MOTORS2_MAX_SPEED);
	DC_PWM_L=0;
	DC_PWM_R=0;
}
/********************************************************************************
						Reset values of the speed sensors
********************************************************************************/ 

void reset_values(void)
{
	if(sampling_speed >=samples_number_ss)
	{
		R_ppp=right_tire_pulses;
		right_tire_pulses = 0;
		L_ppp=left_tire_pulses;
		left_tire_pulses = 0;
		sampling_speed = 0;
	}
}

/********************************************************************************
					testing velocities to read the speed sensors
********************************************************************************/ 

void DC_DP_parameters_test(void)
{
        switch ((DIP_SW_VAL() & 0xC) >> 2) 
        {
                case 0: // funcional
                    LEDS_VEL_STATE_0;
                    MOTOR1_R_SPEED(90);
                    MOTOR1_L_SPEED(90);

                    MOTOR2_R_SPEED(255);
                   	MOTOR2_L_SPEED(255);
                    break;                                  
                       
                case 1:
                	LEDS_VEL_STATE_1;
                	MOTOR1_R_SPEED(120);
                	MOTOR1_L_SPEED(120);
                	
                	MOTOR2_R_SPEED(255);
                	MOTOR2_L_SPEED(255);
                	break;                         
                case 2:
                	LEDS_VEL_STATE_2;
                	MOTOR1_R_SPEED(130);
                	MOTOR1_L_SPEED(130);
                	
                	MOTOR2_R_SPEED(255);
                	MOTOR2_L_SPEED(255);
                	break;                        
                case 3:
                	LEDS_VEL_STATE_3;
                	MOTOR1_R_SPEED(150);
                	MOTOR1_L_SPEED(150);
                	
                	MOTOR2_R_SPEED(255);
                	MOTOR2_L_SPEED(255);
                	break;                      
                default:
                	break;
        }
 }



