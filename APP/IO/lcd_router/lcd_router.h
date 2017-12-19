#ifndef  LCD_ROUTER_H
#define  LCD_ROUTER_H

extern void XLCDInit(void);                                //to initialise the LCD
extern void XLCDPut(char data);                            //to put dtat to be displayed
extern void XLCDPutRamString( char *string);               //to display data string in RAM
//void XLCDPutRomString(const rom char *string);      //to display data stringin ROM
extern void XLCDCommand(unsigned char cmd);                //to send commands to LCD           
extern void XLCD_Clock(void);
extern void ClearBuff(char *ch);
extern void dtob(unsigned char i, unsigned char *bin,unsigned char len);
extern void LCD_Write(char *string, char line);


#endif // MODEM_ROUTER_H
