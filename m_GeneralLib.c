#include "M_GeneralLib.h"


uint8_t u8GL_ncopy(uint8_t *str,const uint8_t *dat,uint8_t len)
{
    uint8_t i = 0;
    for(i=  0;i < len;i++)
    {
        if(dat[i]!='\0')
        {
            *str = dat[i];
            str++;
        }
        else
        {
            break;
        }
          
    }
     return i;
}

void u8GL_strclear(uint8_t *buff,uint16_t buff_len)
{
	uint16_t i =0;
	for(i = 0;i<buff_len;i++)
	{
		*(buff + i) = '\0';
	}	
}
void u8GL_num2str(uint16_t num,char *strin)
{
    uint8_t len =0,i=0;
    uint8_t digit[MAX_DIGITS]={0};
    for(len = 0;len< MAX_DIGITS;len++)
    {
        digit[len] = (uint8_t)(num%10);
        num = num / 10;
        if(num == 0)
        {
            break;
        }
    }
    for(i = 0;i<=len;i++)
    {
        strin[i] = digit[len-i]+ZERO_ASCII_VAL;
    }
    strin[i] = '\0';
}