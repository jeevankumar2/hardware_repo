#ifndef M_TIME_OUT_H
#define M_TIME_OUT_H

#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"

#define DEFAULT_TIMEOUT_VALUE 10

//dont change the values
#define EXPIRED 1
#define NOT_EXPIRED 0


void timeout_start(uint8_t timeout_val);
void timeout_stop(void);
uint8_t is_timeout_expired(void);
uint8_t timeout_chk(uint8_t *flag,uint8_t code,uint8_t timeout);
#endif
