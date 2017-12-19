#include <includes.h>


/*
*********************************************************************************************************
*                                       CLEAR WATCH DOG TIMER
*	Counter				:	count7500ms
*	WATCHDOG 			: 	INTERNAL	
*	TIME OUT PERIOD		:   8 Seconds 
*	No of TASKS			: 	5
*	CLEAR WDT			:	When All Tasks are operating normally 
*						:	Check Tasks for every 7.5 Seconds.	
*	DESCRIPTION			:	Monitors every Task for Every 7.5 Seconds.	
*	Definition Files	:	
*********************************************************************************************************
*/
INT8U	Task_WDT[5]	=	{0};						//	Buffer to monitor Individual Tasks					
INT8U SW_Reset_Tmr = 0;

void	SendMsgToRouter(void)
{
//	static INT16U count30ms 	= 	0;
//	static INT16U count100ms 	= 	0;
	static INT16U count250ms 	= 	0;
//	static INT16U count300ms 	= 	0;
	static INT16U count1500ms 	= 	0;
    static INT16U count1000ms 	= 	0;
    static INT16U count2000ms 	= 	0;
    static INT16U count3000ms 	= 	0;
    static INT16U count13000ms	=	0;
   
	
   	static INT16U periodicTimerMessage;
   	static void * messagePtr;
   	
   	
	if(++count250ms == COUNT_250MS_TIMER)
	{
		count250ms = 0;
		periodicTimerMessage = COUNT_250MS_TIMER;
	    messagePtr	 = (void *) &periodicTimerMessage;
//		OSMboxPost(LCDMboxPtr, messagePtr);												//	LCD TASK
	}
	
	if(++count1000ms == COUNT_1000MS_TIMER)
	{
		Compute_swrtc();
		
	/*	if(SW_Reset_Tmr++ >= SWRESET_TIME * 60)											//	RESET w.r.t time RAVI	
		{
			__asm__ volatile ("reset") ;
		}
	*/	
		MCLR_LED ^= 1;
		periodicTimerMessage = COUNT_1000MS_TIMER;
	    messagePtr	 = (void *) &periodicTimerMessage;
		count1000ms = 0;		
		OSMboxPost(UART1MboxPtr, messagePtr);											//	Modem_Comm 1 Sec
	}
	
	if(++count1500ms == COUNT_1500MS_TIMER)
	{
		count1500ms = 0;
		periodicTimerMessage = COUNT_1500MS_TIMER;
	    messagePtr	 = (void *) &periodicTimerMessage;
//		OSMboxPost(UART4MboxPtr, messagePtr);											//	Dimmer_Comm 1.5 Secs
	}	
		
	if(++count2000ms == COUNT_2000MS_TIMER)
	{
		periodicTimerMessage = COUNT_2000MS_TIMER;
	    messagePtr	 = (void *) &periodicTimerMessage;
		count2000ms = 0;			
		OSMboxPost(UART2MboxPtr, messagePtr);											//	Debug_UART 2 Secs
	}	
	if(++count3000ms == COUNT_3000MS_TIMER)
	{
		periodicTimerMessage = COUNT_3000MS_TIMER;
	    messagePtr	 = (void *) &periodicTimerMessage;
		count3000ms = 0;
//		OSMboxPost(UART3MboxPtr, messagePtr);											//	Ex_EM Comm 3Secs						
	}		
	if(++count13000ms == COUNT_13000MS_TIMER)											// Clear WDT if all Tasks are doing good
	{
		periodicTimerMessage = COUNT_13000MS_TIMER;				//	Check whether to remove or not with Chaitu
	  	messagePtr	 = (void *) &periodicTimerMessage;			//	Check whether to remove or not with Chaitu
	    count13000ms = 0;
	    if( Task_WDT[0] && Task_WDT[1] && Task_WDT[2] && Task_WDT[3] && Task_WDT[4] )
	    {
	    	Task_WDT[0] = 0;				//	LCD_TASK
	    	Task_WDT[1] = 0;				//	MODEM
	    	Task_WDT[2] = 0;				//	DIMMER Communication
	    	Task_WDT[3] = 0;				//	ENERGY METER
	    	Task_WDT[4] = 0;	    		//	DEBUGGER
	    	ClrWdt();
	    }	
	    else
	    {
		    Debug_PutString("\n\r TASK: ");
			Debug_PutChar(Task_WDT[0] + 48 );
			Debug_PutChar(Task_WDT[1] + 48 );
			Debug_PutChar(Task_WDT[2] + 48 );
			Debug_PutChar(Task_WDT[3] + 48 );
			Debug_PutChar(Task_WDT[4] + 48 );
			Debug_PutString("\n");
		}														
	}	
}


void Remap_UART()
{

	PPSUnLock;															//	Unlock Registers
	PPSInput(PPS_U1RX,PPS_RP30);
	PPSOutput(PPS_RP16,PPS_U1TX);

	//PPSInput(PPS_U1RX,PPS_RPI32);
	//PPSOutput(PPS_RP31,PPS_U1TX);
	
	//PPSInput(PPS_U2RX,PPS_RP10);
	//PPSOutput(PPS_RP17,PPS_U2TX);

	//PPSInput(PPS_U3RX,PPS_RP30);
	//PPSOutput(PPS_RP16,PPS_U3TX);

	//PPSInput(PPS_U4RX,PPS_RPI44);
	//PPSOutput(PPS_RP15,PPS_U4TX);	
	PPSLock;	
}



