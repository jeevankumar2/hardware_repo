#ifndef H_DS1307_RTC_DRIVER_H
#define H_DS1307_RTC_DRIVER_H

#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"

typedef struct
{
	uint8_t u8Year;
	uint8_t u8Month;
	uint8_t u8Day;
	uint8_t u8DayOfWeek;
	uint8_t u8Hour;
	uint8_t u8Minute;
	uint8_t u8Second;
}RTC_DATE_TIME;


void h_u8RTC_time_set(RTC_DATE_TIME rtc_cur_dat);
uint8_t h_u8RTC_time_get(RTC_DATE_TIME *rtc_cur_dat);

#endif