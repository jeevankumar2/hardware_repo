#ifndef M_LED_HANDLER_H
#define M_LED_HANDLER_H

#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"

typedef enum
{					//do not change the order
	LED_AC=10,	//IC1_8BITS
	LED_Z1_G,//11
	LED_Z1_R,//12
	LED_BAT_G,
	LED_BAT_R, 
	LED_Z2_G,//15
	LED_Z2_R,//16
	LED_UNUSED_1,
	LED_ALERT=20,	//IC2_8BITS
	LED_Z3_G,//21
	LED_Z3_R,//22
	LED_DAY,
	LED_UNUSED_2,
	LED_Z4_G,//25
	LED_Z4_R,//26
	LED_UNUSED_3,
	LED_NIGHT=30,	//IC3_8BITS
	LED_Z5_G,//31
	LED_Z5_R,//32
	LED_MUTE,
	LED_UNUSED_4,
	LED_Z6_G,//35
	LED_Z6_R,//36
	LED_UNUSED_5,
	
}LED_NAME;

typedef enum
{					//do not change the order
	MODE=0,
	SOURCE,
	SYS_STAT,
	//source parameters
	MAIN,
	BAT_LOW,
	BAT_NORMAL,
	//sys_stat_parameters
	ALT,
	SAFE,
	DISABLE,
	MUTE,
	UNMUTE,
	BAT_DISCONN,
	BAT_CONN,
}LED_GROUP;


void m_u8LED_change(LED_GROUP choice,uint8_t cmd);
void m_u8LED_ctrl_led(LED_NAME choice,G_CTL_CODES cmd);
void m_u8LED_zone(uint8_t zone,uint8_t cmd);
void refresh_led(void);

#endif