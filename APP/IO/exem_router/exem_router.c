#include <includes.h>

extern INT8U	Task_WDT[5];
		


INT8U  ExEM_Reg_Addr_Cnt = 0;

void AppMCommTask (void *p_arg)
{
	INT8S * rxMessage;
	INT8U *err; 
	
   	while(1)
    {
		Task_WDT[3] = 1;
		rxMessage = (INT8S*)OSMboxPend(UART3MboxPtr, 9000, err);
	}
}
