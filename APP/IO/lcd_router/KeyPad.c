#include <p24fj128ga010.h>
#include <XLCD.h>
#include <stdio.h>

#define		KP_D_C1	TRISEbits.TRISE4
#define		KP_D_C2	TRISEbits.TRISE5
#define		KP_D_C3	TRISEbits.TRISE8
#define		KP_D_C4	TRISEbits.TRISE9
#define		KP_D_R1	TRISEbits.TRISE0
#define		KP_D_R2	TRISEbits.TRISE1
#define		KP_D_R3	TRISEbits.TRISE2
#define		KP_D_R4	TRISEbits.TRISE3

#define		KP_P_C1		PORTEbits.RE4
#define		KP_P_C2	PORTEbits.RE5
#define		KP_P_C3	PORTEbits.RE8
#define		KP_P_C4	PORTEbits.RE9
#define		KP_P_R1	PORTEbits.RE0
#define		KP_P_R2	PORTEbits.RE1
#define		KP_P_R3	PORTEbits.RE2
#define		KP_P_R4	PORTEbits.RE3

/*
#define		KP_D_C1	TRISAbits.TRISA0
#define		KP_D_C2	TRISAbits.TRISA1
#define		KP_D_C3	TRISAbits.TRISA2
#define		KP_D_C4	TRISAbits.TRISA3
#define		KP_D_R1	TRISAbits.TRISA4
#define		KP_D_R2	TRISAbits.TRISA5
#define		KP_D_R3	TRISAbits.TRISA6
#define		KP_D_R4	TRISAbits.TRISA7

#define		KP_P_C1		PORTAbits.RA0
#define		KP_P_C2	PORTAbits.RA1
#define		KP_P_C3	PORTAbits.RA2
#define		KP_P_C4	PORTAbits.RA3		
#define		KP_P_R1	PORTAbits.RA4
#define		KP_P_R2	PORTAbits.RA5
#define		KP_P_R3	PORTAbits.RA6
#define		KP_P_R4	PORTAbits.RA7
*/
typedef union
{
	unsigned char Key_Port;
	struct
	{
		unsigned C1:1;
		unsigned C2:1;
		unsigned C3:1;
		unsigned C4:1;
		unsigned R1:1;
		unsigned R2:1;
		unsigned R3:1;
		unsigned R4:1;	
	};
}KEYPAD;

void SetKPPort(unsigned char chr);
void KPInit(void);
unsigned char GetKPPort(void);
unsigned char KeyPadScan( void );


KEYPAD KP;
//const unsigned char KP_Port_Buff[] = {17,33,65,129,18,34,66,130,20,36,68,132,24,40,72,136};
//const unsigned char KP_Port_Buff[] = {0x11,0x21,0x41,0x81,0x12,0x22,0x42,0x82,0x14,0x24,0x44,0x84,0x18,0x28,0x48,0x88};
//const unsigned char	KP_Char_Buff[] = {'8' ,'0' ,'2' ,'5' ,'9' ,'C' ,'3' ,'6' ,'D' ,'E' ,'M' ,'U' ,'7' ,'S' ,'1' ,'4' };

const unsigned char KP_Port_Buff[] = {136,72,40,24,132,68,36,20,130,66,34,18,129,65,33,17};
const unsigned char	KP_Char_Buff[] = {'M' ,'U' ,'D' ,'E' ,'1' ,'4' ,'7' ,'S' ,'2' ,'5' ,'8' ,'0' ,'3' ,'6' ,'9' ,'C' };


int timeout = 0;

void KPInit(void)
{
	
	PMCONbits.PMPEN = 0;
	
	KP_D_C1 = 0;
	KP_D_C2 = 0;
	KP_D_C3 = 0;
	KP_D_C4 = 0;
	KP_D_R1 = 1;
	KP_D_R2 = 1;
	KP_D_R3 = 1;
	KP_D_R4 = 1;
	
	KP_P_C1 = 0;
	KP_P_C2 = 0;
	KP_P_C3 = 0;
	KP_P_C4 = 0;
	KP_P_R1 = 0;
	KP_P_R2 = 0;
	KP_P_R3 = 0;
	KP_P_R4 = 0;	

}

unsigned char GetKPPort(void)
{
	KP.C1 = KP_P_C1;
	KP.C2 = KP_P_C2;
	KP.C3 = KP_P_C3;
	KP.C4 = KP_P_C4;
	KP.R1 = KP_P_R1;
	KP.R2 = KP_P_R2;
	KP.R3 = KP_P_R3;
	KP.R4 = KP_P_R4;

	return KP.Key_Port;
}

void SetKPPort(unsigned char chr)
{
	KP.Key_Port = chr;
	KP_P_C1 = KP.C1;
	KP_P_C2 = KP.C2;
	KP_P_C3 = KP.C3;
	KP_P_C4 = KP.C4;
	KP_P_R1 = KP.R1;
	KP_P_R2 = KP.R2;
	KP_P_R3 = KP.R3;
	KP_P_R4 = KP.R4;
}
unsigned char KeyPadScan( void )
{
    	unsigned char ii =0, i =0, cKeyPad =0,Line[16];
    	timeout = 1000;
//    	sprintf(Line,"  Waiting....   ");
//	LCD_Write(Line,2);
	while(timeout > 0)
	{
		SetKPPort(0x0F);                							// re-init Keypad port 
		if(GetKPPort() != 0x0F)
		{
			for(i=0;i<4;i++)
			{
				SetKPPort(1<<i);
				for(ii=0;ii<16;ii++)
				{
					cKeyPad = GetKPPort();
					if(cKeyPad == KP_Port_Buff[ii])
					{ 			                           
						cKeyPad = KP_Char_Buff[ii];
						i=4,timeout =0;
						break;
					}
					else
						cKeyPad = 0;
				}
			}
		}
		timeout--;
	}
 //   	sprintf(Line,"     Done....   ");
//	LCD_Write(Line,2);

	return cKeyPad; 
}

