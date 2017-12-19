#include <includes.h>

unsigned char  Sec = 0, Min = 0, Hour = 0, DOW = 0, Date = 0, Month = 0, Year = 0;

UnionTimer Sys_Time;


void Read_RTC(void)
{
	Nop();
	Nop();
	StartI2C1();
	IdleI2C1();
	MasterWriteI2C1(RTC_ID);       					// Write Control byte
	IdleI2C1();
	if(I2C1STATbits.ACKSTAT)
		Debug_PutString("\n\r No Read Ack after Control byte");
	MasterWriteI2C1(0x00);							// write LSB Address
   	IdleI2C1();
	if(I2C1STATbits.ACKSTAT)
		Debug_PutString("\n\r No Read Ack after LSB Address");
	RestartI2C1();
	IdleI2C1();
	MasterWriteI2C1(RTC_ID | 1);       				// Read Control Byte
	IdleI2C1();
	if(I2C1STATbits.ACKSTAT)
		Debug_PutString("\n\r No Read Ack after READ con");
    Sec		=	BCD2Bin( MasterReadI2C1() );		//	Read Seconds with  BCD format			
	AckI2C1();										//	Wait for Acknowledgment
	IdleI2C1();	
	Min		=	BCD2Bin( MasterReadI2C1() );		//	Read Minutes with  BCD format	
	IdleI2C1();
	AckI2C1();										//	Wait for Acknowledgment
	IdleI2C1();
	Hour	=	BCD2Bin( MasterReadI2C1() );		//	Read Hour with  BCD format	
	IdleI2C1();
	AckI2C1();										//	Wait for Acknowledgment
	IdleI2C1();
    DOW 	=	BCD2Bin( MasterReadI2C1() );		//	Read Day of the week with  BCD format	
	IdleI2C1();
	AckI2C1();										//	Wait for Acknowledgment
	IdleI2C1();
	Date	=	BCD2Bin( MasterReadI2C1() );		//	Read Date with  BCD format	
	IdleI2C1();
	AckI2C1();										//	Wait for Acknowledgment
	IdleI2C1();
	Month	=	BCD2Bin( MasterReadI2C1() );		//	Read Month with  BCD format	
	IdleI2C1();										
	AckI2C1();										//	Wait for Acknowledgment
	IdleI2C1();
	Year	=	BCD2Bin( MasterReadI2C1() );		//	Read Year(YY) with  BCD format	
	IdleI2C1();
	NotAckI2C1();
	IdleI2C1();
	StopI2C1();
	IdleI2C1();
}

void Compute_swrtc(void)
{
	if( Sys_Time.StructTime.Secs	>=	59 )				
    {
        Sys_Time.StructTime.Secs = 0;
        if( Sys_Time.StructTime.Mins	>=	59 )				
        {
            Sys_Time.StructTime.Mins = 0;
            if( Sys_Time.StructTime.Hrs	>= 23 )					
            {
                Sys_Time.StructTime.Hrs = 0; 
            }
            else 
            	Sys_Time.StructTime.Hrs++;
        }
        else 
			Sys_Time.StructTime.Mins++;
    }
    else 
    	Sys_Time.StructTime.Secs++;
}	

void W_RTC_Time(INT8U hrs, INT8U mins, INT8U secs)
{
	StartI2C1();
	IdleI2C1();
	MasterWriteI2C1(RTC_ID);                      			// Write Control byte along with Device ID
	IdleI2C1();
    while(I2C1STATbits.ACKSTAT);
	MasterWriteI2C1( 0x00 );								// write LSB Address	
    IdleI2C1();                      
	MasterWriteI2C1( D2BCD(secs) );							// write sec Value in D2B format with CH bit=0 at addrs loc 0x00 
	IdleI2C1();
    while(I2C1STATbits.ACKSTAT);
	MasterWriteI2C1( D2BCD(mins));							// write min Value in D2B format at addrs loc 0x01   
	IdleI2C1();
    while(I2C1STATbits.ACKSTAT);
	MasterWriteI2C1(D2BCD(hrs));            				// write hour Value in D2B format at addrs loc 0x02 (Need to consider 12/24 Hr mode)
	IdleI2C1();
	StopI2C1();
    IdleI2C1();
} 		

void W_RTC_Date(INT8U dd, INT8U mm, INT8U yy, INT8U dow)
{
	StartI2C1();
	IdleI2C1();
	MasterWriteI2C1(RTC_ID);                      			// Write Control byte along with Device ID
	IdleI2C1();
    while(I2C1STATbits.ACKSTAT);
	MasterWriteI2C1( 0x03 );								// write LSB Address w.r.t DOW	
    IdleI2C1();                      
	MasterWriteI2C1( D2BCD(dow) );							// write day 0f week Value in D2B format at addrs loc=03
	IdleI2C1();
    while(I2C1STATbits.ACKSTAT);
	MasterWriteI2C1( D2BCD(dd));							// write Date Value in D2B format at addrs loc=04  
	IdleI2C1();
    while(I2C1STATbits.ACKSTAT);
	MasterWriteI2C1(D2BCD(mm));            					// write Month Value in D2B format at addrs loc=05
	IdleI2C1();
	 while(I2C1STATbits.ACKSTAT);
	MasterWriteI2C1(D2BCD(yy));            					// write Year Value in D2B format at addrs loc=06
	IdleI2C1();
	StopI2C1();
    IdleI2C1();
}
				