#include "A_main_app_handler.h"
#include "M_mode_key_handler.h"
#include "M_zone_handler.h"
#include "M_led_handler.h"
#include "M_eeprom_handler.h"
#include "M_led_handler.h"

/******************************************DEFENITIONS_START***************************************/

//debounce time
#define MAX_KEY_DB_TIME 2
#define NO_UPDATE 0
#define UPDATE 1

/******************************************VARIABLES_START***************************************/
 
uint8_t u8_G_pres_state;
uint8_t u8_G_prev_state=0;
uint8_t u8_G_state_change = 0;
uint8_t u8_dbcnt = 0;

MODE_KEY_TYPE u8_G_key_type = EXT_KEY;
SYS_MODE_E u8_G_sys_mode = DEFAULT_MODE;
SYS_MODE_E u8_G_prev_mode = DEFAULT_MODE;

uint8_t m_vMKEY_monitor(void)
{
	G_RESP_CODE ret = DEFAULT_MODE;
	
	if(u8_G_key_type == EXT_KEY)
	{
		u8_G_pres_state=key1_pin;
		u8_G_pres_state=(u8_G_pres_state<<1)|key2_pin;
		if(u8_G_prev_state!=u8_G_pres_state)
		{
			u8_G_prev_state=u8_G_pres_state;
			u8_dbcnt=0;
			u8_G_state_change=1;			
		}
		if(u8_G_state_change == 1)
		{
			u8_dbcnt++;
			if(u8_dbcnt > MAX_KEY_DB_TIME)
			{
				u8_G_sys_mode=u8_G_pres_state-1;
				u8_G_state_change = 0;
				u8_dbcnt = 0;				 			
			}
		}
	}
	if(u8_G_prev_mode - u8_G_sys_mode)
	{	
		u8_G_prev_mode = u8_G_sys_mode;	
		ret = u8_G_sys_mode;
		
		///ch-7-10
		
	}
	return ret;
}

SYS_MODE_E m_u8MKEY_sts_get(void)
{ 
	return u8_G_sys_mode;
}
MODE_KEY_TYPE m_u8MKEY_type_get(void)
{ 
	return u8_G_key_type;
}
