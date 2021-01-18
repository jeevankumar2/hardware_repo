#include "H_led_driver.h"
#include "r_cg_timer.h"

#define LED_TH NO_OF_ROWS * 10

uint8_t u8_G_led_data_arr[NO_OF_ROWS]={0xff,0xff,0xff,0xff,0xff,0xff};
uint8_t u8_G_led_type_arr[NO_OF_ROWS]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

uint32_t G_u32pattern_var=1;
 
//uint8_t _u8LED_hdlr_ctl = ON,u8LED_Gcnt = 0;
//extern uint32_t tick1;
    
G_RESP_CODE h_vLED_shiftout(void);


G_RESP_CODE h_vLED_shiftout(void)                  
{
     int8_t column;
     uint8_t ret;
     LATCH_PIN = LOW;  
     for (column = NO_OF_COLUMNS-1; column >=0; column--)  
     {
	   #if 1
           DATA_PIN1 = (((~(u8_G_led_data_arr[0]^u8_G_led_type_arr[0])) & (1 << column))?HIGH:LOW);
	   DATA_PIN2 = (((~(u8_G_led_data_arr[1]^u8_G_led_type_arr[1])) & (1 << column))?HIGH:LOW);
	   DATA_PIN3 = (((~(u8_G_led_data_arr[2]^u8_G_led_type_arr[2])) & (1 << column))?HIGH:LOW);
	   CLOCK_PIN = HIGH;   
	   CLOCK_PIN = LOW;  
	   #else
	   test_led[0][t_cnt] = (((~(u8_G_led_data_arr[0]^u8_G_led_type_arr[0])) & (1 << column))?HIGH:LOW);
	   test_led[1][t_cnt] = (((~(u8_G_led_data_arr[1]^u8_G_led_type_arr[1])) & (1 << column))?HIGH:LOW);
	   test_led[2][t_cnt] = (((~(u8_G_led_data_arr[2]^u8_G_led_type_arr[2])) & (1 << column))?HIGH:LOW);
	   test_led[3][t_cnt] = (((~(u8_G_led_data_arr[3]^u8_G_led_type_arr[3])) & (1 << column))?HIGH:LOW);
	   test_led[4][t_cnt] = (((~(u8_G_led_data_arr[4]^u8_G_led_type_arr[4])) & (1 << column))?HIGH:LOW);
	   test_led[5][t_cnt] = (((~(u8_G_led_data_arr[5]^u8_G_led_type_arr[5])) & (1 << column))?HIGH:LOW);
	   t_cnt++;
	   #endif
	   
     }
     if(column == NO_OF_COLUMNS)
     {
	   ret = SUCCESS;     
     }
     LATCH_PIN = HIGH;
     return ret;
}

G_RESP_CODE m_u8LED_response_hdlr(uint8_t *buff_ptr)
{
	
	uint8_t index = 0;
	uint8_t ret;
	for(index = 0;index<NO_OF_ROWS;index++)
	{
		u8_G_led_data_arr[index] = *buff_ptr;
		buff_ptr++;
	}
	ret = h_vLED_shiftout();
	return ret;
}

void m_vled_pattern(void)
{
     int8_t column;
     uint8_t ret;
     G_u32pattern_var = G_u32pattern_var<<1;
     if(G_u32pattern_var>=0x800000)
     {
	    G_u32pattern_var = 1; 
     }
     LATCH_PIN = LOW; 
     for (column = NO_OF_COLUMNS-1; column >=0; column--)  
     {
	  DATA_PIN1 = (G_u32pattern_var>>(column)) & (uint32_t)0x01;
	  DATA_PIN2 = (G_u32pattern_var>>(column+8)) & (uint32_t)0x01;
	  DATA_PIN3 = (G_u32pattern_var>>(column+16)) & (uint32_t)0x01;
	  CLOCK_PIN = HIGH;   
	  CLOCK_PIN = LOW;   
     }
     if(column == NO_OF_COLUMNS)
     {
	   ret = SUCCESS;     
     }
     LATCH_PIN = HIGH;
}
