#ifndef M_MODE_KEY_HANDLER_H
#define M_MODE_KEY_HANDLER_H

#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"

//PIN DEFNITIONS
#define key1_pin P3.0
#define key2_pin P4.2

typedef enum
{
	EXT_KEY = 0,
	UI_KEY = 1
}MODE_KEY_TYPE;
 
//dont change the order-see s8_G_DNB[] in zone handler
typedef enum
{
	NIGHT_MODE = 0,
	DAY_MODE = 1,
	DISABLE_MODE = 2,
	ALL_MODE = 3,	 
	DEFAULT_MODE = 4,
	SYS_MODE_END
}SYS_MODE_E;
 


uint8_t m_vMKEY_monitor(void);
G_RESP_CODE m_u8ModeHdlr_update(const uint8_t *dat);
SYS_MODE_E m_u8MKEY_sts_get(void);
MODE_KEY_TYPE m_u8MKEY_type_get(void);
#endif
