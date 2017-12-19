

#ifndef __LCD_H
#define __LCD_H
#include  <p24FJ256GA110.h>


typedef unsigned char BYTE;



#define 	LCD_D0_D TRISGbits.TRISG12
#define 	LCD_D1_D TRISGbits.TRISG13
#define 	LCD_D2_D TRISGbits.TRISG14
#define 	LCD_D3_D TRISGbits.TRISG15
#define 	LCD_RS_D TRISGbits.TRISG0
#define 	LCD_EN_D TRISGbits.TRISG1
//#define 	LCD_BL_D TRISGbits.TRISG9
#define 	LCD_BL_D TRISFbits.TRISF1
#define 	LCD_RW_D TRISGbits.TRISG8

#define 	LCD_D0_P PORTGbits.RG12
#define 	LCD_D1_P PORTGbits.RG13
#define 	LCD_D2_P PORTGbits.RG14
#define 	LCD_D3_P PORTGbits.RG15
#define 	LCD_RS_P PORTGbits.RG0
#define 	LCD_EN_P PORTGbits.RG1
#define 	LCD_BL_P PORTFbits.RF1
#define 	LCD_RW_P PORTGbits.RG8

#define		BUTTON1_D	TRISDbits.TRISD10
//#define		BUTTON2_D	TRISAbits.TRISA4
#define		BUTTON2_D	TRISEbits.TRISE9
#define		BUTTON1	!PORTDbits.RD10
//#define		BUTTON2	!PORTAbits.RA4
#define		BUTTON2	!PORTEbits.RE9

#define		K1		'1'
#define		K2		'2'
#define		K3		'8'
#define		K4		'9'

#define XLCDL1home()    	   			XLCDCommand(0x80)
#define XLCDL2home()     	   			XLCDCommand(0xC0)
#define XLCDL3home()    	   			XLCDCommand(0x94)
#define XLCDL4home()     	   			XLCDCommand(0xD4)

#define XLCDClear()     	   			XLCDCommand(0b00000001)		//0x01
#define XLCD_RETURNHOME()  				XLCDCommand(0b00000010)		//0x02
#define XLCD_CRIGHTSHIFT()  			XLCDCommand(0b00010100)		//0x14
#define XLCD_BACKSPACE()       			XLCDCommand(0b00010000)		//0x10
#define XLCD_BLINKOON()        			XLCDCommand(0b00001111)		//0x0F
#define XLCD_BLINKOFF()         		XLCDCommand(0b00001110)		//0x0E
#define XLCD_DRIGHTSHIFT()  			XLCDCommand(0b00011100)		//0x1C
#define XLCD_DLEFTSHIFT()     			XLCDCommand(0b00011000)		//0x18

void XLCDInit(void);                                //to initialise the LCD
void XLCDPut(char data);                            //to put dtat to be displayed
void XLCDPutRamString( char *string);               //to display data string in RAM
//void XLCDPutRomString(const rom char *string);      //to display data stringin ROM
void XLCDCommand(unsigned char cmd);                //to send commands to LCD           
void XLCD_Clock(void);
void dtob(unsigned char i, unsigned char *bin,unsigned char len);
void LCD_Write(char *string, char line);


void XLCDDelay15ms(void);
void XLCDDelay4ms(void);
void XLCDDelay(void);




#define	MoveRight(x)	{ unsigned char _dcnt = x; \
			              while(_d cnt-- != 0) \
				            XLCDCommand(0b00010100); }

#define	MoveLeft(x)	{ unsigned char _dcnt = x; \
			              while(_dcnt-- != 0) \
				            XLCDCommand(0b00010000); }

typedef union LCD_PORT
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

typedef union LCD_DATA
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



