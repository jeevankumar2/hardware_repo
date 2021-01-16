#ifndef M_ADC_HDLR_H
#define M_ADC_HDLR_H
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"

//TOTAL_AD_CH and MAX_ADC_CH_SCAN should not exceed NO_OF_ADC_IN
#define NO_OF_ADC_IN 2
#define MAX_ADC_CH_SCAN 1 
#define TOTAL_AD_CH 2


#define ADC_AC 1
#define ADC_BAT 0

#define ADC_CRI_ENTER 0
#define ADC_CRI_EXIT 1

#define ADC_STABLE 0
#define ADC_UNSTABLE 1
#define ADC_INVALID_PARAM 2

#define MAF_INDEX_MAX 10//Moving average filter index



typedef struct
{
	uint16_t ADC_buffer[MAF_INDEX_MAX];	 
	uint16_t REF_ADC_buffer;
	uint16_t ACC_ADC_result;
	uint16_t RAW_ADC_buffer;
	uint8_t ADC_db_cnt;
	uint8_t ADC_STS;
	uint8_t MAF_index;	 
}ADC_CH_ST;

void M_ADC_init(void);
void M_ADCMonitor(void);
uint8_t M_ADC_CHsts_get(uint8_t ch_no,uint16_t *buffer);
void M_ADC_EnterCritical(uint8_t enter_exit);
void M_ADC_Conditioner(void);
#endif