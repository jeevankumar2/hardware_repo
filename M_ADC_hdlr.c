#pragma div


#include "M_ADC_hdlr.h"
//#include "H_lcd_driver.h"
#include "M_event_handler.h"
#include "stdlib.h"
#include "r_cg_adc.h" 

#define ADC_FLUC_TH 10

#define ADC_DB_MAX 2

/********used in M_ADC_init()**/
#define ADC_INITIAL_VAL 500
//uint16_t ADC_buffer[NO_OF_ADC_IN] = {0};

uint16_t test_adc_array[NO_OF_ADC_IN] = {0};

ADC_CH_ST ADC_ch_st[NO_OF_ADC_IN] = {0};
uint8_t ADC_CH_cnt = 0;

int16_t m_u16Diff_cal(uint16_t val1,uint16_t val2);


void M_ADC_init(void)
{
	uint8_t i = 0,j = 0;
	for(i = 0;i<NO_OF_ADC_IN;i++)
	{
		for(j = 0;j<MAF_INDEX_MAX;j++)
		{
			ADC_ch_st[i].ADC_buffer[j] = ADC_INITIAL_VAL;
		}
		 	 
	        ADC_ch_st[i].REF_ADC_buffer = ADC_INITIAL_VAL;
		ADC_ch_st[i].ACC_ADC_result = ADC_INITIAL_VAL;
		ADC_ch_st[i].RAW_ADC_buffer = ADC_INITIAL_VAL;
		ADC_ch_st[i].ADC_db_cnt = 0;
		ADC_ch_st[i].ADC_STS = ADC_STABLE;
		ADC_ch_st[i].MAF_index = 0;	 	
	}
}


void M_ADCMonitor(void)
{
	uint8_t i = 0;
	uint8_t test = 0;
	char lcd_dbg[10] = {0};
	
	for(i = 0;i<NO_OF_ADC_IN;i++)
	{
		if(m_u16Diff_cal(ADC_ch_st[i].ACC_ADC_result,ADC_ch_st[i].REF_ADC_buffer) < ADC_FLUC_TH)
		{
			ADC_ch_st[i].ADC_db_cnt++;
			if(ADC_ch_st[i].ADC_db_cnt > ADC_DB_MAX)
			{
				ADC_ch_st[i].ADC_db_cnt = 0;
				ADC_ch_st[i].ADC_STS = ADC_STABLE;
				test_adc_array[i] = ADC_ch_st[i].ACC_ADC_result;
			}
		}
		else
		{			 
			ADC_ch_st[i].ADC_db_cnt = 0;
			ADC_ch_st[i].REF_ADC_buffer = ADC_ch_st[i].ACC_ADC_result;
			ADC_ch_st[i].ADC_STS = ADC_UNSTABLE;
		}
	}
	 
	#if 0
	sprintf((char*)debug_array,"%d %d %d %d",test_adc_array[0],test_adc_array[1],test_adc_array[2],test_adc_array[3]);
	LCD_update_hdlr(0,0,(char*)debug_array,16);
	sprintf((char*)debug_array,"%d %d %d %d",ADC_ch_st[0].RAW_ADC_buffer,ADC_ch_st[1].RAW_ADC_buffer,ADC_ch_st[2].RAW_ADC_buffer,ADC_ch_st[3].RAW_ADC_buffer);
	LCD_update_hdlr(0,1,(char*)debug_array,16);
	#else
	//sprintf( lcd_dbg,"%d",test_adc_array[0]);
	//lcd_update_hdlr(12,0, lcd_dbg,4);
	//sprintf( lcd_dbg,"%d",test_adc_array[1]);
	//lcd_update_hdlr(12,1, lcd_dbg,4);
	#endif
	 	
	  
}

void M_ADC_Conditioner(void)
{
	uint8_t j = 0,i = 0;
	/******critical section start***********/
	//R_ADC_INT_disable();
	 //R_ADC_Stop();
	// DI();
	M_ADC_EnterCritical(ADC_CRI_ENTER);
	for(i = 0;i<NO_OF_ADC_IN;i++)
	{
		ADC_ch_st[i].ADC_buffer[ADC_ch_st[i].MAF_index] = ADC_ch_st[i].RAW_ADC_buffer;
		ADC_ch_st[i].MAF_index++;
		if(ADC_ch_st[i].MAF_index >= MAF_INDEX_MAX)
		{
		    ADC_ch_st[i].MAF_index = 0;
		}
	}
	M_ADC_EnterCritical(ADC_CRI_EXIT);
	//EI();
	//R_ADC_INT_enable();
	// R_ADC_Start();
	/******critical section stop***********/
	
	for(i = 0;i<NO_OF_ADC_IN;i++)
	{
		ADC_ch_st[i].ACC_ADC_result = 0;
	    	for(j = 0;j<MAF_INDEX_MAX;j++)
	    	{
			ADC_ch_st[i].ACC_ADC_result = ADC_ch_st[i].ACC_ADC_result + ADC_ch_st[i].ADC_buffer[j];
	    	}
	    	ADC_ch_st[i].ACC_ADC_result = divuw(ADC_ch_st[i].ACC_ADC_result,MAF_INDEX_MAX);
	}
	R_ADC_Start();
    	
}

void M_ADC_EnterCritical(uint8_t enter_exit)
{
	if(ADC_CRI_ENTER == enter_exit)
	{
		GSM_event_flag_manager(ADC_CRI_FLAG,FLAG_SET);
	}
	else if(ADC_CRI_EXIT == enter_exit)
	{
		GSM_event_flag_manager(ADC_CRI_FLAG,FLAG_CLR);
	}
}
uint8_t M_ADC_CHsts_get(uint8_t ch_no,uint16_t *buffer)
{
	if(ch_no < NO_OF_ADC_IN)
	{		 
		*buffer = ADC_ch_st[ch_no].ACC_ADC_result;
	         return ADC_ch_st[ch_no].ADC_STS;
	}
	return ADC_INVALID_PARAM;	
}
int16_t m_u16Diff_cal(uint16_t val1,uint16_t val2)
{
	int16_t result = 0;
	result = abs(val1 - val2);
	return result;
}
