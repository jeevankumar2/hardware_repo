#include "M_eeprom_handler.h"
#include "M_error_handler.h"
#include "M_event_handler.h"
#include "H_M95M01_eeprom_driver.h"
#include "M_rtc_handler.h" 
#include "M_mode_key_handler.h"



#include "string.h"
#include "stdio.h"
#include "r_cg_serial.h"
#include "M_zone_handler.h"
#include "M_GeneralLib.h"
#include "M_time_out.h"


#define GSM_TIME_LEN 30

//#define E2P_OFFSET 4
 #ifdef DEBUG_ENABLE

 char debug_arr[DEBUG_LEN]; 

#endif

#define LOG_INIT 0
#define LOG_TRIG 1

uint8_t eeprom_buff[MAX_EEPROM_BUFF_SIZE] = {0};
uint8_t E2P_log_buff[MAX_EEPROM_BUFF_SIZE] = {0};

const char s8_G_SYS_MODE[6][16]={"N","D","DIS","B","NW","W"};
static uint8_t u8frag_cnt = 0;
static int16_t log_e2p_page = LOG_PAGE_START;
static uint8_t u8E2P_log_start = LOG_INIT;

static m95_ERROR_CODE m_u8EEPROM_criticalDataStore(void);
/*
G_RESP_CODE m_u8E2P_response_hdlr(MSGQ_TYPE *msgq)
{
	G_RESP_CODE ret = EEPROM_IN_PRO;
	m95_ERROR_CODE resp;
	uint8_t temp = 0;
	uint16_t start_index = 0;
	uint8_t *buff_ptr = msgq->data;
	int i = 0;
	 
	if(u8frag_cnt == 0)
	{
		u8frag_cnt = msgq->response;
	}
	start_index = (uint16_t)(u8frag_cnt - msgq->response)*MAX_FRAG_SIZE;
	for(i = start_index;i<(start_index + msgq->pkt_len);i++)
	{
		//eeprom_buff[i+E2P_OFFSET] = *buff_ptr;
		eeprom_buff[i] = *buff_ptr;
		buff_ptr++;
	} 	
	if(msgq->response == 1)
	{
		u8frag_cnt = 0;	
		m_u8EEPROM_criticalDataStore();	
		ret = SUCCESS;
	}		 
	return ret;
}*/
/*
G_RESP_CODE m_u8E2P_Req_resp_hdlr(void)
{
	G_RESP_CODE ret = FAILURE;
	APP_MSG_TYPE_ST loc_msgq;
	loc_msgq.pkt_len = 255;
	loc_msgq.pkt_sts = MSG_COMMAND;
	loc_msgq.command = E2P_SEND;
	loc_msgq.response = NO_RESPONSE;
	//loc_msgq.data = eeprom_buff+E2P_OFFSET;
	loc_msgq.data = eeprom_buff;
	m_u8EEPROM_criticalDataRetrieve();
	ret = m_u8MSGQ_msg_send(&loc_msgq,FP_QUEUE);
	if(ret == MSGQ_NOT_ENOUGH)
	{	
		//error need to be handled
		loc_msgq.pkt_sts = MSG_COMMAND;
	}
	return ret;
}*/

#if 0
void m_u8E2p_EXT_KEY_set(void)
{
	m95_ERROR_CODE resp; 
	if(m_u8MKEY_type_get() == UI_KEY)
	{
		resp = h_u8EEPROM_page_read((uint8_t *)eeprom_buff,CRITICAL_DATA_PAGE);
		if(resp != EEPROM_SUCCESS)
		{
			//PG_NO_OUT_OF_BOUND
			if(resp == PG_NO_OUT_OF_BOUND)
			{
				
			}
			else
			{
				
			}
			/***error need to be handled******/
			#ifdef DEBUG_ENABLE				 
			sprintf(debug_arr,"e2p_read_fail\r");
			R_UART1_Send((uint8_t *)debug_arr,DEBUG_LEN);
			while(!u8_log_uart_tx_flag);
			u8_log_uart_tx_flag = 0;
			#endif
		}
		else
		{
			 
		}
		eeprom_buff[MODE_CTRL_CONFIG_START_INDEX] = EXT_KEY;
		resp = h_u8EEPROM_page_write(eeprom_buff,sizeof(eeprom_buff),DAT);
		if(resp != EEPROM_SUCCESS)
		{
			//WRITE_IN_PROGRESS
			if(resp == WRITE_IN_PROGRESS)
			{
				/***error need to be handled******/
				#ifdef DEBUG_ENABLE				 
				sprintf(debug_arr,"e2p_WIP\r");
				R_UART1_Send((uint8_t *)debug_arr,DEBUG_LEN);
				while(!u8_log_uart_tx_flag);
				u8_log_uart_tx_flag = 0;
				#endif
			}
			else
			{
				 /***error need to be handled****/
			}
		}
		else
		{
			m_u8EEPROM_criticalDataRetrieve();		 
		}
	}
}
#endif

void m_u8E2p_BP_DataStore(void)
{
	m95_ERROR_CODE resp;
	G_RESP_CODE ret;
	uint16_t temp_page = 0;	
	RTC_DATE_TIME ref_time;	
	temp_page = h_u8EEPROM_page_get();
	resp = h_u8EEPROM_page_read((uint8_t *)eeprom_buff,CRITICAL_DATA_PAGE);
	if(resp != EEPROM_SUCCESS)
	{
		//PG_NO_OUT_OF_BOUND
		if(resp == PG_NO_OUT_OF_BOUND)
		{
			
		}
		else
		{
			
		}
		/***error need to be handled******/
		#ifdef DEBUG_ENABLE				 
		sprintf(debug_arr,"e2p_read_fail\r");
		R_UART1_Send((uint8_t *)debug_arr,DEBUG_LEN);
		while(!u8_log_uart_tx_flag);
		u8_log_uart_tx_flag = 0;
		#endif
	}
	else
	{
		 
	}	 
	ret = m_u8RTC_time_get(&ref_time);
	if(SUCCESS == ret)
	{
		/*eeprom_buff[E2P_OFFSET+REF_RTC_YEAR] = ref_time.u8Year;
		eeprom_buff[E2P_OFFSET+REF_RTC_MONTH] = ref_time.u8Month;
		eeprom_buff[E2P_OFFSET+REF_RTC_DAY] = ref_time.u8Day;
		eeprom_buff[E2P_OFFSET+REF_RTC_HOUR] = ref_time.u8Hour;
		eeprom_buff[E2P_OFFSET+REF_RTC_MINUTE] = ref_time.u8Minute;*/
		
		eeprom_buff[REF_RTC_YEAR] = ref_time.u8Year;
		eeprom_buff[REF_RTC_MONTH] = ref_time.u8Month;
		eeprom_buff[REF_RTC_DAY] = ref_time.u8Day;
		eeprom_buff[REF_RTC_HOUR] = ref_time.u8Hour;
		eeprom_buff[REF_RTC_MINUTE] = ref_time.u8Minute;
		
	}
	
	//eeprom_buff[E2P_OFFSET+CURR_E2P_PAGE] = (uint8_t)((temp_page & 0xff00)>>8);
	//eeprom_buff[E2P_OFFSET+CURR_E2P_PAGE+1] = (uint8_t)(temp_page & 0x00ff);
	
	eeprom_buff[CURR_E2P_PAGE] = (uint8_t)((temp_page & 0xff00)>>8);
	eeprom_buff[CURR_E2P_PAGE+1] = (uint8_t)(temp_page & 0x00ff);
	resp = h_u8EEPROM_page_write(eeprom_buff,sizeof(eeprom_buff),DAT);
	if(resp != EEPROM_SUCCESS)
	{
		//WRITE_IN_PROGRESS
		if(resp == WRITE_IN_PROGRESS)
		{
			/***error need to be handled******/
			#ifdef DEBUG_ENABLE				 
			sprintf(debug_arr,"e2p_WIP\r");
			R_UART1_Send((uint8_t *)debug_arr,DEBUG_LEN);
			while(!u8_log_uart_tx_flag);
			u8_log_uart_tx_flag = 0;
			#endif
		}
		else
		{
			 /***error need to be handled****/
		}
	}
	else
	{
		m_u8EEPROM_criticalDataRetrieve();		 
	} 
}
static m95_ERROR_CODE m_u8EEPROM_criticalDataStore(void)
{	 
	m95_ERROR_CODE resp;
	G_RESP_CODE ret;
	uint16_t temp_page = 0;	
	RTC_DATE_TIME ref_time;
	temp_page = h_u8EEPROM_page_get();
	//eeprom_buff[E2P_OFFSET+CURR_E2P_PAGE] = (uint8_t)((temp_page & 0xff00)>>8);
	//eeprom_buff[E2P_OFFSET+CURR_E2P_PAGE+1] = (uint8_t)(temp_page & 0x00ff);
	
	eeprom_buff[CURR_E2P_PAGE] = (uint8_t)((temp_page & 0xff00)>>8);
	eeprom_buff[CURR_E2P_PAGE+1] = (uint8_t)(temp_page & 0x00ff);
	ret = m_u8RTC_time_get(&ref_time);
	if(SUCCESS == ret)
	{
		/*eeprom_buff[E2P_OFFSET+REF_RTC_YEAR] = ref_time.u8Year;
		eeprom_buff[E2P_OFFSET+REF_RTC_MONTH] = ref_time.u8Month;
		eeprom_buff[E2P_OFFSET+REF_RTC_DAY] = ref_time.u8Day;
		eeprom_buff[E2P_OFFSET+REF_RTC_HOUR] = ref_time.u8Hour;
		eeprom_buff[E2P_OFFSET+REF_RTC_MINUTE] = ref_time.u8Minute;*/
		
		eeprom_buff[REF_RTC_YEAR] = ref_time.u8Year;
		eeprom_buff[REF_RTC_MONTH] = ref_time.u8Month;
		eeprom_buff[REF_RTC_DAY] = ref_time.u8Day;
		eeprom_buff[REF_RTC_HOUR] = ref_time.u8Hour;
		eeprom_buff[REF_RTC_MINUTE] = ref_time.u8Minute;
	}
	resp = h_u8EEPROM_page_write(eeprom_buff,sizeof(eeprom_buff),DAT);
	if(resp != EEPROM_SUCCESS)
	{
		//WRITE_IN_PROGRESS
		if(resp == WRITE_IN_PROGRESS)
		{
			/***error need to be handled******/
			#ifdef DEBUG_ENABLE				 
			sprintf(debug_arr,"e2p_WIP\r");
			R_UART1_Send((uint8_t *)debug_arr,DEBUG_LEN);
			while(!u8_log_uart_tx_flag);
			u8_log_uart_tx_flag = 0;
			#endif
		}
		else
		{
			 /***error need to be handled****/
		}
	}
	else
	{
		m_u8EEPROM_criticalDataRetrieve();		 
	}	
	return resp;
}
G_RESP_CODE m_u8EEPROM_criticalDataRetrieve(void)
{ 
	G_RESP_CODE ret = FAILURE;
	m95_ERROR_CODE resp;
	uint16_t temp_page = 0;
	 
	u8GL_strclear(eeprom_buff,MAX_EEPROM_BUFF_SIZE);
	resp = h_u8EEPROM_page_read((uint8_t *)eeprom_buff,CRITICAL_DATA_PAGE);
	if(resp != EEPROM_SUCCESS)
	{
		//PG_NO_OUT_OF_BOUND
		if(resp == PG_NO_OUT_OF_BOUND)
		{
			
		}
		else
		{
			
		}
		/***error need to be handled******/
		#ifdef DEBUG_ENABLE				 
		sprintf(debug_arr,"e2p_read_fail\r");
		R_UART1_Send((uint8_t *)debug_arr,DEBUG_LEN);
		while(!u8_log_uart_tx_flag);
		u8_log_uart_tx_flag = 0;
		#endif
	}
	else
	{
		ret = SUCCESS;
	}
	//temp_page = (uint16_t)eeprom_buff[E2P_OFFSET+CURR_E2P_PAGE];
	temp_page = (uint16_t)eeprom_buff[CURR_E2P_PAGE];
	temp_page = temp_page << 8;
	//temp_page= (temp_page)| (uint16_t)eeprom_buff[E2P_OFFSET+CURR_E2P_PAGE+1];	
	temp_page= (temp_page)| (uint16_t)eeprom_buff[CURR_E2P_PAGE+1];
	h_u8EEPROM_page_set(temp_page);
	
	/*m_u8RTC_ref_set(eeprom_buff+E2P_OFFSET);
	m_u8fill_zone_details(eeprom_buff+E2P_OFFSET);
	m_u8fill_ph_details(eeprom_buff+E2P_OFFSET);
	m_u8fill_sys_details(eeprom_buff+E2P_OFFSET);
	m_u8ModeHdlr_update(eeprom_buff+E2P_OFFSET);*/
	
	m_u8RTC_ref_set(eeprom_buff);
	//m_u8fill_zone_details(eeprom_buff);
	//m_u8fill_ph_details(eeprom_buff);
	//m_u8fill_sys_details(eeprom_buff);
	//m_u8ModeHdlr_update(eeprom_buff);
	return ret;
}
G_RESP_CODE m_u8EEPROM_test(void)
{
	G_RESP_CODE ret = WAITING;
	m95_ERROR_CODE resp;
	uint8_t test = 0;	 
	int i=0;
	for(i=0;i<MAX_EEPROM_BUFF_SIZE;i++)
	{
		if(i%2==0)
		{
			E2P_log_buff[i]=0x55;
			//E2P_log_buff[i]='D';
		}
		else
		{
			E2P_log_buff[i]=0xAA;	
			//E2P_log_buff[i]='E';
		}	
	}
	resp = h_u8EEPROM_page_write(E2P_log_buff,sizeof(E2P_log_buff),E2P_TEST);
	if(resp != EEPROM_SUCCESS)
	{
		ret = E2P_NOT_OK;
		/***error need to be handled******/
		#ifdef DEBUG_ENABLE				 
		sprintf(debug_arr,"e2p_write_fail\r");
		R_UART1_Send((uint8_t *)debug_arr,DEBUG_LEN);
		while(!u8_log_uart_tx_flag);
		u8_log_uart_tx_flag = 0;
		#endif
	}
	u8GL_strclear(E2P_log_buff,MAX_EEPROM_BUFF_SIZE);
	
	resp = h_u8EEPROM_page_read((uint8_t *)E2P_log_buff,E2P_TEST);
	if(resp != EEPROM_SUCCESS)
	{
		ret = E2P_NOT_OK;
		/***error need to be handled******/
		#ifdef DEBUG_ENABLE				 
		sprintf(debug_arr,"e2p_read_fail\r");
		R_UART1_Send((uint8_t *)debug_arr,DEBUG_LEN);
		while(!u8_log_uart_tx_flag);
		u8_log_uart_tx_flag = 0;
		#endif
	}
	else
	{
		ret = E2P_OK;
		for(i=0;i<MAX_EEPROM_BUFF_SIZE;i++)
		{	
			if(((i%2==0)&&(E2P_log_buff[i]==0x55))||((i%2==1)&&(E2P_log_buff[i]==0xAA)))
			{
				/*****nothing to be done*********/
			}
			else
			{
				ret = E2P_NOT_OK;
				break;
			}	
		}
	}	 
	u8GL_strclear(eeprom_buff,MAX_EEPROM_BUFF_SIZE);
	m_u8EEPROM_criticalDataRetrieve();
	return ret;
}
G_RESP_CODE m_vE2P_log_reset(void)
{
	G_RESP_CODE resp = EEPROM_SUCCESS;
	int i = 0;
	for(i = 0;i < MAX_EEPROM_BUFF_SIZE;i++)
	{
		E2P_log_buff[i]=0xff;
		//eeprom_buff[i] = 0x00;
	}
	for(i = LOG_PAGE_START;i<=LOG_PAGE_END;i++)
	{
		resp = h_u8EEPROM_page_write((uint8_t *)E2P_log_buff,sizeof(E2P_log_buff),LOG);
		if(resp != EEPROM_SUCCESS)
		{
			if(resp == WRITE_IN_PROGRESS)
			{
				
			}
			else
			{
				 
			}
			 /***error need to be handled******/
			#ifdef DEBUG_ENABLE			 
			sprintf(debug_arr,"log_write_fail\r");
			R_UART1_Send((uint8_t *)debug_arr,DEBUG_LEN);
			while(!u8_log_uart_tx_flag);
			u8_log_uart_tx_flag = 0;
			#endif
		}
	}
	//resp = h_u8EEPROM_page_write(eeprom_buff,sizeof(eeprom_buff),DAT);
	return resp;
}
G_RESP_CODE m_u8EEPROM_log_store(void)
{	 
	
	/* LOG FORMAT
	   $ TIME,D/N/DIS >Z0 =A/I,NO/NC,DLY,D/N,ZO/ZC;  */
	   
	   
	uint16_t len;
	uint8_t k = 0;
	uint8_t *ptr = NULL;
	G_RESP_CODE resp = EEPROM_SUCCESS;	 
	uint8_t loc_date_time[GSM_TIME_LEN]={0};
	
	ptr = E2P_log_buff;
	
	
	m_u8RTC_get_str(loc_date_time);
	u8GL_strclear(E2P_log_buff,MAX_EEPROM_BUFF_SIZE);	 
	 
	*ptr = '$';
        ptr++;
	k =  u8GL_ncopy(ptr,(uint8_t *)loc_date_time,GSM_TIME_LEN);
        ptr = ptr + k;
        *ptr = ' ';
        ptr++;
	 
	//m_u8log_zone_data(ptr);
	 
	#if 1
	resp = h_u8EEPROM_page_write((uint8_t *)E2P_log_buff,sizeof(E2P_log_buff),LOG);
	if(resp != EEPROM_SUCCESS)
	{
		if(resp == WRITE_IN_PROGRESS)
		{
			
		}
		else
		{
			 
		}
		 /***error need to be handled******/
		#ifdef DEBUG_ENABLE				 
		sprintf(debug_arr,"log_write_fail\r");
		R_UART1_Send((uint8_t *)debug_arr,DEBUG_LEN);
		while(!u8_log_uart_tx_flag);
		u8_log_uart_tx_flag = 0;
		#endif
	}
	else
	{
		m_u8E2p_BP_DataStore();
		 /***error need to be handled******/
		#ifdef DEBUG_ENABLE				 
		sprintf(debug_arr,"log_write_fail\r");
		R_UART1_Send((uint8_t *)E2P_log_buff,sizeof(E2P_log_buff));
		while(!u8_log_uart_tx_flag);
		u8_log_uart_tx_flag = 0;
		#endif
	}
	#endif
	return 0;
}

G_RESP_CODE m_u8EEPROM_log_retrieve(uint8_t log_cmd)
{
	uint16_t u16Len = 0;
	uint8_t FL_ret = WAITING,i =0,j = 0, k = 0;
	switch(log_cmd)
	{
		case LOG_START:
		{
			switch(u8E2P_log_start)
			{
				case LOG_INIT:
				{
					log_e2p_page = h_u8EEPROM_page_get();					 
					u8E2P_log_start = LOG_TRIG;
					/*****break intentionally left********/
				}
				case LOG_TRIG:
				{
					for(i = 0;i < 50;i++)
					{
						if((log_e2p_page-1) < LOG_PAGE_START)
						{
							log_e2p_page = LOG_PAGE_END;
						}
						else
						{
							log_e2p_page--;
						}
						if(h_u8EEPROM_page_get() == log_e2p_page)
						{
							log_e2p_page = LOG_PAGE_START;
							u8E2P_log_start = LOG_INIT;
							FL_ret = SUCCESS;
							u8GL_strclear(E2P_log_buff,MAX_EEPROM_BUFF_SIZE);							 
							break;
						}
						else
						{
							h_u8EEPROM_page_read(E2P_log_buff,log_e2p_page);
							u16Len = strlen((char *)E2P_log_buff);
							if(u16Len!=0)
							{
								for(j = 0;j < 50;j++)
								{
									if(0xFF == E2P_log_buff[j])
									{
										k++;
									}
								}
								if(k<20)
								{
									u8_log_uart_tx_flag = INT_IDLE;
									R_UART1_Send(E2P_log_buff,u16Len);
									timeout_chk(&u8_log_uart_tx_flag,8,DEFAULT_TIMEOUT_VALUE);									 
								}
								k = 0;	
							}
						}					
					}
					break;
				}
				default:
				break;
			}					
			break;
		}
		case LOG_STP:
		{
			log_e2p_page = LOG_PAGE_START;
			u8E2P_log_start = LOG_INIT;
			FL_ret = SUCCESS;
			u8GL_strclear(E2P_log_buff,MAX_EEPROM_BUFF_SIZE);
			break;
		}
		default:
		break;
	}	 
	return FL_ret;
}
#ifdef STARTUP_E2P_CLEAR
void m_vE2P_clear(void)
{
	m95_ERROR_CODE resp;
	uint16_t temp_page;
	G_RESP_CODE ret;
	RTC_DATE_TIME ref_time;
	
	u8GL_strclear(eeprom_buff,MAX_EEPROM_BUFF_SIZE);
	
	ret = m_u8RTC_time_get(&ref_time);
	if(SUCCESS == ret)
	{
		eeprom_buff[REF_RTC_YEAR] = ref_time.u8Year;
		eeprom_buff[REF_RTC_MONTH] = ref_time.u8Month;
		eeprom_buff[REF_RTC_DAY] = ref_time.u8Day;
		eeprom_buff[REF_RTC_HOUR] = ref_time.u8Hour;
		eeprom_buff[REF_RTC_MINUTE] = ref_time.u8Minute;
	}
	temp_page = LOG_PAGE_START;
	eeprom_buff[CURR_E2P_PAGE] = (uint8_t)((temp_page & 0xff00)>>8);
	eeprom_buff[CURR_E2P_PAGE+1] = (uint8_t)(temp_page & 0x00ff);
	
	resp = h_u8EEPROM_page_write(eeprom_buff,sizeof(eeprom_buff),DAT);
	if(resp != EEPROM_SUCCESS)
	{
		//WRITE_IN_PROGRESS
		if(resp == WRITE_IN_PROGRESS)
		{
			/***error need to be handled******/
			#ifdef DEBUG_ENABLE				 
			sprintf(debug_arr,"e2p_WIP\r");
			R_UART1_Send((uint8_t *)debug_arr,DEBUG_LEN);
			while(!u8_log_uart_tx_flag);
			u8_log_uart_tx_flag = 0;
			#endif
		}
		else
		{
			 /***error need to be handled****/
		}
	}
	else
	{
		m_u8EEPROM_criticalDataRetrieve();		 
	}
}
#endif
