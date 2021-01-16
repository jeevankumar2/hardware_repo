#ifndef M_EVENT_HANDLER_H_
#define M_EVENT_HANDLER_H_

#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"

#define FLAG_SET 1 //Dont change these values
#define FLAG_CLR 0
#define FLAG_CHK 2


#define SETB(A,B) (A = (1<<B)|A)
#define CLRB(A,B) (A = ~(1<<B)&A)
#define CHKB(A,B) ((1<<B)&A)


/***************EVENT FLAG DEFINITIONS*********************/ 
//#define SYS_IN_ALT (0x00U) //0 - no alert, 1 - alert
//#define SYS_IN_MUTE_FLAG (0x01U) //0-normal; 1-mute 
#define ADC_CRI_FLAG (0x02U)//1-Response pending 0 - Response received
//#define ZONE_ALT_FLAG (0x03)//0- DONT TAKE LOG;1 - TAKE LOG 
//#define ZONE_LED_FLAG (0x04)//0- DONT UPdate led ;1 - update led 
//#define INT_BUZZ_TOGG_FLAG (0x05)//
//#define INT_BP_LED1_TOGG_FLAG (0x06)//
//#define INT_BP_LED2_TOGG_FLAG (0x07)//
/***************EVENT FLAG DEFINITIONS*********************/


G_CTL_CODES GSM_event_flag_manager(uint16_t flag_pos,uint8_t operation);
G_CTL_CODES EVE_manipulate(uint16_t flag_pos,uint8_t operation,uint16_t *Event_group);
#endif