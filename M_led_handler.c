#include "M_led_handler.h"
#include "M_zone_handler.h"
#include "M_mode_key_handler.h"
//#include "M_power_handler.h"
#include "A_main_app_handler.h"
#include "r_cg_serial.h"
#include "M_rtc_handler.h"
#include "H_led_driver.h"

//ch-7-10 -- ff changed to 00 
uint8_t u8_G_led_arr[NO_OF_ROWS]={0x00,0x00,0x00,0x00,0x00,0x00};

#if 0
uint8_t m_u8LED_arr_get(uint8_t index)
{
	uint8_t ret;
	ret = u8_G_led_arr[index]; 
	return ret;
}
#endif

void m_u8LED_ctrl_led(LED_NAME choice,G_CTL_CODES cmd)
{
	uint8_t bit_index = choice%10;
	uint8_t ic_index = (choice-bit_index)/10;
	ic_index = ic_index-1;
	if((ic_index < 3)&&(bit_index<8))
	{
		if(cmd == ON)
		{
		u8_G_led_arr[ic_index] = u8_G_led_arr[ic_index] | ((uint8_t)1 << bit_index);
		}
		else if(cmd == OFF)
		{
		u8_G_led_arr[ic_index] = u8_G_led_arr[ic_index] & (~((uint8_t)1 << bit_index));	
		}
		else
		{
			
		}
	 	m_u8LED_response_hdlr(u8_G_led_arr); 
	}
	
	
}

void m_u8LED_change(LED_GROUP choice,uint8_t cmd)
{
	uint8_t i = 0;
	switch(choice)
	{
		case MODE:
		{
			switch(cmd)
			{
				case NIGHT_MODE:
				{
					m_u8LED_ctrl_led(LED_DAY,OFF);
					m_u8LED_ctrl_led(LED_NIGHT,ON);
					break;
				}
				case DAY_MODE:
				{
					m_u8LED_ctrl_led(LED_NIGHT,OFF);
					m_u8LED_ctrl_led(LED_DAY,ON);
					break;	
				}
				case DISABLE_MODE:
				{
					m_u8LED_ctrl_led(LED_NIGHT,OFF);
					m_u8LED_ctrl_led(LED_DAY,OFF);
					for(i=0;i<6;i++)
					{
						m_u8LED_zone(i,DISABLE);
					}
					break;	
				}
				default:
				break;
			}
			break;
		}
		case SOURCE:
		{
			switch(cmd)
			{  
				case MAIN:
				{
					m_u8LED_ctrl_led(LED_BAT_R,OFF);
					m_u8LED_ctrl_led(LED_BAT_G,ON);
					m_u8LED_ctrl_led(LED_AC,ON);
					break;
				}
				case BAT_LOW:
				{
					
					m_u8LED_ctrl_led(LED_AC,OFF);
					m_u8LED_ctrl_led(LED_BAT_G,OFF);
					m_u8LED_ctrl_led(LED_BAT_R,ON);
					break;	
				}
				case BAT_NORMAL:
				{
					m_u8LED_ctrl_led(LED_BAT_R,OFF);
					m_u8LED_ctrl_led(LED_AC,OFF);
					m_u8LED_ctrl_led(LED_BAT_G,ON);
					break;	
				}
				case BAT_DISCONN:
				{
					m_u8LED_ctrl_led(LED_BAT_R,OFF);
					m_u8LED_ctrl_led(LED_BAT_G,OFF);
					break;	
				}
				default:
				break;
			}
			break;
		}
		case SYS_STAT:
		{
			switch(cmd)
			{
				case ALT:
				{
					m_u8LED_ctrl_led(LED_ALERT,ON);
					break;
				}
				case SAFE:
				{
					m_u8LED_ctrl_led(LED_ALERT,OFF);
					break;
				}
				case MUTE:
				{
					m_u8LED_ctrl_led(LED_MUTE,ON);
					break;	
				}
				case UNMUTE:
				{
					m_u8LED_ctrl_led(LED_MUTE,OFF);
					break;	
				}
				default:
				break;
			}
			break;
		}
		default:
		break;	
	}
	 m_u8LED_response_hdlr(u8_G_led_arr);
	
}

void m_u8LED_zone(uint8_t zone,uint8_t cmd)
{
	uint8_t index = 0;
	if(zone < 6)
	{
		if(zone % 2 == 0)
		{
			index = 11+10*(zone/2);
			//index = 10+10*(zone/2);
		}
		else
		{
			index = 15+10*((zone-1)/2);
			//index = 13+10*((zone-1)/2);
		}
	}
	
	if((index==25)||(index==35))
	{
	index = index -1;	
	}
	
	if(cmd == SAFE)
	{
		m_u8LED_ctrl_led(index,ON);
		m_u8LED_ctrl_led(index+1,OFF);
		
	}
	else if(cmd == ALT)
	{
		m_u8LED_ctrl_led(index,OFF);
		m_u8LED_ctrl_led(index+1,ON);
	}
	else if(cmd == DISABLE)
	{
		m_u8LED_ctrl_led(index,OFF);
		m_u8LED_ctrl_led(index+1,OFF);
	}
	else
	{
		
	}
	 m_u8LED_response_hdlr(u8_G_led_arr);
}

void refresh_led(void)
{
	m_u8LED_response_hdlr(u8_G_led_arr);	
}