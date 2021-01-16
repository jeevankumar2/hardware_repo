#include "H_DS1307_rtc_driver.h"
#include "r_cg_serial.h"
#include "r_cg_timer.h"
#include "M_time_out.h"


#define DS1307_I2C_ADDRESS 0xD0

#define HOLD_TIME_AFTER_START 200   //This value specifies the delay to occur after a START condition in i2c

#define RTC_TIMEOUT_VAL 1//in 100ms units

extern uint8_t h_u8RTC_tx_flag,h_u8RTC_rx_flag; 
extern volatile uint32_t rtc_tick_micro;







void delay_1us(unsigned long val);
uint8_t decToBcd(uint8_t val);
uint8_t bcdToDec(uint8_t val);

uint8_t h_u8RTC_init(void)
{
return 0;	
}


void h_u8RTC_time_set(RTC_DATE_TIME rtc_cur_dat)  // Function to set the currnt time, change the second&minute&hour to the right time
{  
  uint8_t u8_rtc_buff[10] = {0};   
  u8_rtc_buff[0] = decToBcd(0);
  u8_rtc_buff[1] = decToBcd(rtc_cur_dat.u8Second);
  u8_rtc_buff[2] = decToBcd(rtc_cur_dat.u8Minute);
  u8_rtc_buff[3] = decToBcd(rtc_cur_dat.u8Hour);
  //rtc_buff[3] = 114;
  u8_rtc_buff[4] = decToBcd(rtc_cur_dat.u8DayOfWeek);
  u8_rtc_buff[5] = decToBcd(rtc_cur_dat.u8Day);
  u8_rtc_buff[6] = decToBcd(rtc_cur_dat.u8Month);
  u8_rtc_buff[7] = decToBcd(rtc_cur_dat.u8Year);   
  delay_1us(5);
  h_u8RTC_tx_flag = INT_IDLE;
  R_IIC21_Master_Send(DS1307_I2C_ADDRESS,u8_rtc_buff,8);
  timeout_chk(&h_u8RTC_tx_flag,3,RTC_TIMEOUT_VAL);
  
  /*timeout_start(RTC_TIMEOUT_VAL);
  while((!h_u8RTC_tx_flag)&&(!is_timeout_expired()));
  if(EXPIRED == is_timeout_expired())
  {
	m_u8MSGQ_cmd_send(LCD_DBG_CMD,3,FP_QUEUE);
  } 
  timeout_stop();
  h_u8RTC_tx_flag = INT_IDLE;*/
} 
uint8_t h_u8RTC_time_get(RTC_DATE_TIME *rtc_cur_dat)  // Function to gets the date and time from the ds1307 and prints result
{  
  uint8_t ret = SUCCESS;
  uint8_t rx_buff[10]= {0};  
  rx_buff[0] = decToBcd(0);  
   
  delay_1us(5);// 4.7us delay is needed between a START and STOP condition
  
  //To reset register pointer to 0x00h in ds1307
  h_u8RTC_tx_flag = INT_IDLE;
  R_IIC21_Master_Send(DS1307_I2C_ADDRESS,rx_buff,1);
  ret = timeout_chk(&h_u8RTC_tx_flag,4,RTC_TIMEOUT_VAL);
  if(ret == EXPIRED)
  {
	ret = FAILURE;  
  }
  else if(ret == NOT_EXPIRED)
  {
	  delay_1us(5);// 4.7us delay is needed between a START and STOP condition
	 //To read the time keeping registers in ds1307
	  h_u8RTC_rx_flag = INT_IDLE;
	  R_IIC21_Master_Receive(DS1307_I2C_ADDRESS,rx_buff,7);
	  ret = timeout_chk(&h_u8RTC_rx_flag,5,RTC_TIMEOUT_VAL);
	  if(ret == EXPIRED)
	  {
		ret = FAILURE;  
	  }
	  else if(ret == NOT_EXPIRED)
	  {
		ret = SUCCESS;
		rtc_cur_dat->u8Second = bcdToDec(rx_buff[0] & 0x7f);
  		rtc_cur_dat->u8Minute = bcdToDec(rx_buff[1]);
  		rtc_cur_dat->u8Hour = bcdToDec(rx_buff[2] & 0x3f); //if 24 hour am/pm
  		//rtc_cur_dat.u8Hour = bcdToDec(rx_buff[2] & 0x1f); // if 12 hour am/pm
  		rtc_cur_dat->u8DayOfWeek = bcdToDec(rx_buff[3]);
  		rtc_cur_dat->u8Day = bcdToDec(rx_buff[4]);
  		rtc_cur_dat->u8Month = bcdToDec(rx_buff[5]);
  		rtc_cur_dat->u8Year = bcdToDec(rx_buff[6]);
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
uint8_t decToBcd(uint8_t val)
{
  return ( (val/10*16) + (val%10) );
} 
uint8_t bcdToDec(uint8_t val) // Convert binary coded decimal to normal decimal numbers
{
  return ( (val/16*10) + (val%16) );
}

void delay_1us(unsigned long val)
{
	 R_TAU0_Channel4_Start();
	 while(rtc_tick_micro<val);
	 rtc_tick_micro = 0;
	 R_TAU0_Channel4_Stop();
}