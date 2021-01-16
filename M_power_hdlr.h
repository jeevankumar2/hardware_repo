#ifndef M_POWER_HDLR_H
#define M_POWER_HDLR_H

#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"

void m_vPOWER_monitor(void);
uint8_t m_u8POWER_sts_get(void);
uint8_t m_u8POWER_src_get(void);
uint8_t m_u8POWER_test(void);
G_RESP_CODE m_u8POW_hdlr_ctl(G_CTL_CODES ctl);

#endif
