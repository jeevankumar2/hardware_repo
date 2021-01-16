#ifndef H_M95M01_EEPROM_DRIVER_H
#define H_M95M01_EEPROM_DRIVER_H

#include "r_cg_macrodriver.h"



/**********EEPROM INSTRUCTION SET*******************/
#define WREN (0x06U) //write enable register
#define WRDI (0x04U) //write disable register
#define RDSR (0x05U) //read status register
#define WRSR (0x01U) //write status register
#define READ (0x03U) //read from memory
#define WRITE (0x02U) //write to memory
/**********EEPROM INSTRUCTION SET*******************/

#define WIP_CHECK (0x01U)

#define M95M01_MAX_PAGE_SIZE 256
#define CRITICAL_DATA_PAGE 0
#define E2P_TEST_PAGE 1//SDBG
#define LOG_PAGE_START 2
#define LOG_PAGE_END 511
#define LOG 0
#define DAT 2
#define E2P_TEST 1//SDBG

#define MAX_EEPROM_BUFF_SIZE M95M01_MAX_PAGE_SIZE 
 
#define EEPROM_CS P4.1

typedef enum
{
	EEPROM_SUCCESS = 0,
	LONG_WRITE_LEN = 1,
	PG_NO_OUT_OF_BOUND = 2,
	WRITE_IN_PROGRESS = 3,	
}m95_ERROR_CODE;



m95_ERROR_CODE h_u8EEPROM_page_write(uint8_t *log,uint16_t log_len,uint8_t type); 
m95_ERROR_CODE h_u8EEPROM_page_read(uint8_t *rx_log,uint16_t PageNo);
uint16_t h_u8EEPROM_page_get(void);
void h_u8EEPROM_page_set(uint16_t page_val);
#endif