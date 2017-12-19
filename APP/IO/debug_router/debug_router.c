#include <includes.h>


INT16U	PS 	= 	0 ;								// Peripheral Status 16 Bit Value
INT16U SNRS = 	0 ;								// Sensors Status 16 Bit Value
INT16U SW_V = 	0 ;								// PIC Software Version
INT16U CD 	= 	0 ;								// PIC Program Complie Date
INT16U POR 	= 	0 ;								// Power On Reset Count
INT16U BOR 	= 	0 ;								// Brown Out Reser Count
INT16U WDT 	= 	0 ;								// Watch Dog reset count
INT16U LVDT = 	0 ;								// Low Voltage Reset Count
INT16U Vcc	=	0 ;								// PIC Operating Voltage



extern INT8U	Task_WDT[5];					//	Definition in app_cfg.c

extern INT8U rdata;  
extern INT8U wdata; 
void Handle_Reset(void);
static void Disp_RTC(void); 

char i2cData[10];

void  AppDebugTask (void *p_arg)
{
	CPU_INT08S * rxMessage;
	CPU_INT08U *err;
	Debug_PutString("\n\rUART 2 Ready..");

	Handle_Reset();	
	while(1)
	{
    	Task_WDT[4] = 1;	    						//	DEBUG_TASK WATCHDOG STATUS BIT
		rxMessage = (CPU_INT08S*)OSMboxPend(UART2MboxPtr, 9000, err);
		//	if((*rxMessage) == COUNT_2000MS_TIMER)
		{
		}	
	}		
}	


void Handle_Reset(void)
{
	if(RCON > 0)
	{
		if(RCONbits.TRAPR)						// Trap Conflit Occured
		{
			
			RCONbits.TRAPR = 0;
			Debug_PutString("\n\rTrap Conflit Occured\n\r");	
		}
		if(RCONbits.IOPUWR)						// ILLEGAL OPCODE detected
		{			
			RCONbits.IOPUWR = 0;
			Debug_PutString("\n\rILLEGAL OPCODE detected\n\r");	
		}
		if(RCONbits.CM)							// Configuration Word Mismatch Occured
		{	
			RCONbits.CM = 0;
			Debug_PutString("\n\rConfiguration Word Mismatch Occured\n\r");	
		}
		if(RCONbits.EXTR)						// MCLR Reset Done
		{
			RCONbits.EXTR = 0;
			Debug_PutString("\n\rMCLR Reset Done\n\r");	
		}
		if(RCONbits.SWR)						// Software Reset Done
		{		
			RCONbits.SWR = 0;
			Debug_PutString("\n\rSoftware Reset Done\n\r");	
		}
		if(RCONbits.WDTO)						// Watch Dog Timmer Reset Occured
		{			
			RCONbits.WDTO = 0;
			Debug_PutString("\n\rWatch Dog Timmer Reset Occured\n\r");	
		}
		if(RCONbits.POR)						// Power On Reset Occured
		{			
			RCONbits.POR = 0;
			Debug_PutString("\n\rPower On Reset Occured\n\r");	
		}
		if(RCONbits.BOR)						// Brown Out Reset Occured
		{			
			RCONbits.BOR = 0;
			Debug_PutString("\n\rBrown Out Reset Occured\n\r");	
		}
	}
}

static void 	Disp_RTC(void)
{

	Debug_PutString("\n\n\r HH:MM:SS ");
	Debug_PutInt(Hour);
	Debug_PutChar(':'); 
	Debug_PutInt(Min);
	Debug_PutChar(':'); 
	Debug_PutInt(Sec);
	Debug_PutString("\t Sys_Time ");
	Debug_PutInt(Sys_Time.StructTime.Hrs); 
	Debug_PutChar(':'); 
	Debug_PutInt(Sys_Time.StructTime.Mins); 
	Debug_PutChar(':'); 
	Debug_PutInt(Sys_Time.StructTime.Secs); 		
	Debug_PutString("\n\r DD:MM:YY ");
	Debug_PutInt(Date);
	Debug_PutChar(':'); 
	Debug_PutInt(Month);
	Debug_PutChar(':'); 
	Debug_PutInt(Year);	
	Debug_PutString("\t\t DOW: ");
	Debug_PutInt(DOW);	
	Debug_PutChar('\n'); 	
	Debug_PutChar('\r'); 	
}
