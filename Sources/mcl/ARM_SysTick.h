#ifndef ARM_SYSTICK_H_
#define ARM_SYSTICK_H_

void InitSysTick();

#define SYSTICK_FREQUENCY 49920    //  SYSTICK_FREQUENCY  = (CORE_CLOCK * 20.8nS) /PERIOD_TICK_WANTED 
								   //  PERIOD_TICK_ACTUAL =  20 uS // 100 us
								
#endif /* SYSTICK_H_ */

