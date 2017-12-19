#ifndef  RTC_H
#define  RTC_H

#define RTC_ID	0xD0
#define SECONDS_IN_MINUTES 59
#define MINUTES_IN_HOURS 59
#define HOURS_IN_DAY 23


//#define SW_RTC_SYNC_TIME_VAL 60 // for 3 mins
#define SW_RTC_SYNC_TIME_VAL 1200 // for 1 Hr		To test the efficiency of SW timer

extern INT8U    BCD2Bin(INT8U data);
extern volatile INT8U 	RTC_SET;
extern unsigned char  Sec,Min,Hour,DOW,Date,Month,Year;

/*
|---------------------------------------|
|	Reserved | Hrs 	|	Mins  |	Secs	|
|	MSB		 |		|		  | LSB		|
|---------------------------------------|
*/

typedef union _UnionTimerTag
{
	INT32U TimeValue;
	struct
	{		
		INT8U Secs;
		INT8U Mins;
		INT8U Hrs;
		INT8U Reserved;
	}StructTime;
}UnionTimer;


void Compute_swrtc(void);
extern UnionTimer Sys_Time;


VOID Write_RTC(VOID);
VOID Read_RTC(VOID);


void W_RTC_Time(INT8U hrs, INT8U mins, INT8U secs);
void W_RTC_Date(INT8U dd, INT8U mm, INT8U yy, INT8U dow);
#endif //RTC_ROUTER_H
