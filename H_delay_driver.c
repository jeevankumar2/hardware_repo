#include "H_delay_driver.h"
#include "M_time_out.h"
#include "r_cg_timer.h"



extern volatile uint16_t delay_cnt;
extern volatile uint32_t tick_micro;

void h_vDEL_Ms(uint16_t val)
{
	R_TAU0_Channel1_Start();
	while(delay_cnt < val);
	delay_cnt = 0;
	R_TAU0_Channel1_Stop(); 
}


void h_vDEL_100nano(uint32_t val)
{
	 R_TAU0_Channel2_Start();
	 while(tick_micro<val);
	 tick_micro = 0;
	 R_TAU0_Channel2_Stop();
}
