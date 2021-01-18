#pragma mul

#include "M_LOG_hdlr.h"
#include "M_zone_handler.h"
#include "M_mode_key_handler.h"
#include "r_cg_serial.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "M_led_handler.h"
#include "A_main_app_handler.h"
#include "M_eeprom_handler.h"
#include "M_GeneralLib.h"
#include "M_led_handler.h"

/******************************************DEFNITIONS_START***************************************/

#define NO_LED_BIT 0 //DONT CHANGE THIS VALUE
#define ISOLATE_LED_BIT 2 //10 
#define ALERT_LED_BIT 1 //01 
#define ACTIVE_LED_BIT 4 //100 
#define MUTE_DEFAULT 4
#define ZONE_DELIMITER '\r'
#define STS_DELAY_DELIMITER ' '
#define ZONE_COUNT 6
#define ZO_ZC_MAX_INDEX 4
#define ZONE_SELECT_STR_LEN 4
#define ZO_ZC_STR_LEN 5
#define DNB_STR_LEN 4
#define MAX_DELAY_TIME 630
#define SW1_WEIGHTAGE 45
#define SEC1_CNT_500MS 2
#define DIP_1SW_DELAY (SW1_WEIGHTAGE*SEC1_CNT_500MS)

/******************************************ENUM_START***************************************/
 
typedef enum
{
	NO = 0,//dont change
	NC = 1//these values
}ZONE_TRIG;


typedef enum
{
	ISOLATE = 0,
	ACTIVATE = 1
}ACTIVE_STS;

typedef enum
{
	NO_ALT = 0,//dont change 
	ALT_SENSED = 1,//the 
	ALT_TRIGGERED = 2,//order
	NIGHT_WAIT = 3
}ZONE_STS;

typedef struct  
{
	ACTIVE_STS active_sts;
	ZONE_TRIG trig_setting;
	uint16_t delay_timing;
	SYS_MODE_E mode_setting;
	ZONE_TRIG zone_input;
	uint16_t zone_dly_cnt;
	ZONE_STS zone_sts;	
}ZONE_SETTINGS;

/******************************************VARIABLES_START***************************************/

uint8_t mute_db_cnt = 0;
uint8_t mute_sts = MUTE_DEFAULT;//this value must not be 0 or 1
uint8_t mute_sts_change = 0;
uint8_t pres_sts = SYS_SAFE;

uint8_t tamper_sts = NO_ALT;

uint8_t sw_db_cnt = 0;
uint8_t G_mult_fact = 0;
uint8_t sw_sts_change = 0;

ZONE_SETTINGS sensor_index[ZONE_COUNT] = {
					  {ACTIVATE,NO,2,DAY_MODE,NC,0,NO_ALT},{ACTIVATE,NO,2,DAY_MODE,NC,0,NO_ALT},
					  {ACTIVATE,NO,2,DAY_MODE,NC,0,NO_ALT},{ACTIVATE,NC,2,NIGHT_MODE,NO,0,NO_ALT},
					  {ACTIVATE,NC,2,NIGHT_MODE,NO,0,NO_ALT},{ACTIVATE,NC,2,NIGHT_MODE,NO,0,NO_ALT}
					 };			
const uint8_t s8_G_zone_select[ZONE_COUNT][ZONE_SELECT_STR_LEN] = {"Z1","Z2","Z3","Z4","Z5","Z6"/*,"Z7 ","Z8 ","Z9 ","Z10","Z11","Z12"*/}; 
const uint8_t s8_G_active_isolate[2][2]={"I","A"};
const uint8_t s8_G_NO_NC[2][3]={"NO","NC"};
const uint8_t s8_G_DNB[5][DNB_STR_LEN]={"N","D","DIS","B","NW"};
const uint8_t s8_G_ZO_ZC[ZO_ZC_MAX_INDEX][ZO_ZC_STR_LEN]={"NORM","ALT ","ALT ","NW  "};
 
SYS_MODE_E u8ZONE_sys_mode = DEFAULT_MODE;

/******************************************FUNCTIONS_START***************************************/

void Relay_ON(void);
void Relay_OFF(void);
void sounder_ctrl(G_CTL_CODES cmd);

void m_vZONE_monitor(void)
{	 
	uint8_t zone=0;
	uint8_t tamper = 0;
	tamper = TAMPER;
	sensor_index[0].zone_input=zone1;
	sensor_index[1].zone_input=zone2;
	sensor_index[2].zone_input=zone3;
	sensor_index[3].zone_input=zone4;
	sensor_index[4].zone_input=zone5;
	sensor_index[5].zone_input=zone6;
	for(zone = 0; zone < ZONE_COUNT;zone++)
	{
		switch(sensor_index[zone].zone_sts)
		{
			case NO_ALT:
			{
				if(sensor_index[zone].active_sts==ACTIVATE)
				{
					//m_u8LED_zone(zone,SAFE);
					
					if((( m_u8MKEY_sts_get() == sensor_index[zone].mode_setting) || (sensor_index[zone].mode_setting == DAY_MODE))&&(sensor_index[zone].trig_setting==sensor_index[zone].zone_input))
					{
						sensor_index[zone].zone_sts=ALT_SENSED;
						m_u8LED_zone(zone,ALT);
						m_vLOG_logdata();
						log_disable();
									
					}
				}
				break;
			}
			case ALT_SENSED:
			{
				sensor_index[zone].zone_dly_cnt++;				
				if(sensor_index[zone].zone_dly_cnt > sensor_index[zone].delay_timing)
				{					
					sensor_index[zone].zone_dly_cnt = 0;				 
					sensor_index[zone].zone_sts=ALT_TRIGGERED;
					alt_trig();					 
				}
				break;
			}
			case NIGHT_WAIT:
			{
				sensor_index[zone].zone_dly_cnt++;		
				if(sensor_index[zone].zone_dly_cnt > sensor_index[zone].delay_timing)
				{
					sensor_index[zone].zone_sts = NO_ALT;
					sensor_index[zone].zone_dly_cnt=0;
				}
				break;
			}
			default:
			break;
		}
	}
	if(tamper == NC)
	{
		if(tamper_sts == NO_ALT)
		{
			alt_trig();
			tamper_sts = ALT_SENSED;
			m_vLOG_logdata();
		}
	}
}

uint8_t m_u8ZONE_sts_get(uint8_t zone)
{
	uint8_t ret = NO_LED_BIT;	 
	if(zone < ZONE_COUNT)
	{
		if((ALT_SENSED == sensor_index[zone].zone_sts) || (ALT_TRIGGERED == sensor_index[zone].zone_sts))
		{
			ret = ALERT_LED_BIT; 
		}
		else
		{
			if(sensor_index[zone].active_sts == ACTIVATE)
			{
				ret = ACTIVE_LED_BIT;	 
			}
			else
			{
				ret = ISOLATE_LED_BIT;  
			}		
		}		
	}	 
	return ret;
}
void m_u8ZONE_alt_reset(void)
{	
	uint8_t zone=0;
	SYS_MODE_E cmd;
	cmd = m_u8MKEY_sts_get();
	for(zone = 0; zone < ZONE_COUNT;zone++)
	{
		m_u8LED_zone(zone,SAFE);
		sensor_index[zone].zone_dly_cnt = 0;		
		if(NIGHT_MODE == cmd)  
		{
			//ch-27-10
			if((ACTIVATE == sensor_index[zone].active_sts)&&(NIGHT_MODE == sensor_index[zone].mode_setting))
			{
				sensor_index[zone].zone_sts = NIGHT_WAIT;
			}	
			else
			{
				sensor_index[zone].zone_sts = NO_ALT;
			}			
		}
		else
		{
			sensor_index[zone].zone_sts = NO_ALT;
		}		 
	}
	//ch-27-10
	u8ZONE_sys_mode = cmd;
	tamper_sts = NO_ALT;
}

G_RESP_CODE m_u8log_zone_data(uint8_t *ptr)
{
    uint8_t zone=0;
    char stri[MAX_DIGITS+1]={0};	 
    uint8_t k =0; 
    k = m_u8MKEY_sts_get();
    if(k >= SYS_MODE_END)
    {
	k = DEFAULT_MODE;
    }
    k =  u8GL_ncopy(ptr,s8_G_DNB[k],DNB_STR_LEN-1);
    ptr = ptr + k;
   *ptr = ZONE_DELIMITER;
    ptr++;
    for(zone = 0;zone<ZONE_COUNT;zone++)
    {
       k =  u8GL_ncopy(ptr,s8_G_zone_select[zone],ZONE_SELECT_STR_LEN-1);
       ptr = ptr + k;
       *ptr = '=';
       ptr++;
       if(sensor_index[zone].zone_sts < ZO_ZC_MAX_INDEX)
       {
       	       k =  u8GL_ncopy(ptr,s8_G_ZO_ZC[sensor_index[zone].zone_sts],ZO_ZC_STR_LEN-1);
       }
       else
       {
	       k =  u8GL_ncopy(ptr,(uint8_t *)"ERR",4); 
       }
       ptr = ptr + k;
       if(zone == (ZONE_COUNT-1))
       {
       *ptr = STS_DELAY_DELIMITER;
       ptr++;       
       if(sensor_index[zone].delay_timing <= MAX_DELAY_TIME)
       {
       	     u8GL_num2str(sensor_index[zone].delay_timing, stri);
	     k = u8GL_ncopy(ptr,(uint8_t*)stri,MAX_DIGITS);
       }
       else
       {
	     u8GL_ncopy((uint8_t *)stri,(uint8_t *)"ERR",4);
	     k = u8GL_ncopy(ptr,(uint8_t*)stri,4);
       }
       
       ptr = ptr + k;
       *ptr = ZONE_DELIMITER;
       ptr++;
       }
       else
       {
       *ptr = ZONE_DELIMITER;
       ptr++;       
       }
    }
    k =  u8GL_ncopy(ptr,(uint8_t *)"T ",2);
    ptr = ptr + k;
    if(tamper_sts < (ZO_ZC_MAX_INDEX-2))
    {
    	    k = u8GL_ncopy(ptr,s8_G_ZO_ZC[tamper_sts],ZO_ZC_STR_LEN-1);
    }
    else
    {
	    k =  u8GL_ncopy(ptr,(uint8_t *)"ERR",4); 
    }
    ptr = ptr + k;
   //*ptr = ZONE_DELIMITER;
    //ptr++;
    *ptr = ZONE_DELIMITER;
    ptr++;
    *ptr = '\0';	
    return 0;	
	
}

void m_vZONE_get_dipdelay(void)
{
	uint8_t mult_fact=0;
	mult_fact=DELAY_SW_1;
	mult_fact=(mult_fact<<1)|DELAY_SW_2;
	mult_fact=(mult_fact<<1)|DELAY_SW_3;
	if(mult_fact != G_mult_fact)
	{
		G_mult_fact = mult_fact;
		sw_db_cnt = 0;
		sw_sts_change = 1;
		
	}
	if(sw_sts_change == 1)
	{	
		sw_db_cnt++;	
		if(sw_db_cnt > MAX_SWITCH_DB_TIME)
		{
			sw_db_cnt = 0;
			sw_sts_change = 0;
			if(mult_fact == 0)
			{
				sensor_index[DELAY_ZONE].delay_timing = 2;
			}
			else
			{
				sensor_index[DELAY_ZONE].delay_timing = mulu(mult_fact,DIP_1SW_DELAY);
			}
			
		}
		
	}
}

void alt_trig(void)
{
	Relay_ON();	
	log_disable();
	pres_sts = SYS_IN_ALT;
	m_u8LED_change(SYS_STAT,UNMUTE);
	m_u8LED_change(SYS_STAT,ALT);
}

void alt_stop(void)
{
	Relay_OFF();
	m_u8ZONE_alt_reset();
	if(m_u8get_alt_sts() == SYS_IN_ALT)
	{
		log_enable();		
	}
	m_u8LED_change(SYS_STAT,UNMUTE);
	pres_sts = SYS_SAFE;
	m_u8LED_change(SYS_STAT,SAFE);
}

void Relay_ON(void)
{
	sounder_ctrl(ON);
	REL2 = ON;
	REL3 = ON;
	
}

void Relay_OFF(void)
{
	sounder_ctrl(OFF);
	REL2 = OFF;
	REL3 = OFF;
	
}

void sounder_ctrl(G_CTL_CODES cmd)
{
	REL1 = cmd;	
}

void update_mute_sts(void)
{
	uint8_t mute_inp = 0;
	mute_inp = MUTE_BTN;
	if(mute_sts != mute_inp)
	{
		mute_sts = mute_inp;	
		mute_db_cnt = 0;
		mute_sts_change = 1;
	}
	
	if(mute_sts_change == 1)
	{
		mute_db_cnt++;	
		if(mute_db_cnt > MAX_MUTE_DB_TIME)
		{
			if(pres_sts == SYS_IN_ALT)
			{
				if(mute_inp==OFF)
				{
					m_u8LED_change(SYS_STAT,UNMUTE);
					sounder_ctrl(ON);
				}
				else
				{
					m_u8LED_change(SYS_STAT,MUTE);
					sounder_ctrl(OFF);
				}
			}
			else
			{
				
			}
			mute_db_cnt = 0;
			mute_sts_change = 0;
		}
		
	}
		
}
G_RESP_CODE m_u8get_alt_sts(void)
{
	G_RESP_CODE ret = WAITING;
	ret = pres_sts;
	return ret;
}