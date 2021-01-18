#include "A_main_app_handler.h"
#include "M_rtc_handler.h"
#include "M_led_handler.h"
#include "M_LOG_hdlr.h"
#include "stdlib.h"
#include "string.h"
#include "M_eeprom_handler.h"
#include "M_zone_handler.h"
#include "M_mode_key_handler.h"
#include "M_power_handler.h"
#include "r_cg_serial.h"
#include "H_led_driver.h"

/******************************************DEFNITIONS_START***************************************/
 
#ifdef DEBUG_ENABLE
extern uint8_t u8_log_uart_tx_flag;
char debug_arr[DEBUG_LEN]={0};
#endif

#define STARTUP_WAIT_TIME 10

/******************************************ENUM_START***************************************/

typedef enum
{	
	STARTUP=0,
	MONITOR_STATE,
	DISABLE_STATE,
	LOG_STATE,
}E_SYS_MAIN_STATE;

/******************************************VARIABLES_START***************************************/

E_SYS_MAIN_STATE e_sys_main_state = STARTUP;
E_SYS_MAIN_STATE e_sys_prev_state = MONITOR_STATE;
 
uint8_t init_wait_cnt=STARTUP_WAIT_TIME;


void a_u8APP_handler(void)
{
	G_RESP_CODE ret = 0;
	m_vZONE_get_dipdelay();
	switch(e_sys_main_state)
	{
	case STARTUP:
	{
		m_vled_pattern();
		init_wait_cnt--;
		if(init_wait_cnt == 0)
		{
			e_sys_main_state = MONITOR_STATE;
		}
		break;
	}
	case MONITOR_STATE:
	{
		m_POW_monitor();
		m_vZONE_monitor();
		update_mute_sts();
		ret = m_vMKEY_monitor();
		if(ret != DEFAULT_MODE)
		{
			
			alt_stop();
			m_vLOG_logdata();
			//m_u8ZONE_alt_reset();
			m_u8LED_change(MODE,ret);
			if(ret == DISABLE_MODE)
			{
				e_sys_main_state = DISABLE_STATE;
			}
		}
		if(m_u8get_alt_sts() == SYS_SAFE)
		{
			ret = admin();
			if(ret == SUCCESS)
			{
				m_vLOG_storelog_stop();
				e_sys_prev_state = e_sys_main_state;
				e_sys_main_state = LOG_STATE;
			}
			
		}
		break;	
	}
	case DISABLE_STATE:
	{
		m_POW_monitor();
		ret = m_vMKEY_monitor();
		if(ret != DEFAULT_MODE)
		{
			//alt_stop();
			m_u8ZONE_alt_reset();
			m_u8LED_change(MODE,ret);
			m_vLOG_logdata();
			if(ret != DISABLE_MODE)
			{
				e_sys_main_state = MONITOR_STATE;
			}
		}
		ret = admin();
		if(ret == SUCCESS)
		{
			m_vLOG_storelog_stop();
			e_sys_prev_state = e_sys_main_state;
			e_sys_main_state = LOG_STATE;
		}
		break;
	}
	case LOG_STATE:
	{
		m_vled_pattern();
		ret = user();
		if(ret == SUCCESS)
		{
			e_sys_main_state = e_sys_prev_state;
			refresh_led();
		}
		break;
	}
	default:
	break;
	}
}
