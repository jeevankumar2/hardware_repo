#include "M_ADC_hdlr.h" 
#include "M_power_handler.h"
#include "M_led_handler.h"
//#include "M_SYS_NOTI_hdlr.h"
 

/*
	bat 12v --> 440 to 470
	bat 10v --> 360 to 385
	bat 9v --> 325 to 340
	
*/

#define AC_ON_TH 450
//#define BAT_ON_TH 210//435
#define BAT_LOW_TO_BAT_NORM_TH 230
#define BAT_ON_HIGH_TH 420//435
#define BAT_ON_LOW_TH 345//435
#define BAT_LOW_TH 318//500
#define BAT_SH_TH 500
#define BAT_FAIL_TH 500

#define BAT_CONN_TH 785
#define BAT_DISCONN_TH 780


 #define POW_TH 20
 
 #define BAT_LOW_DEV 30

LED_GROUP POW_sts = SOURCE;
LED_GROUP BAT_sts = SOURCE;
 
void m_POW_monitor(void)
{	
	uint16_t buffer = 0;
	//SYS_SUB_EVENTS_T sts = POW_sts;
	LED_GROUP sts = POW_sts;
	if(ADC_STABLE == M_ADC_CHsts_get(ADC_AC,&buffer))
	{
		if(AC_ON_TH <= buffer)
		{
			  sts = MAIN;
		}
		else if((BAT_ON_LOW_TH <= buffer) && ((BAT_ON_HIGH_TH) > buffer))
		{
			if(POW_sts == BAT_LOW)
			{
				if(BAT_LOW_TO_BAT_NORM_TH < buffer)
				{
					sts = BAT_NORMAL;
				}
			}
			else
			{
				sts = BAT_NORMAL;
			}
			
			
		}
		else if(BAT_LOW_TH > buffer)
		{
			 sts = BAT_LOW;
			 
		}	
		if(POW_sts - sts)
		{
			POW_sts = sts;
			m_u8LED_change(SOURCE,POW_sts);
		}
	}	
	sts = BAT_sts;
	if(ADC_STABLE == M_ADC_CHsts_get(ADC_BAT,&buffer))
	{
		/******************logic must be verified with different battery voltages*********************/
		if(BAT_CONN_TH < buffer)
		{
			sts = BAT_DISCONN;	
		
		}
		else if(BAT_DISCONN_TH >= buffer)
		{
			sts = BAT_CONN;
		}
	}	
	if(BAT_sts - sts)
	{
		BAT_sts = sts;
		m_u8LED_change(SOURCE,BAT_sts);
		if(BAT_sts == BAT_CONN)
		{
			m_u8LED_change(SOURCE,POW_sts);
		}
		//NOTI_hdlr(E_POWER,POW_sts);
	}
}
void m_POW_reset(void)
{
	//POW_sts = ES_DEFAULT;
}
  