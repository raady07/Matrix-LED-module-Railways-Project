#include <includes.h>
extern void EEPROM_write(INT8U Address, INT8U *Data, INT8U Len);				//	Definition eeprom_router.c
extern void EEPROM_read(INT8U Address, INT8U *Data, INT8U Len);
void EEPROM_write(INT8U Address, INT8U *Data, INT8U Len)
{
	INT8U i = 0;
	OSTimeDly(2);
	IdleI2C1();
	StartI2C1();
	IdleI2C1();
	
	MasterWriteI2C1(EEPROM_WRITE);                      // Write Control byte
	IdleI2C1();
	if(I2C1STATbits.ACKSTAT)
		Debug_PutString("\n\r No Ack after Control byte");
	MasterWriteI2C1(0);                       // write MSB Address 
	IdleI2C1();
	if(I2C1STATbits.ACKSTAT)
		Debug_PutString("\n\r No Ack after MSB Address");
	MasterWriteI2C1(Address);                      // write LSB Address
	if(I2C1STATbits.ACKSTAT)
		Debug_PutString("\n\r No Ack after LSB Address");
	IdleI2C1();
	while(i < Len)
        {
#ifdef EPPROM_Debug	        
	       Debug_PutString("\n\r Data Writing : ");
	       Debug_PutChar(*Data);
	       Debug_PutString("\n\r"); 
#endif
           MasterWriteI2C1(*Data);
           IdleI2C1();
       	   if(I2C1STATbits.ACKSTAT)
		   		Debug_PutString("\n\r No Ack write data");
		   Data++;
		   i++;
		   //IdleI2C1();	
         }                // write string 
	IdleI2C1();
	StopI2C1();
    IdleI2C1();
 
}                   // End write
void EEPROM_read(INT8U Address, INT8U *Data, INT8U Len)
{
  	unsigned char i=0;
 	OSTimeDly(2); 	
	IdleI2C1();
	StartI2C1();
	IdleI2C1();
	MasterWriteI2C1(EEPROM_WRITE);                      // Write Control byte
	IdleI2C1();
	if(I2C1STATbits.ACKSTAT)
		Debug_PutString("\n\r No Ack after Control byte");
	MasterWriteI2C1(0);                       // write MSB Address 
	IdleI2C1();
	if(I2C1STATbits.ACKSTAT)
		Debug_PutString("\n\r No Ack after MSB Address");
	MasterWriteI2C1(Address);                      // write LSB Address
	IdleI2C1();

	if(I2C1STATbits.ACKSTAT)
		Debug_PutString("\n\r No Ack after LSB Address");
	RestartI2C1();
	IdleI2C1();
	MasterWriteI2C1(EEPROM_READ);                      // Read 
	IdleI2C1();
	if(I2C1STATbits.ACKSTAT)
		Debug_PutString("\n\r No Ack after Reading Con byte ");
    while(i<(Len-1))
    {        
   		Data[i++]=MasterReadI2C1();
    	IdleI2C1();
    	Nop();
    	if(I2C1STATbits.ACKSTAT)
			Debug_PutString("\n\r No Ack after Reading data ");
    	AckI2C1();
		IdleI2C1();
    }
    Data[i]=MasterReadI2C1();
    IdleI2C1();
    NotAckI2C1();
	IdleI2C1();
	StopI2C1();
    IdleI2C1();
 //	Data[i]='\0';	
}                // End read

