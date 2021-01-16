 #include "r_cg_macrodriver.h"
 #include "M_event_handler.h"
 #include "r_cg_userdefine.h"


/*****************EVENT FLAG *******************/
uint16_t u16_GSM_EVENT_FLAG_GROUP = 0;
/*****************EVENT FLAG *******************/



G_CTL_CODES GSM_event_flag_manager(uint16_t flag_pos,uint8_t operation)
{
	uint8_t ret = 5;
	uint16_t chk_val = 0;
	switch(operation)
	{
	  	case FLAG_SET:
		{
			SETB(u16_GSM_EVENT_FLAG_GROUP,flag_pos);
			break;
		}
		case FLAG_CLR:
		{
			CLRB(u16_GSM_EVENT_FLAG_GROUP,flag_pos);
			break;
		}
		case FLAG_CHK:
		{
			chk_val = CHKB(u16_GSM_EVENT_FLAG_GROUP,flag_pos);
			if(chk_val)
			{
				ret = FLAG_SET;	
			}
			else
			{
				ret = FLAG_CLR;
			}
			break;
		}
		default:
		break;
	}
	return ret;
}
G_CTL_CODES EVE_manipulate(uint16_t flag_pos,uint8_t operation,uint16_t *Event_group)
{
	uint8_t ret = 5;
	uint16_t chk_val = 0;
	switch(operation)
	{
	  	case FLAG_SET:
		{
			SETB(*Event_group,flag_pos);
			break;
		}
		case FLAG_CLR:
		{
			CLRB(*Event_group,flag_pos);
			break;
		}
		case FLAG_CHK:
		{
			chk_val = CHKB(*Event_group,flag_pos);
			if(chk_val)
			{
				ret = FLAG_SET;	
			}
			else
			{
				ret = FLAG_CLR;
			}
			break;
		}
		default:
		break;
	}
	return ret;
}