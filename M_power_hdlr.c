#include "M_led_handler.h"
#include "M_power_hdlr.h"
#include "r_cg_adc.h"
#include "stdlib.h"
#include "r_cg_serial.h"


/*
	bat 12v --> 440 to 470
	bat 10v --> 360 to 385
	bat 9v --> 325 to 340
	
*/


#define POW_FLUC_TH 60

 

#define POWER_DEBOUNCE_TIME 5

#define LOW_POWER_MODE_THRESHOLD 310//385
#define NORMAL_POWER_MODE_THRESHOLD 385//480
#define SHUTDOWN_MODE_THRESHOLD 300//325//test_th = 200

uint8_t u8_G_power_mode = WAITING; //DONT CHANGE THIS ;REFER POWER TEST CALLEE
uint8_t u8_G_dbcount=0;
uint8_t _u8POW_hdlr_ctl = OFF;
uint16_t ref_adc = 0;
static uint16_t u16Adc_Buffer=0;
int16_t m_u16Diff_cal(uint16_t val1,uint16_t val2);
void m_vPOWER_monitor(void)
{
	uint8_t curr_mode = 0;
	G_RESP_CODE response; 
	(void)R_ADC_Get_Result(&u16Adc_Buffer);
	
	if(m_u16Diff_cal(u16Adc_Buffer,ref_adc) < POW_FLUC_TH)
	{
		u8_G_dbcount++;
		if(u8_G_dbcount > POWER_DEBOUNCE_TIME)
		{
			u8_G_dbcount = 0;
			if(u16Adc_Buffer < LOW_POWER_MODE_THRESHOLD)
			{
				 response = POW_LOW;
				 m_u8LED_change(SOURCE,BAT_LOW);
			}
			else if(u16Adc_Buffer < NORMAL_POWER_MODE_THRESHOLD)
			{
				 response = POW_NORMAL;
				 m_u8LED_change(SOURCE,BAT_NORMAL);
			}
			else
			{
				 response = POW_AC;
				 m_u8LED_change(SOURCE,MAIN);
			}
			if(u8_G_power_mode !=  response)
			{
				u8_G_power_mode =  response;		 
			}
			
		}
	}
	else
	{
		ref_adc = u16Adc_Buffer;
		u8_G_dbcount = 0;
	} 	
}
int16_t m_u16Diff_cal(uint16_t val1,uint16_t val2)
{
	int16_t result = 0;
	result = abs(val1 - val2);
	return result;
}
uint8_t m_u8POWER_sts_get(void)
{
	uint8_t ret;
	ret = u8_G_power_mode;
	return ret;
}
uint8_t m_u8POWER_test(void)
{
	uint8_t ret;
	ret = m_u8POWER_sts_get();
	return ret;
}
G_RESP_CODE m_u8POW_hdlr_ctl(G_CTL_CODES ctl)
{
	G_RESP_CODE ret = SUCCESS;
	switch(ctl)
	{
		case T_START:
		{
			_u8POW_hdlr_ctl = ON;
			u8_G_dbcount = POWER_DEBOUNCE_TIME;
			break;
		}
		case T_STOP:
		{
			_u8POW_hdlr_ctl = OFF;
			break;
		}
		default:
		break;
	}
	return ret;
}
