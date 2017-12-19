
#include "lcd.h"
#include <string.h>
char _vXLCDreg =0;          //Used as a flag to check if from XLCDInit()

LCD_PORT LP;
LCD_DATA LD;
void XLCDInit(void)
{

	SPI2STATbits.SPIEN = 0;
	
	LCD_D0_D = 0;
	LCD_D1_D = 0;
	LCD_D2_D = 0;
	LCD_D3_D = 0;
	LCD_RS_D = 0;
	LCD_EN_D = 0;
	LCD_BL_D = 0;
	LCD_RW_D = 0;
	
	LCD_D0_P = 0;
	LCD_D1_P = 0;
	LCD_D2_P = 0;
	LCD_D3_P = 0;
	LCD_RS_P = 0;
	LCD_EN_P = 0;
	LCD_BL_P = 0;
	LCD_RW_P =0;

	XLCDDelay15ms(); 

	TOPORT(0b00000011);
	TOPORT(0b00000011);
	TOPORT(0b00000011);
	TOPORT(0b00000010);

	XLCDCommand(0b00101000);    	//Function Set :- Data lines:4;5X8 Dots
   	XLCDCommand(0b00001000);        //display off  :- Display Off;Cursor Off;
   	XLCDClear();
   	XLCDCommand(0b00000110);    	//if cursor inc and no display shift
	XLCDCommand(0b00001111);    	//display on cursor on blink on	        
	XLCDClear();
}


void XLCDCommand(unsigned char cmd)
{	
	XLCDDelay4ms();

	LCD_EN_P = 0;
	LCD_RS_P = 0;
	
	LD.DATA = cmd;
	TOPORT(LD.UN);
	TOPORT(LD.LN);
}

void XLCDPut(char data)
{

	XLCDDelay4ms();

	LCD_EN_P = 0;
	LCD_RS_P = 1;
	
	LD.DATA = data;
	
	TOPORT(LD.UN);
	TOPORT(LD.LN);

}

void TOPORT(BYTE DT)
{
	LP.PORT = DT;
	LCD_D0_P = LP.D0;	
	LCD_D1_P = LP.D1;	
	LCD_D2_P = LP.D2;	
	LCD_D3_P = LP.D3;	
	XLCD_Clock();
}

void XLCDPutRamString(char *string)
{
    while(*string)                          // Write data to LCD up to null
    {   
        XLCDPut(*string);                   // Write character to LCD
        string++;                           // Increment buffer
    }
    return;
}
/*
void XLCDPutRomString(const rom char *string)
{
    while(*string)                         // Write data to LCD up to null
    {    
        XLCDPut(*string);                   // Write character to LCD
        string++;                           // Increment buffer
    }
    return;
}
*/
void XLCDDelay15ms (void)
{
   	char i;
   	for(i=0;i<4;i++)
   	{
   		XLCDDelay4ms();
   	}
   	return;
}


void XLCDDelay4ms (void)
{
    int i;
  	for(i=0;i<3000;i++)			//	10000 replaced by Ravi
   	{
   		Nop();
   	}
   	return;
}


void XLCD_Clock(void)
{
	LCD_EN_P = 1; 
 //   Nop();
    Nop();
    Nop();
    LCD_EN_P = 0; 	
}

void dtob(unsigned char i, unsigned char *bin,unsigned char len)
{
	unsigned char j;
	for( j = len-1; j >= 0; j-- )
	{
		if (i == 0)
			bin[j] = 48;
		else
			bin[j] = (i % 2) + 48;
		i = i / 2;
	
	}
}

void LCD_Write(char *string, char line)
{
	if (line == 1)
	{
		XLCDL1home();
		XLCDPutRamString(string);
	}
	else if (line == 2)
	{
		XLCDL2home();
		XLCDPutRamString(string);
	}
}
