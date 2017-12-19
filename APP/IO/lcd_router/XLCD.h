/*********************************************************************
 *
 *                  External LCD access routines defs
 *
 *********************************************************************
 * FileName:        XLCD.h
 * Dependencies:    compiler.h
 * Processor:       PIC18
 * Complier:        MCC18 v1.00.50 or higher
 *                  HITECH PICC-18 V8.10PL1 or higher
 * Company:         Microchip Technology, Inc.
 *
 * Author               Date    Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Naveen Raj     6/9/03  Original        (Rev 1.0)
 ********************************************************************/

#ifndef __XLCD_H
#define __XLCD_H
#include  <p24FJ128GA010.h>


typedef unsigned char BYTE;

//////////////////////////////////////////////////////

#define 	LCD_D0_D TRISGbits.TRISG12
#define 	LCD_D1_D TRISGbits.TRISG13
#define 	LCD_D2_D TRISGbits.TRISG14
#define 	LCD_D3_D TRISGbits.TRISG15
#define 	LCD_RS_D TRISGbits.TRISG0
#define 	LCD_EN_D TRISGbits.TRISG1
#define 	LCD_BL_D TRISGbits.TRISG9
#define 	LCD_RW_D TRISGbits.TRISG8

#define 	LCD_D0_P PORTGbits.RG12
#define 	LCD_D1_P PORTGbits.RG13
#define 	LCD_D2_P PORTGbits.RG14
#define 	LCD_D3_P PORTGbits.RG15
#define 	LCD_RS_P PORTGbits.RG0
#define 	LCD_EN_P PORTGbits.RG1
#define 	LCD_BL_P PORTGbits.RG9
#define 	LCD_RW_P PORTGbits.RG8

/////////////////////////////////////////


void XLCDInit(void);                                //to initialise the LCD
void XLCDPut(char data);                            //to put dtat to be displayed
void XLCDPutRamString( char *string);               //to display data string in RAM
//void XLCDPutRomString(const rom char *string);      //to display data stringin ROM
void XLCDCommand(unsigned char cmd);                //to send commands to LCD           
void XLCD_Clock(void);
void ClearBuff(char *ch);
void dtob(unsigned char i, unsigned char *bin,unsigned char len);
void LCD_Write(char *string, char line);

#define XLCDL1home()    	   			XLCDCommand(0x80)
#define XLCDL2home()     	   		XLCDCommand(0xC0)
#define XLCDClear()     	   			XLCDCommand(0b00000001)		//0x01
#define XLCDReturnHome() 	   		XLCDCommand(0b00000010)		//0x02
#define XLCD_CRIGHTSHIFT()  	XLCDCommand(0b00010100)		//0x14
#define XLCD_BACKSPACE()       	XLCDCommand(0b00010000)		//0x10
#define XLCD_BLINKOON()        	XLCDCommand(0b00001111)		//0x0F
#define XLCD_BLINKOFF()         	XLCDCommand(0b00001110)		//0x0E
#define XLCD_RETURNHOME()  	XLCDCommand(0b00000010)		//0x02
#define XLCD_DRIGHTSHIFT()  	XLCDCommand(0b00011100)		//0x1C
#define XLCD_DLEFTSHIFT()     	XLCDCommand(0b00011000)		//0x18

void XLCDDelay15ms(void);
void XLCDDelay4ms(void);
void XLCDDelay100us(void);
void XLCD_Delay500ns(void);
void XLCDDelay(void);
void XLCDDelay1s (void);
void XLCDDelay100ms (void);
void XLCDDelay500ms (void);
#define	MoveRight(x)	{ unsigned char _dcnt = x; \
			              while(_d cnt-- != 0) \
				            XLCDCommand(0b00010100); }

#define	MoveLeft(x)	{ unsigned char _dcnt = x; \
			              while(_dcnt-- != 0) \
				            XLCDCommand(0b00010000); }

typedef union 
{
	BYTE PORT;
	struct
	{
		unsigned D0:1;
		unsigned D1:1;
		unsigned D2:1;
		unsigned D3:1;
		unsigned Dummy:12;
	};
}LCD_PORT;

typedef union 
{
	BYTE DATA;
	struct
	{
		unsigned LN:4;		//UPPER NIBBLE
		unsigned UN:4;		//LOWER NIBBLE
	};
}LCD_DATA;


void TOPORT(BYTE DT);
#endif



