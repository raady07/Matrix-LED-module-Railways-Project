#include <includes.h>




void    Init_SPI2(VOID)
{
	SPI2CON1bits.DISSCK = 0;  //Internal SPI clock is enabled
	SPI2CON1bits.DISSDO = 0; //SDOx pin is controlled by the module
	SPI2CON1bits.MODE16 = 0; //8 bit mode
	SPI2CON1bits.SMP = 0; // Input data sampled at middle of data output time
	SPI2CON1bits.CKE = 1; // active state(high) to Idle state(low) when CKP = 0
	SPI2CON1bits.SSEN = 1; //slave select bit
	SPI2CON1bits.CKP = 0; //Idle state for clock is a low level; active state is a high level
	SPI2CON1bits.MSTEN = 1; // Master mode
	SPI2CON1bits.PPRE = 2; //  for 1MHz CLK	
    SPI2CON1bits.SPRE = 5; //  for 1MHz CLK	
	SPI2CON2 = 0;
	
	SPI2STATbits.SPIROV = 0;
	SPI2STATbits.SPIEN = 1;
	
//	DDR_CS = 0;
}	


VOID  Write_SPI2(INT8U  data)
{
	 SPI2BUF = data;
	 while(SPI2STATbits.SPITBF);
}

INT8U Read_SPI2(VOID)
{
	while(SPI2STATbits.SPIROV!=1);
	INT8U data = SPI2BUF;
	return data;
}		

VOID Enable_SS(VOID)
{
//	EEPROM_CS = 0;
}	 
VOID Disable_SS(VOID)
{
//	EEPROM_CS = 1;
}	 
