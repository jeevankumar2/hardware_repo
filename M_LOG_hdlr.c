#include "M_LOG_hdlr.h"
#include "M_rtc_handler.h"
#include "r_cg_serial.h"  
#include "M_GeneralLib.h"
#include "M_zone_handler.h"
#include "H_M95M01_eeprom_driver.h"
#include "M_GeneralLib.h"
#include "M_time_out.h"
#include "string.h"
#include "stdio.h"

/******************************************DEFNITIONS_START***************************************/
#define MAX_RETRY_CNT 5
#define MAX_LOG_BUFF_SIZE 256

#define MAX_MSG_CHAR_CNT 30

#define RESPONSE_ARR_LEN 5
#define ADMIN_TIMEOUT 120
#define LOG_INIT 0
#define LOG_TRIG 1

/******************************************ENUM_START***************************************/

typedef enum
{	
	IDLE=0,
	RTC_GET,
	LOG_STORE,
	RTC_RETRY,
	E2P_RETRY,
}E_LOG_MAIN_STATE;

typedef enum
{	
	LS_USER=0,
	LS_TAKELOG,
	LS_DELETELOG,
	
}E_LOG_STATES;

typedef enum
{	
	AUTHENTICATE=0,
	USER,
	
}MAIN_STATE;

typedef enum
{
	LC_NOCMD=0,
	LC_TAKE,
	LC_STOP,
	LC_DELETE,
	LC_SPWD,
	LC_RTC_GET,
	LC_RTC_SET,
	LC_LOGOUT,
		
}E_LOG_CMD;

/******************************************VARIABLES_START***************************************/


#ifdef DEBUG_ENABLE

char debug_arr[DEBUG_LEN]={0};
#endif

MAIN_STATE main_state = AUTHENTICATE;
E_LOG_STATES e_log_states = LS_USER;
E_LOG_MAIN_STATE e_log_main_state = IDLE ; 
uint8_t retry_cnt = 0;
//test variables
uint8_t val = 'A';
uint8_t change = 0;
uint8_t log_event =0;

extern uint8_t u8_log_uart_tx_flag;

uint8_t char_cnt = 0;
uint8_t u8_pwd[5]={"1234"};
uint8_t loc_date_time[RTC_TIME_LEN]={0};
int8_t h_u8LOG_End_cnt = 0;
uint8_t u8log_tout_cnt = 0;
CIRC_BUFF_TYPE LOG_Rx_buff[MAX_CIRC_BUFF_DEPTH];
CIRC_BUFF_ST Rx_LOG_Circ = {LOG_Rx_buff,0,0,MAX_CIRC_BUFF_DEPTH};
LOG_LINE_TYPE line_ptr = LOG_LINE_HEAD_L;
static uint8_t u8E2P_log_start = LOG_INIT;

static int16_t log_e2p_page = LOG_PAGE_START;

uint8_t E2P_log_buff[MAX_EEPROM_BUFF_SIZE] = {0};

/******************************************FUNCTIONS_START***************************************/
//test function
void m_u8TEST_LOG(uint8_t *ptr);

G_RESP_CODE E2P_logstore(uint8_t *buff_ptr,uint16_t buff_len); 
G_RESP_CODE h_u8LOG_buf_chk(CIRC_BUFF_ST *circ_buff);
G_RESP_CODE h_u8LOG_buf_put(CIRC_BUFF_ST *circ_buff,CIRC_BUFF_TYPE *msg_val);
void h_u8LOG_CIRCflush(void);
int8_t h_u8LOG_CIRCsearch(CIRC_BUFF_ST *circ_buff,CIRC_BUFF_TYPE *str,uint8_t len);
G_RESP_CODE h_u8LOG_Tx_chkdata(LOG_Q_NAME qname);
int8_t h_u8LOG_CIRCcopy(CIRC_BUFF_ST *circ_buff,uint8_t index,CIRC_BUFF_TYPE *str,uint8_t len);
uint8_t m_u8MSGQ_buf_cap_chk(CIRC_BUFF_ST *circ_buff);
uint8_t m_u8get_LOG_buff_len(void);
void buff_reset(void);
uint8_t get_tail(void);
G_RESP_CODE m_vE2P_log_reset(void);
G_RESP_CODE m_u8EEPROM_log_retrieve(uint8_t log_cmd);
void m_vLOG_logdata(void);
#ifdef STARTUP_E2P_CLEAR
void m_vE2P_clear(void);
#endif


void LOG_Hdlr(void)
{
	G_RESP_CODE ret = WAITING;
	uint8_t k = 0;
	uint8_t *ptr = NULL;
	switch(e_log_main_state)
	{
		case IDLE:
		{
			break;
		}
		case RTC_GET:
		{ 
			ret = m_u8RTC_get_str(loc_date_time);
			if(ret ==FAILURE)
			{
				retry_cnt = 0;
				e_log_main_state = RTC_RETRY; 
				break;
			}
			
		}
		case LOG_STORE:
		{
			ptr = E2P_log_buff;
			u8GL_strclear(E2P_log_buff,MAX_LOG_BUFF_SIZE);
			*ptr = '$';
        		ptr++;
			k =  u8GL_ncopy(ptr,(uint8_t *)loc_date_time,RTC_TIME_LEN);
        		ptr = ptr + k;
        		*ptr = ' ';
        		ptr++;
			m_u8log_zone_data(ptr);
			//m_u8TEST_LOG(ptr);
			ret = E2P_logstore(E2P_log_buff,MAX_LOG_BUFF_SIZE);
			if(ret == FAILURE)
			{
				e_log_main_state = E2P_RETRY; 	
			}
			else
			{
				e_log_main_state = IDLE;	
			}
			retry_cnt = 0;
			break;
		}
		case RTC_RETRY:
		{
			ret = m_u8RTC_get_str(loc_date_time);
			if(ret ==SUCCESS)
			{
				retry_cnt = 0;
				e_log_main_state = LOG_STORE; 
				
			}
			else
			{
				retry_cnt++;
				if(retry_cnt > MAX_RETRY_CNT)
				{
					retry_cnt = 0;
					u8GL_ncopy(loc_date_time,(uint8_t *)"RTC TIME ERR",13); 
					e_log_main_state = LOG_STORE;
					
				}
			}
			break;
		}
		case E2P_RETRY:
		{
			ret = E2P_logstore(E2P_log_buff,MAX_LOG_BUFF_SIZE);
			if(ret ==SUCCESS)
			{
				retry_cnt = 0;
				e_log_main_state = IDLE; 
				
			}
			else
			{
				retry_cnt++;
				if(retry_cnt > MAX_RETRY_CNT)
				{
					retry_cnt = 0;
					e_log_main_state = IDLE;
					
				}
			}
			break;
		}
		default:
		break;	
	}
}



G_RESP_CODE admin(void)
{
	G_RESP_CODE ret = WAITING;
	int8_t head = -1;
	uint8_t len=0;
	ret = h_u8LOG_Tx_chkdata(LOG_QUEUE);
	if(ret == DATA_AVAILABLE)
	{
		head = h_u8LOG_CIRCsearch(&Rx_LOG_Circ,"ADMIN",5);
		if(head != -1)
		{
			len = m_u8get_LOG_buff_len();
			if((len==9)&&(get_tail() == head))
			{
				head = h_u8LOG_CIRCsearch(&Rx_LOG_Circ,(char *)u8_pwd,4);
				if(head != -1)
				{
					R_UART1_Send((uint8_t *)"LOGGED IN",10);
					ret = SUCCESS;
				}
				else
				{
					R_UART1_Send((uint8_t *)"INVALID FORMAT",15);
				}
			}
			else
			{
				R_UART1_Send((uint8_t *)"INVALID FORMAT",15);	
			}
			
		}
		else
		{
			R_UART1_Send((uint8_t *)"LOG IN FIRST",13);	
		}
		buff_reset();
	}
	return ret;	
}
 
#if 0
G_RESP_CODE cmd_hdlr(void)
{
	G_RESP_CODE ret = WAITING; 
	switch(main_state)
	{
		case AUTHENTICATE:
		{
			log_event++;
			
			if(log_event >10)
			{
			log_event =0;
			m_vLOG_logdata();
			}
			ret = admin();
			if(ret != WAITING)
			{
				if(ret == SUCCESS)
				{
					m_vLOG_storelog_stop();
					main_state = USER;		
				}	
			}
			break;
		}
		case USER:
		{
			ret = user();
			if(ret != WAITING)
			{
				if(ret == SUCCESS)
				{
					main_state = AUTHENTICATE;		
				}	
			}
			break;
		}
		default:
		break;
		
		
	}
		
	return ret;	
	
}
#endif

G_RESP_CODE user(void)
{
	G_RESP_CODE G_ret = WAITING;
	G_RESP_CODE ret = WAITING;
	int8_t head = -1;
	uint8_t len=0;
	uint8_t i = 0;
	char pwd_arr[5]={0};
	E_LOG_CMD e_log_cmd = LC_TAKE;
	len = m_u8get_LOG_buff_len();
	switch(e_log_states)
	{
		case LS_USER:
		{
			ret = h_u8LOG_Tx_chkdata(LOG_QUEUE);
			if(ret == DATA_AVAILABLE)
			{
				u8log_tout_cnt = 0;
				ret = SUCCESS;
				switch(e_log_cmd)
				{
					case LC_TAKE:
					{
						head = h_u8LOG_CIRCsearch(&Rx_LOG_Circ,"TAKE",4);
						if(head != -1)
						{
							if((len==4)&&(get_tail() == head))
							{
								R_UART1_Send((uint8_t *)"LOGGING",8);
								e_log_states = LS_TAKELOG;
								
							}
							else
							{
								ret = FAILURE;
							}
							break;
						}
					}
					case LC_DELETE:
					{
						
						head = h_u8LOG_CIRCsearch(&Rx_LOG_Circ,"DELETE",6);
						if(head != -1)
						{
							if((len==6)&&(get_tail() == head))
							{
								R_UART1_Send((uint8_t *)"DELETING",9);
								e_log_states = LS_DELETELOG;
							}
							else
							{
								ret = FAILURE;
							}
							break;
						}
					}
							
					
					case LC_SPWD:
					{
						
						head = h_u8LOG_CIRCsearch(&Rx_LOG_Circ,"SPWD",4);
						if(head != -1)
						{
							if((len==8)&&(get_tail() == head))
							{
								h_u8LOG_CIRCcopy(&Rx_LOG_Circ,head+4,(char *)pwd_arr,4);
								for(i=0;i<4;i++)
								{
									if((pwd_arr[i]<58)&&(pwd_arr[i]>47))
									{
										
									}
									else
									{
										break;
									}
								}
								if(i == 4)
								{
									//h_u8LOG_CIRCcopy(&Rx_LOG_Circ,head+4,(char *)u8_pwd,4);
									u8GL_ncopy(u8_pwd,(uint8_t*)pwd_arr,4);
									m_u8EEPROM_criticalDataStore();
									R_UART1_Send((uint8_t *)"PWD SAVED",10);
								}
								else
								{
									R_UART1_Send((uint8_t *)"INVALID CHARACTERS",19);
								}
								
							}
							else
							{
								ret = FAILURE;
							}
							break;
						}
						
							
					}
					case LC_RTC_GET:
					{
						head = h_u8LOG_CIRCsearch(&Rx_LOG_Circ,"RTCGET",6);
						if(head != -1)
						{
							if((len==6)&&(get_tail() == head))
							{
								ret = m_u8RTC_get_str(loc_date_time);
								if(ret == SUCCESS)
								{
									R_UART1_Send(loc_date_time,strlen((char *)loc_date_time));
									
								}
								else
								{
									R_UART1_Send((uint8_t *)"SET TIME FIRST",15);
									ret = SUCCESS;
								}
			
								
							}
							else
							{
								ret = FAILURE;
							}
							break;
						} 
					}
					case LC_RTC_SET:
					{
						
						head = h_u8LOG_CIRCsearch(&Rx_LOG_Circ,"RTCSET",6);
						if(head != -1)
						{
							len = m_u8get_LOG_buff_len();
							if((len==24)&&(get_tail() == head))
							{
								h_u8LOG_CIRCcopy(&Rx_LOG_Circ,head+6,(char *)loc_date_time,18);
								ret = m_u8RTC_strTime_set(loc_date_time);
								if(ret == SUCCESS)
								{
									
									R_UART1_Send((uint8_t *)"TIME SET SUCCESS",17);	
								}
								else
								{
									R_UART1_Send((uint8_t *)"INVALID TIME",13);
									ret = SUCCESS; 
								}
								u8GL_strclear(loc_date_time,RTC_TIME_LEN);
								
							}
							else
							{
								ret = FAILURE;
							}
							break;
						}
						
					}
					case LC_LOGOUT:
					{
						
						head = h_u8LOG_CIRCsearch(&Rx_LOG_Circ,"LOGOUT",6);
						if(head != -1)
						{
							if((len==6)&&(get_tail() == head))
							{
								R_UART1_Send((uint8_t *)"LOGGED OUT",11);
								G_ret =SUCCESS;
								u8log_tout_cnt = 0;
							
							}
							else
							{
								ret = FAILURE;
							}
							break;
						}		
					}
					default:
					{
						R_UART1_Send((uint8_t *)"CMD INVALID REFER USER MANUAL",30);
						break;
					}
					
				}
				if(ret == FAILURE)
				{
					R_UART1_Send((uint8_t *)"CMD INVALID REFER USER MANUAL",30);	
				}
				buff_reset();
			}
			else
			{
				u8log_tout_cnt++;
				if(u8log_tout_cnt > ADMIN_TIMEOUT)
				{
					R_UART1_Send((uint8_t *)"TIMED OUT",10);	
					G_ret  =SUCCESS;
					u8log_tout_cnt = 0;
				}
			}
			break;
		}
		case LS_TAKELOG:
		{
			ret = m_u8EEPROM_log_retrieve(LOG_START);
			if(ret == SUCCESS)
			{
				R_UART1_Send((uint8_t *)"LOG FINISHED",13);
				e_log_states = LS_USER;
				m_u8EEPROM_log_retrieve(LOG_STP);
			}
			else
			{
				ret = h_u8LOG_Tx_chkdata(LOG_QUEUE);
				if(ret == DATA_AVAILABLE)
				{
					head = h_u8LOG_CIRCsearch(&Rx_LOG_Circ,"STOP",4);
					if(head != -1)
					{
						len = m_u8get_LOG_buff_len();
						if((len==4)&&(get_tail() == head))
						{
						R_UART1_Send((uint8_t *)"LOG STOPPED",12);
						e_log_states = LS_USER;
						m_u8EEPROM_log_retrieve(LOG_STP);
						}
					}
					else
					{
						R_UART1_Send((uint8_t *)"STOP LOGGING FIRST",19);	
					}
					buff_reset();
				}
			}
			break;	
				
		}
		case LS_DELETELOG:
		{
			m_vE2P_log_reset();
			R_UART1_Send((uint8_t *)"DELETED",8);
			e_log_states = LS_USER;
			break;
		}
		default:
		break;	
	}
	return G_ret;
}

G_RESP_CODE E2P_logstore(uint8_t *buff_ptr,uint16_t buff_len)  
{
	G_RESP_CODE ret = WAITING;
	ret = h_u8EEPROM_page_write(buff_ptr ,MAX_EEPROM_BUFF_SIZE,LOG);
	if(ret != EEPROM_SUCCESS)
	{
		ret = FAILURE;
	}
	else
	{	
		m_u8EEPROM_criticalDataStore();
		ret = SUCCESS;
	}
	return ret;
	
}



G_RESP_CODE h_u8LOG_Q_send(LOG_Q_NAME qname,CIRC_BUFF_TYPE *val)
 {
	 G_RESP_CODE ret  = SUCCESS;
	 switch(line_ptr)
	 {
		case LOG_LINE_HEAD_L:
		{
			if(*val == '$')
			{
				line_ptr = LOG_LINE_TAIL_H;
			}
			break;
		}
		case LOG_LINE_TAIL_H:
		{
			if(*val == ';')
			{
				line_ptr=IDLE_WAIT;
				h_u8LOG_End_cnt++;		        				
			}
			else if(*val == '$')
			{
				buff_reset();
				line_ptr = LOG_LINE_TAIL_H;
			}
			else
			{
				h_u8LOG_buf_put(&Rx_LOG_Circ,val);
				char_cnt++;
				if(char_cnt >MAX_MSG_CHAR_CNT)
				{
					buff_reset();
				}
			}
			break;
		}
		case IDLE_WAIT:
		{
			break;	
		}
		default:
		break;
	 }	 
	 return ret;
 }

G_RESP_CODE h_u8LOG_Tx_chkdata(LOG_Q_NAME qname)
{
	G_RESP_CODE ret = WAITING;
	if(h_u8LOG_End_cnt > 0)
	{
		ret = DATA_AVAILABLE;
	}
	return ret;
}

uint8_t get_tail(void)
{
	uint8_t ret = 0;
	ret = Rx_LOG_Circ.tail;
	return ret;
}
 
G_RESP_CODE h_u8LOG_Q_chk(LOG_Q_NAME qname)
 {
	 G_RESP_CODE ret  = WAITING;
	 ret = h_u8LOG_buf_chk(&Rx_LOG_Circ);
	 return ret;
 }
 
 G_RESP_CODE h_u8LOG_buf_put(CIRC_BUFF_ST *circ_buff,CIRC_BUFF_TYPE *msg_val)
 {
	G_RESP_CODE ret = SUCCESS;
	uint8_t temp_head = 0;
	temp_head = circ_buff->head;
	circ_buff->buffer[temp_head] = *msg_val;
	if((temp_head+1) >= circ_buff->maxlen)
	{
		if(circ_buff->tail == 0)
		{
			return MSGQ_FULL;
		}
		temp_head = 0;
	}
	else
	{
		if(circ_buff->tail == (temp_head+1))
		{
			return MSGQ_FULL;
		}
		temp_head = temp_head + 1;
	}
	//circ_buff->buffer[1].data[3] = 't';  for reference
	circ_buff->head = temp_head;
	return ret;
 }
 G_RESP_CODE h_u8LOG_buf_get(CIRC_BUFF_ST *circ_buff,CIRC_BUFF_TYPE *msg_val)
 {
	G_RESP_CODE ret = SUCCESS;
	uint8_t temp_tail = 0;
	temp_tail = circ_buff->tail;
	if((temp_tail - circ_buff->head) == 0)
	{
		return MSGQ_EMPTY;
	}
	*msg_val = circ_buff->buffer[temp_tail];
	if((temp_tail+1) >= circ_buff->maxlen)
	{
		temp_tail = 0;
	}
	else
	{
		temp_tail = temp_tail + 1;
	}
	circ_buff->tail = temp_tail;
	return ret;
 }
 G_RESP_CODE h_u8LOG_buf_chk(CIRC_BUFF_ST *circ_buff)
 {
	G_RESP_CODE ret = WAITING;	 
	if((circ_buff->tail - circ_buff->head) == 0)
	{
		return MSGQ_EMPTY;
	}
	return ret;
 }
void h_u8LOG_CIRCflush(void)
{
	Rx_LOG_Circ.tail = Rx_LOG_Circ.head;
	
}

void buff_reset(void)
{
	char_cnt = 0;
	h_u8LOG_CIRCflush();
	h_u8LOG_End_cnt = 0;
	line_ptr = LOG_LINE_HEAD_L;
}

int8_t h_u8LOG_CIRCcopy(CIRC_BUFF_ST *circ_buff,uint8_t index,CIRC_BUFF_TYPE *str,uint8_t len)
{
    int8_t ret = -1;
    uint8_t t4 = 0;
    uint8_t i = 0,r = 0;
   
    if(circ_buff->tail - circ_buff->head)
    {
	if(index >= circ_buff->maxlen)
        {
            index = index - circ_buff->maxlen;
        }
        if(circ_buff->head > circ_buff->tail)
        {
            if((index < circ_buff->tail)||(index >= circ_buff->head))
            {
                return -1;
            }
            t4 = circ_buff->head;
        }
        else
        {
            if((index < circ_buff->tail)&&(index >= circ_buff->head))
            {
                return -1;
            }
            t4 = circ_buff->maxlen;
        }
        for(i = index;i<t4;i++)
        {
             str[r] = circ_buff->buffer[i];
             r++;
             if(r == len)
             {
                 str[r] = '\0';
                  ret = i;
                  break;
             }
            if(i == circ_buff->maxlen-1)
            {
                i = -1;
                t4 = circ_buff->head;
            }
        }
    }
    str[r] = '\0';
    return ret;
}
int8_t h_u8LOG_CIRCsearch(CIRC_BUFF_ST *circ_buff,CIRC_BUFF_TYPE *str,uint8_t len)
{
    int8_t ret = -1;
    uint8_t t1 = 0,t4 = 0;
    uint8_t i = 0,r = 0;
    t1 = circ_buff->tail;
    if(circ_buff->tail - circ_buff->head)
    {
        if(circ_buff->head > circ_buff->tail)
        {
            t4 = circ_buff->head;
        }
        else
        {
            t4 = circ_buff->maxlen;
        }
        for(i = t1;i<t4;i++)
        {
            if(str[r] == circ_buff->buffer[i])
            {
                r++;
                if(r == len)
                {
                    ret = i;
                    break;
                }
            }
            else
            {
                if(r > 0)
                {
                    i--;
                }
                r = 0;
            }
            if(i == circ_buff->maxlen-1)
            {
                i = -1;
                t4 = circ_buff->head;
            }
            
        }
        if(ret != -1)
        {
            ret = ret - len + 1;
            if(ret<0)
            {
                ret = circ_buff->maxlen-(ret* -1);
            }
        }
    }
    
    return ret;
}

uint8_t m_u8MSGQ_buf_cap_chk(CIRC_BUFF_ST *circ_buff)
 {
	 uint8_t ret;
	 if(circ_buff->head > circ_buff->tail)
	 {
		 ret = (circ_buff->head - circ_buff->tail );
	 }
	 else if(circ_buff->tail > circ_buff->head)
	 {
		 ret = (circ_buff->maxlen - circ_buff->tail) + circ_buff->head ;
	 }
	 else if(circ_buff->tail == circ_buff->head)
	 {
		 ret = 0;
	 }
	 return ret;
 }
 
 uint8_t m_u8get_LOG_buff_len(void)
 {
	uint8_t len = 0;
	len = m_u8MSGQ_buf_cap_chk(&Rx_LOG_Circ);
	return len;
	 
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
	h_u8EEPROM_page_set(LOG_PAGE_START);
	//SAVING CRITICAL DATA FOR STORING CHANGED EEPROM PAGE 
	m_u8EEPROM_criticalDataStore();
	return resp;
}

#ifdef STARTUP_E2P_CLEAR
void m_vE2P_clear(void)
{
	m95_ERROR_CODE resp;
	uint16_t temp_page;
	uint8_t i = 0;
	G_RESP_CODE ret;
	
	
	u8GL_strclear(E2P_log_buff,MAX_EEPROM_BUFF_SIZE);
	for(i=0;i<4;i++)
	{
	E2P_log_buff[PWD_START_INDEX+i] = u8_pwd[i];
	}
	temp_page = LOG_PAGE_START;
	E2P_log_buff[CURR_E2P_PAGE] = (uint8_t)((temp_page & 0xff00)>>8);
	E2P_log_buff[CURR_E2P_PAGE+1] = (uint8_t)(temp_page & 0x00ff);
	
	resp = h_u8EEPROM_page_write(E2P_log_buff,sizeof(E2P_log_buff),DAT);
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

G_RESP_CODE m_u8EEPROM_criticalDataRetrieve(void)
{
	G_RESP_CODE ret = WAITING;
	uint8_t i=0;
	uint16_t temp_page = 0;
	u8GL_strclear(E2P_log_buff,MAX_EEPROM_BUFF_SIZE);
	ret = h_u8EEPROM_page_read((uint8_t *)E2P_log_buff,CRITICAL_DATA_PAGE);
	if(ret != EEPROM_SUCCESS)
	{
	
	}
	else
	{
		ret = SUCCESS;	
	}
	for(i=0;i<4;i++)
	{
	u8_pwd[i] = E2P_log_buff[PWD_START_INDEX+i];
	}
	u8_pwd[i] = 0;
	temp_page = (uint16_t)E2P_log_buff[CURR_E2P_PAGE];
	temp_page = temp_page << 8;
	temp_page= (temp_page)| (uint16_t)E2P_log_buff[CURR_E2P_PAGE+1];
	h_u8EEPROM_page_set(temp_page);
	return ret;
	
}

G_RESP_CODE m_u8EEPROM_criticalDataStore(void)
{
	G_RESP_CODE ret = WAITING;
	uint8_t i=0;
	uint16_t temp_page = h_u8EEPROM_page_get();
	u8GL_strclear(E2P_log_buff,MAX_EEPROM_BUFF_SIZE);
	for(i=0;i<4;i++)
	{
	E2P_log_buff[PWD_START_INDEX+i] = u8_pwd[i];
	}
	E2P_log_buff[CURR_E2P_PAGE] = (uint8_t)((temp_page & 0xff00)>>8);
	E2P_log_buff[CURR_E2P_PAGE+1] = (uint8_t)(temp_page & 0x00ff);
	ret = h_u8EEPROM_page_write( E2P_log_buff,sizeof(E2P_log_buff),DAT);
	if(ret != EEPROM_SUCCESS)
	{
		
	}
	else
	{
		m_u8EEPROM_criticalDataRetrieve();
		ret = SUCCESS;
	}
	return ret;
}

void log_disable(void)
{
	line_ptr = IDLE_WAIT;
	char_cnt = 0;
	h_u8LOG_CIRCflush();
	h_u8LOG_End_cnt = 0;	
}

void log_enable(void)
{
	/*char_cnt = 0;
	h_u8LOG_End_cnt = 0;
	h_u8LOG_CIRCflush();
	line_ptr = LOG_LINE_HEAD_L;*/
	buff_reset();
}

void m_vLOG_logdata(void)
{
	retry_cnt = 0;
	e_log_main_state = RTC_GET;
	
}

void m_vLOG_storelog_stop(void)
{
	retry_cnt = 0;
	e_log_main_state = IDLE;
	
}

void m_u8TEST_LOG(uint8_t *ptr)
{
	*ptr = '\r';
	ptr++;
	*ptr = '\n';
	ptr++;
	*ptr = val;
	ptr++;
	*ptr = '\r';
	ptr++;
	*ptr = '\n';
	
	change++;
	if(change > 50)
	{
	change = 0;
	val++;
	}
	
}