#ifndef M_RTC_HANDLER_H
#define M_RTC_HANDLER_H
#include "H_DS1307_rtc_driver.h"

#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"

#define RTC_TIME_LEN 30

G_RESP_CODE m_u8RTC_get_str(uint8_t *str);
G_RESP_CODE m_u8RTC_strTime_set(uint8_t *current_time);
//G_RESP_CODE m_u8RTC_test(G_CTL_CODES ctl);
//G_RESP_CODE m_u8RTC_time_get(RTC_DATE_TIME *rtc_cur_dat);
//void m_u8RTC_ref_set(uint8_t *buff);
//void m_u8RTC_ref_get(RTC_DATE_TIME *rtc_cur_dat);
#endif
