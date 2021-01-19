#include "M_rtc_handler.h"  
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "M_eeprom_handler.h" 
#include "M_GeneralLib.h" 

/*
#define NO_SIM 0
#define DEVIATION_THRESH 1

#define STR 0
#define DEC 1


typedef enum
{
	DAT_A_GREAT  =0,
	DAT_B_GREAT = 1,
	DATE_SAME = 2,
	DATE_INVALID = 3,
	DATE_A_INVALID = 4,
	DATE_B_INVALID = 5
}DATE_RESP_E;


 
uint8_t u8PrevHour = 25; 
RTC_DATE_TIME st_dt_ref_time;

G_RESP_CODE m_u8RTC_gsmTime_get(RTC_DATE_TIME *rtc_cur_dat);
DATE_RESP_E m_u8RTC_compare_time(RTC_DATE_TIME *dat_a,RTC_DATE_TIME *dat_b);
static uint8_t u8MonEndFinder(uint8_t month,uint8_t year);
DATE_RESP_E u8Diff(uint8_t val_a,uint8_t val_b,uint8_t *diff_val);
uint16_t u8Date_diff_cal(RTC_DATE_TIME *dat_a,RTC_DATE_TIME *dat_b);
DATE_RESP_E u8Date_diff(RTC_DATE_TIME *dat_a,RTC_DATE_TIME *dat_b);
 */
 
G_RESP_CODE m_u8RTC_get_str(uint8_t *str)
{
	G_RESP_CODE ret =SUCCESS;
	RTC_DATE_TIME rtc_cur_dat;
	uint8_t num_arr[5] = {0};
	uint8_t k = 0;
	ret = h_u8RTC_time_get(&rtc_cur_dat);
	if(ret == SUCCESS)
	{
	//date_time_arr - 18/05/31,18:48:57 SIZE 18 BYTES
	//sprintf(str,"%d/%d/%d,%d:%d:%d",rtc_cur_dat.u8Year,rtc_cur_dat.u8Month,rtc_cur_dat.u8Day,
	//rtc_cur_dat.u8Hour,rtc_cur_dat.u8Minute,rtc_cur_dat.u8Second);
		if((rtc_cur_dat.u8Year >= 18)&&(rtc_cur_dat.u8Month<=12)&&(rtc_cur_dat.u8Day <=31)&&
			(rtc_cur_dat.u8Hour<=24)&&(rtc_cur_dat.u8Minute<=60)&&(rtc_cur_dat.u8Month!=0)&&(rtc_cur_dat.u8Day!=0))				
		{
			
			u8GL_num2str(rtc_cur_dat.u8Year,(char*)num_arr);
			k =  u8GL_ncopy(str,num_arr,2);
			str = str + k;
		       *str = '/';
		        str++;
			u8GL_num2str(rtc_cur_dat.u8Month,(char*)num_arr); 
			k =  u8GL_ncopy(str,num_arr,2);	
			str = str + k;
		       *str = '/';
		        str++;
			u8GL_num2str(rtc_cur_dat.u8Day,(char*)num_arr); 
			k =  u8GL_ncopy(str,num_arr,2);	
			str = str + k;
		       *str = ',';
		        str++;
			u8GL_num2str(rtc_cur_dat.u8Hour,(char*)num_arr); 
			k =  u8GL_ncopy(str,num_arr,2);	
			str = str + k;
		       *str = ':';
		        str++;
			u8GL_num2str(rtc_cur_dat.u8Minute,(char*)num_arr);
			k =  u8GL_ncopy(str,num_arr,2);
			str = str + k;
		       *str = ':';
		        str++;
			u8GL_num2str(rtc_cur_dat.u8Second,(char*)num_arr);
			k =  u8GL_ncopy(str,num_arr,2);
			str = str + k;      
		       *str = '\0';
		       ret = SUCCESS;	
		}
		else
		{
			ret = FAILURE;
		}
	}
       else
       {
	      ret = FAILURE; 
       }
       return ret;
}

G_RESP_CODE m_u8RTC_strTime_set(uint8_t *current_time)
{	//date_time_arr - 18/5/31,18:48:57 SIZE 18 BYTES
	G_RESP_CODE ret =SUCCESS;
	RTC_DATE_TIME rtc_cur_dat;	 
	char time_data[6][4]={0};
	uint8_t i = 0,n = 0;
	uint8_t head = 0;
	//GSM TIME FORMAT - 18/05/31,18:48:57 SIZE 18 BYTES
        //h_u8GSM_time_get(current_time);
	//m_u8GSM_time_get(current_time);
	for(i=0;i<RTC_TIME_LEN;i++)
	{
		if(current_time[i]==47)
		{
			strncpy(time_data[n],(char *)current_time+head,i-(head));
			if(strlen(time_data[n])==0)
			{
				ret =FAILURE;
				break;	
			}
			n++;
			head = i+1;
			if(n==6)
			{
				ret =SUCCESS;
				break;
			}
			
		}
		else if((current_time[i] < 58)&&(current_time[i] > 47))
		{
			
		}
		else
		{
		    	ret = FAILURE;
			break;
		}
	}
	if(ret == SUCCESS)
	{
		rtc_cur_dat.u8Second = (uint8_t)atoi(time_data[5]);
	  	rtc_cur_dat.u8Minute = (uint8_t)atoi(time_data[4]);
	  	rtc_cur_dat.u8Hour = (uint8_t)atoi(time_data[3]);
	  	rtc_cur_dat.u8Day = (uint8_t)atoi(time_data[2]);
	  	rtc_cur_dat.u8DayOfWeek =0;
	  	rtc_cur_dat.u8Month = (uint8_t)atoi(time_data[1]);
	  	rtc_cur_dat.u8Year= (uint8_t)atoi(time_data[0]);
		if((rtc_cur_dat.u8Year >= 18)&&(rtc_cur_dat.u8Month<=12)&&(rtc_cur_dat.u8Day <=31)&&
				(rtc_cur_dat.u8Hour<=24)&&(rtc_cur_dat.u8Minute<=60)&&(rtc_cur_dat.u8Month!=0)&&(rtc_cur_dat.u8Day!=0))				
		{
			h_u8RTC_time_set(rtc_cur_dat);
			ret =SUCCESS;	
		}
		else
		{
			ret = FAILURE;	
		}
	}
	
  	return ret;
}


#if 0
DATE_RESP_E m_u8RTC_compare_time(RTC_DATE_TIME *dat_a,RTC_DATE_TIME *dat_b)
{
	DATE_RESP_E dat_res = DATE_SAME;
	uint16_t temp_val_a =0,temp_val_b =0;
	
	dat_res = u8Date_diff(dat_a,dat_b);	
	if((DATE_A_INVALID != dat_res) && (DATE_B_INVALID != dat_res))
	{
		temp_val_a = (((uint16_t)dat_a->u8Hour) * 60) + ((uint16_t)dat_a->u8Minute);
		temp_val_b = (((uint16_t)dat_b->u8Hour) * 60) + ((uint16_t)dat_b->u8Minute);
		if(DATE_SAME == dat_res)
		{		
			if(temp_val_a > temp_val_b)
			{
				temp_val_a = temp_val_a - temp_val_b;
				dat_res = DAT_A_GREAT;
			}
			else
			{
				temp_val_a = temp_val_b - temp_val_a;
				dat_res = DAT_B_GREAT;
			}				 
		}
		else if(DAT_A_GREAT == dat_res)
		{
			temp_val_b = 1440 - temp_val_b;
			temp_val_a = temp_val_b + temp_val_a;
		}
		else if(DAT_B_GREAT == dat_res)
		{
			temp_val_a = 1440 - temp_val_a;
			temp_val_a = temp_val_a + temp_val_b;
		}
		if(temp_val_a <= DEVIATION_THRESH)
		{
			dat_res = DATE_SAME;
		}
	}	
    return dat_res;
}


G_RESP_CODE m_u8RTC_time_set(RTC_DATE_TIME *rtc_cur_dat)
{
	G_RESP_CODE ret = SUCCESS;
	h_u8RTC_time_set(*rtc_cur_dat);
	return ret;
}
void m_u8RTC_ref_get(RTC_DATE_TIME *rtc_cur_dat)
{
	rtc_cur_dat->u8Second =st_dt_ref_time.u8Second;
	rtc_cur_dat->u8Minute = st_dt_ref_time.u8Minute;
	rtc_cur_dat->u8Hour = st_dt_ref_time.u8Hour;
	rtc_cur_dat->u8Day = st_dt_ref_time.u8Day;
	rtc_cur_dat->u8DayOfWeek =st_dt_ref_time.u8DayOfWeek;
	rtc_cur_dat->u8Month =st_dt_ref_time.u8Month;
	rtc_cur_dat->u8Year= st_dt_ref_time.u8Year;
}
G_RESP_CODE m_u8RTC_time_get(RTC_DATE_TIME *rtc_cur_dat)
{
	G_RESP_CODE ret = SUCCESS;
	//RTC_DATE_TIME curr_time_st;
	//m_u8RTC_test();	
	h_u8RTC_time_get(rtc_cur_dat);
	return ret;
} 

DATE_RESP_E u8Date_diff(RTC_DATE_TIME *dat_a,RTC_DATE_TIME *dat_b)
{
	DATE_RESP_E ret = DATE_INVALID;
	uint8_t diff_val = 0;	 
	if((dat_a->u8Year < 100) && (dat_a->u8Month <= 12) && (dat_a->u8Day <= 31) && (dat_a->u8Hour < 60) && (dat_a->u8Minute < 60))
	{
		if((dat_b->u8Year < 100) && (dat_b->u8Month <= 12) && (dat_b->u8Day <= 31) && (dat_b->u8Hour < 60) && (dat_b->u8Minute < 60))
		{
			ret = DATE_SAME;
		}
		else
		{
			ret = DATE_B_INVALID;
		}
	}
	else
	{
		ret = DATE_A_INVALID;
	}
	if(DATE_SAME == ret)
	{
		ret = u8Diff(dat_a->u8Year,dat_b->u8Year,&diff_val);
		if(DATE_SAME == ret)
		{
			ret = u8Diff(dat_a->u8Month,dat_b->u8Month,&diff_val);
			if(DATE_SAME == ret)
			{
				ret = u8Diff(dat_a->u8Day,dat_b->u8Day,&diff_val);
			}
			else if(DAT_A_GREAT == ret)
			{
				//u8Date_diff_cal(dat_a,dat_b);
			}
			else
			{
				//u8Date_diff_cal(dat_b,dat_a);
			}
		}
	}
	return ret;
}
uint16_t u8Date_diff_cal(RTC_DATE_TIME *dat_a,RTC_DATE_TIME *dat_b)
{
	uint16_t sum  =0;
	uint8_t month_end = 0,i = 0,j = 0;	 
	month_end = u8MonEndFinder(dat_b->u8Month,dat_b->u8Year);
	sum = month_end - dat_b->u8Day;
	i = dat_a->u8Month - dat_b->u8Month - 1;
	j =  dat_b->u8Month + 1;
	for(;i>0;i--)
	{
		month_end = u8MonEndFinder(j,dat_b->u8Year);
		sum = sum + month_end;
		j++;
	}
	sum = sum + dat_a->u8Day;
	return sum;
}
DATE_RESP_E u8Diff(uint8_t val_a,uint8_t val_b,uint8_t *diff_val)
{
	DATE_RESP_E ret = DATE_SAME;
	*diff_val = val_a - val_b;
	if(val_a > val_b)
	{
		*diff_val = val_a - val_b;
		ret = DAT_A_GREAT;	 
	}
	else if(val_b > val_a)
	{
		*diff_val = val_b - val_a;
		ret = DAT_B_GREAT;
	}
	return ret;
}
static uint8_t u8MonEndFinder(uint8_t month,uint8_t year)
{
	uint8_t ret = 31;
	if(month % 2 == 0)
	{
		ret = 30;
		if(8 == month)
		{
			ret = 31;
		}
		else if(2 == month)
		{
			if(year % 4 == 0)
			{
				ret = 29;
			}
			else
			{
				ret = 28;
			}
		}		 
	}
	return ret;
}
#endif
