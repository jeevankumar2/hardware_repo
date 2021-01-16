#ifndef M_GENERAL_LIB_H
#define M_GENERAL_LIB_H

#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"

#define MAX_DIGITS 5

uint8_t u8GL_ncopy(uint8_t *str,const uint8_t *dat,uint8_t len);
void u8GL_strclear(uint8_t *buff,uint16_t buff_len);
void u8GL_num2str(uint16_t num,char *strin);
#endif