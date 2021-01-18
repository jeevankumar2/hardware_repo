#ifndef M_ZONE_HANDLER_H
#define M_ZONE_HANDLER_H

#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"

#define MAX_MUTE_DB_TIME 4
#define MAX_SWITCH_DB_TIME 4

#define DELAY_ZONE 5

#define D2N 0
#define N2D 1

 /********************PIN DEFINITIONS****************/ 
#define zone1 P5.0 
#define zone2 P5.1
#define zone3 P5.2
#define zone4 P5.3
#define zone5 P5.4
#define zone6 P5.5

#define DELAY_SW_1 P0.6
#define DELAY_SW_2 P0.5
#define DELAY_SW_3 P0.4

#define TAMPER P2.6 //default NO
 /********************PIN DEFINITIONS****************/
void update_mute_sts(void);		
void m_vZONE_monitor(void);
G_RESP_CODE m_u8get_alt_sts(void);
uint8_t m_u8ZONE_sts_get(uint8_t zone); 
void m_u8ZONE_alt_reset(void);
void alt_stop(void);
void alt_trig(void);
void change_zones_mode(uint8_t from_mode,uint8_t to_mode);
G_RESP_CODE m_u8fill_zone_details(uint8_t *arr);
G_RESP_CODE m_u8log_zone_data(uint8_t *ptr); 

void m_vZONE_get_dipdelay(void);
#endif
