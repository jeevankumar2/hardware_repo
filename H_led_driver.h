#ifndef H_LED_DRIVER_H
#define H_LED_DRIVER_H

#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"

#define NO_OF_ROWS 6
#define NO_OF_COLUMNS 8

#define LATCH_PIN P7.5//P1.0 
#define CLOCK_PIN P7.6//P1.5
 
#define DATA_PIN1 P7.2//P5.0
#define DATA_PIN2 P7.3//P5.1
#define DATA_PIN3 P7.4//P5.2


#define HIGH 1
#define LOW 0


G_RESP_CODE m_u8LED_response_hdlr(uint8_t *buff_ptr);
void m_vled_pattern(void);



#endif
