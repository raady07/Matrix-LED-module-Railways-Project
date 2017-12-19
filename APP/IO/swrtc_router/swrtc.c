
#include <includes.h>

	

__SWRTCTime SWRTCTime;
#define SECONDS_IN_MINUTES 59
#define MINUTES_IN_HOURS 59
#define HOURS_IN_DAY 23
void AppSWRTCTask (void *p_arg)
{
	CPU_INT08S * rxMessage;
	CPU_INT08U *err;    
    SWRTCTime.seconds = 0;
    SWRTCTime.minutes = 0;
    SWRTCTime.hours = 0;
   while(1)
   {
		rxMessage = (CPU_INT08S*)OSMboxPend(SWRTCMboxPtr, 9000, err);
		if((*rxMessage) == COUNT_1000MS_TIMER)
		{
			
		}	  
   } 
	
}

void Compute_swrtc(void)
{
	if(++SWRTCTime.seconds == SECONDS_IN_MINUTES)
	{
		SWRTCTime.seconds = 0;
		SWRTCTime.minutes++;
	}
	if((SWRTCTime.seconds == 0) && (SWRTCTime.minutes == MINUTES_IN_HOURS))
	{
		SWRTCTime.minutes = 0;
		SWRTCTime.hours++;
	}
	if((SWRTCTime.seconds == 0)&& (SWRTCTime.minutes == 0) && (SWRTCTime.hours == HOURS_IN_DAY))
	{
		SWRTCTime.hours = 0;
	}
}	