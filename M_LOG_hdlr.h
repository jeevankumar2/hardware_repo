#ifndef M_LOG_HDLR_H
#define M_LOG_HDLR_H

#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"

typedef char CIRC_BUFF_TYPE;

#define MAX_CIRC_BUFF_DEPTH 100


typedef enum
{
	LOG_LINE_NO_HEAD = 0,
	LOG_LINE_HEAD_L = 1,
	LOG_LINE_HEAD_H = 2,
	LOG_LINE_TAIL_L = 3,
	LOG_LINE_TAIL_H = 4,
	IDLE_WAIT=5,
}LOG_LINE_TYPE;

typedef struct 
{
    CIRC_BUFF_TYPE *const buffer;
    uint8_t head;
    uint8_t tail;
    const uint8_t maxlen;
}CIRC_BUFF_ST;

typedef enum
{
	LOG_QUEUE=0,
}LOG_Q_NAME;

void LOG_Hdlr(void);
G_RESP_CODE h_u8LOG_Q_send(LOG_Q_NAME qname,CIRC_BUFF_TYPE *val);
G_RESP_CODE h_u8LOG_Q_chk(LOG_Q_NAME qname);
#if 0
G_RESP_CODE cmd_hdlr(void);
#endif
G_RESP_CODE m_u8EEPROM_criticalDataRetrieve(void);
G_RESP_CODE m_u8EEPROM_criticalDataStore(void);
G_RESP_CODE user(void);
G_RESP_CODE admin(void);
void log_disable(void);
void log_enable(void);
void m_vLOG_logdata(void);
void m_vLOG_storelog_stop(void);

#endif