#ifndef M_EEPROM_HANDLER_H_
#define M_EEPROM_HANDLER_H_

//#include "M_message_queue_handler.h"
#include "r_cg_userdefine.h"
#include "M_sys_config.h"
 


G_RESP_CODE m_vE2P_log_reset(void);
void m_u8E2p_BP_DataStore(void);
//G_RESP_CODE m_u8E2P_response_hdlr(MSGQ_TYPE *msgq);
//G_RESP_CODE m_u8E2P_Req_resp_hdlr(void);
G_RESP_CODE m_u8EEPROM_criticalDataRetrieve(void);
G_RESP_CODE m_u8EEPROM_test(void);
G_RESP_CODE m_u8EEPROM_log_store(void);
G_RESP_CODE m_u8EEPROM_log_retrieve(uint8_t log_cmd);
void m_u8E2p_EXT_KEY_set(void);
#ifdef STARTUP_E2P_CLEAR
void m_vE2P_clear(void);
#endif


#endif