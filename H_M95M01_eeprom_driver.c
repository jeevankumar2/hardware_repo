#include "H_M95M01_eeprom_driver.h"
#include "r_cg_serial.h"
#include "r_cg_timer.h"
#include "string.h"
#include "M_time_out.h"
#include "H_delay_driver.h"


#define TIMEOUT_ENABLE 1

#define E2P_TIMEOUT 1//in 100ms units
 
extern uint8_t eeprom_tx_rx_flag;

uint16_t eeprom_page = LOG_PAGE_START;
void ReceiveEepromSpi(uint8_t *rx_log);
m95_ERROR_CODE SendEepromSpi(uint8_t *log,uint16_t log_len,uint32_t *address);


m95_ERROR_CODE h_u8EEPROM_page_write(uint8_t *log,uint16_t log_len,uint8_t type)
{
	uint8_t resp;
	uint16_t i = 0;	
	uint32_t eeprom_address = 0;
	if(log_len > MAX_EEPROM_BUFF_SIZE)
	{
		return LONG_WRITE_LEN;
	}
	if(eeprom_page > LOG_PAGE_END)
	{
		eeprom_page = LOG_PAGE_START;
	}
	if(eeprom_page < LOG_PAGE_START)
	{
		eeprom_page = LOG_PAGE_END;
	}
	//eeprom_address = eeprom_page * 256;	 
	if(type == DAT)
	{
		eeprom_address = CRITICAL_DATA_PAGE;
	}	
	else if (type == E2P_TEST)
	{
		eeprom_address = E2P_TEST_PAGE * 256;
	}
	else
	{
		for(i = 0;i<eeprom_page;i++)
		{
			eeprom_address = eeprom_address + M95M01_MAX_PAGE_SIZE;
		}
	}	 
	resp = SendEepromSpi(log,log_len,&eeprom_address);
	if(!resp)
	{
		if(type == LOG)
		{
			eeprom_page++;
		}		
	}
	 		 
	return resp;	 
}
m95_ERROR_CODE h_u8EEPROM_page_read(uint8_t *rx_log,uint16_t PageNo)
{
	uint32_t eeprom_address = 0;
	uint8_t dummy_tx_buff[4] = {0};
	uint16_t i = 0;
	if(PageNo > LOG_PAGE_END)
	{
		return PG_NO_OUT_OF_BOUND;
	}
	for(i = 0;i<PageNo;i++)
	{
		eeprom_address = eeprom_address + M95M01_MAX_PAGE_SIZE;
	}
	//eeprom_address = (uint32_t) (PageNo * 256);
	dummy_tx_buff[0] = READ;
	dummy_tx_buff[1] = (uint8_t)(eeprom_address>>16);
	dummy_tx_buff[2] = (uint8_t)(eeprom_address>>8);
	dummy_tx_buff[3] = (uint8_t)(eeprom_address);
	
	EEPROM_CS = 1;
	h_vDEL_100nano(1);
	EEPROM_CS = 0;
	h_vDEL_100nano(1);
	eeprom_tx_rx_flag = INT_IDLE; 
	R_CSI20_Send_Receive(dummy_tx_buff,4,NULL);
	timeout_chk(&eeprom_tx_rx_flag,100,E2P_TIMEOUT);
	 
	/*#ifdef TIMEOUT_ENABLE
	timeout_start(E2P_TIMEOUT);
	while((INT_CMPL != eeprom_tx_rx_flag) && (!is_timeout_expired()));
	timeout_stop();
	#else
	while(INT_CMPL != eeprom_tx_rx_flag);
	#endif*/
	
	eeprom_tx_rx_flag = INT_IDLE;	
	R_CSI20_Send_Receive(NULL,M95M01_MAX_PAGE_SIZE,rx_log);
	timeout_chk(&eeprom_tx_rx_flag,6,E2P_TIMEOUT);
	
	/*#ifdef TIMEOUT_ENABLE
	timeout_start(E2P_TIMEOUT);
	while((INT_CMPL != eeprom_tx_rx_flag) && (!is_timeout_expired()));
	timeout_stop();
	#else
	while(INT_CMPL != eeprom_tx_rx_flag);
	#endif*/	 
	h_vDEL_100nano(1);
	EEPROM_CS = 1;
	return EEPROM_SUCCESS;
}
void ReceiveEepromSpi(uint8_t *rx_log)
{
	//uint8_t dummy_tx_buff[MAX_EEPROM_BUFF_SIZE] = {0};
	
	EEPROM_CS = 0;
	h_vDEL_100nano(1);
	eeprom_tx_rx_flag = INT_IDLE; 
	R_CSI20_Send_Receive(NULL,256,rx_log);
	#ifdef TIMEOUT_ENABLE
	timeout_start(E2P_TIMEOUT);
	while((INT_CMPL != eeprom_tx_rx_flag) && (!is_timeout_expired()));
	timeout_stop();
	#else
	while(INT_CMPL != eeprom_tx_rx_flag);
	#endif
	eeprom_tx_rx_flag = INT_IDLE; 
	h_vDEL_100nano(1);
	EEPROM_CS = 1;
}
m95_ERROR_CODE SendEepromSpi(uint8_t *log,uint16_t log_len,uint32_t *address)
{
	uint8_t index = 0;
	uint8_t commandptr = WREN;
	uint8_t dummy_var;
	//uint8_t dummy_buff[300];
	uint8_t add_buff[4] = {0};
	
	if(log_len > MAX_EEPROM_BUFF_SIZE)
	{
		return LONG_WRITE_LEN;
	}
	EEPROM_CS = 1;
	h_vDEL_100nano(1);
	commandptr = RDSR;
	EEPROM_CS = 0;
	h_vDEL_100nano(1);
	eeprom_tx_rx_flag = INT_IDLE; 
	//R_CSI20_Send_Receive(&commandptr,1,&dummy_var);
	R_CSI20_Send_Receive(&commandptr,1,NULL);
	timeout_chk(&eeprom_tx_rx_flag,100,E2P_TIMEOUT);
	
	/*#ifdef TIMEOUT_ENABLE
	timeout_start(E2P_TIMEOUT);
	while((INT_CMPL != eeprom_tx_rx_flag) && (!is_timeout_expired()));
	timeout_stop();
	#else
	while(INT_CMPL != eeprom_tx_rx_flag);
	#endif
	eeprom_tx_rx_flag = INT_IDLE; */
	for(index = 0; index <15 ;index++)
	{		 
		R_CSI20_Send_Receive(&commandptr,1,&dummy_var);
		timeout_chk(&eeprom_tx_rx_flag,100,E2P_TIMEOUT);
		
		/*#ifdef TIMEOUT_ENABLE
		timeout_start(E2P_TIMEOUT);
		while((INT_CMPL != eeprom_tx_rx_flag) && (!is_timeout_expired()));
		timeout_stop();
		#else
		while(INT_CMPL != eeprom_tx_rx_flag);
		#endif
		eeprom_tx_rx_flag = INT_IDLE; */
		if(dummy_var & WIP_CHECK)
		{
			h_vDEL_Ms(1);
		}
		else
		{
			break;
		}
		
	}
	if(index >= 15)
	{
          //m_u8MSGQ_cmd_send(LCD_DBG_CMD,9,FP_QUEUE);
	  return WRITE_IN_PROGRESS;
	}
	EEPROM_CS = 1;
	h_vDEL_100nano(1);
	commandptr = WREN;
	EEPROM_CS = 0;
	h_vDEL_100nano(1);
	eeprom_tx_rx_flag = INT_IDLE; 
	R_CSI20_Send_Receive(&commandptr,1,&dummy_var);
	timeout_chk(&eeprom_tx_rx_flag,100,E2P_TIMEOUT);
	
	/*#ifdef TIMEOUT_ENABLE
	timeout_start(E2P_TIMEOUT);
	while((INT_CMPL != eeprom_tx_rx_flag) && (!is_timeout_expired()));
	timeout_stop();
	#else
	while(INT_CMPL != eeprom_tx_rx_flag);
	#endif
	eeprom_tx_rx_flag = INT_IDLE; */
	h_vDEL_100nano(1);
	EEPROM_CS = 1;	
	h_vDEL_100nano(1); 
	
	/***************************************/
	EEPROM_CS = 1;
	h_vDEL_100nano(1);
	commandptr = RDSR;
	EEPROM_CS = 0;
	h_vDEL_100nano(1);
	eeprom_tx_rx_flag = INT_IDLE; 
	R_CSI20_Send_Receive(&commandptr,1,&dummy_var);
	timeout_chk(&eeprom_tx_rx_flag,100,E2P_TIMEOUT);
	
	/*#ifdef TIMEOUT_ENABLE
	timeout_start(E2P_TIMEOUT);
	while((INT_CMPL != eeprom_tx_rx_flag) && (!is_timeout_expired()));
	timeout_stop();
	#else
	while(INT_CMPL != eeprom_tx_rx_flag);
	#endif
	eeprom_tx_rx_flag = INT_IDLE; */
	
	R_CSI20_Send_Receive(&commandptr,1,&dummy_var);
	timeout_chk(&eeprom_tx_rx_flag,100,E2P_TIMEOUT);
	
	/*#ifdef TIMEOUT_ENABLE
	timeout_start(E2P_TIMEOUT);
	while((INT_CMPL != eeprom_tx_rx_flag) && (!is_timeout_expired()));
	timeout_stop();
	#else
	while(INT_CMPL != eeprom_tx_rx_flag);
	#endif
	eeprom_tx_rx_flag = INT_IDLE; */
	EEPROM_CS = 1;	
	h_vDEL_100nano(1); 
	/***************************************/
	
	add_buff[0] = WRITE;
	add_buff[1] = (uint8_t)((*address)>>16);
	add_buff[2] = (uint8_t)((*address)>>8);
	add_buff[3] = (uint8_t)((*address));
	
	EEPROM_CS = 0;
	
	h_vDEL_100nano(1);	
	eeprom_tx_rx_flag = INT_IDLE; 
	R_CSI20_Send_Receive(add_buff,4,NULL);	
	timeout_chk(&eeprom_tx_rx_flag,100,E2P_TIMEOUT);
	
	/*#ifdef TIMEOUT_ENABLE
	timeout_start(E2P_TIMEOUT);
	while((INT_CMPL != eeprom_tx_rx_flag) && (!is_timeout_expired()));
	timeout_stop();
	#else
	while(INT_CMPL != eeprom_tx_rx_flag);
	#endif
	eeprom_tx_rx_flag = INT_IDLE; */
	
	h_vDEL_100nano(1);	
	eeprom_tx_rx_flag = INT_IDLE; 
	R_CSI20_Send_Receive(log,log_len,NULL);	
	timeout_chk(&eeprom_tx_rx_flag,7,E2P_TIMEOUT);
	
	/*#ifdef TIMEOUT_ENABLE
	timeout_start(E2P_TIMEOUT);
	while((INT_CMPL != eeprom_tx_rx_flag) && (!is_timeout_expired()));
	timeout_stop();
	#else
	while(INT_CMPL != eeprom_tx_rx_flag);
	#endif
	eeprom_tx_rx_flag = INT_IDLE; */
	h_vDEL_100nano(1);
	EEPROM_CS = 1;
	h_vDEL_Ms(5);
	return EEPROM_SUCCESS;
}




uint16_t h_u8EEPROM_page_get(void)
{
	return eeprom_page;	
}
void h_u8EEPROM_page_set(uint16_t page_val)
{
	eeprom_page = page_val;	
}
m95_ERROR_CODE h_u8EEPROM_write()
{
	return 0;
}

m95_ERROR_CODE h_u8EEPROM_read()
{
	return 0;
}
m95_ERROR_CODE h_u8EEPROM_init()
{
	return 0;
}
m95_ERROR_CODE h_u8EEPROM_erase()
{
	return 0;
}
m95_ERROR_CODE h_u8EEPROM_page_erase()
{
	return 0;
}
