#include <includes.h>


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

typedef union
{
	unsigned int Err;
	struct
	{
		unsigned Server:1;
		unsigned GPRS:1;
		unsigned Modem:1;
		unsigned SIM:1;
		unsigned RTC:1;
		unsigned EM:1;
		unsigned PHR:1;
		unsigned PHY:1;
		unsigned PHB:1;
		unsigned EEPROM:1;
		unsigned UPLOAD:1;
		unsigned IMEI:1;
		unsigned LOWBAL:1;
		unsigned NOGSMSERV:1;
		unsigned LOWGSMSIG:1;
		unsigned Extra:1;
	};
}ERR_STS;



volatile INT8U RTC_SET = 0;																				// Variable to Indicate Change in Time or Date																	

extern INT8U	Task_WDT[5];				//	Definition in app_cfg.c
extern INT8U RTC_DD, RTC_MM, RTC_YY, RTC_WW;
extern INT8U Bat_V, Tmpr, SB;
extern INT8U SLOT1, SLOT2, SLOT3, GENRAL;
char Err_Str[] = "Error";

const INT8U keypad[] = {136,132,130,129,72,40,24,68,36,20,66,34,18,33,65,17};
const INT8U	character[] = {'M' ,'U' ,'D' ,'E' ,'1' ,'2' ,'3' ,'4' ,'5' ,'6' ,'7' ,'8' ,'9' ,'0' ,'S' ,'C' };
INT8U Yaxis = 0, Xaxis= 0, Zaxis = 0,  TimeOut = 0, Lcd_Z = 0;

INT8U CursorPos =0,KPHBuff[3], KPMBuff[3],KPDBuff[3],KPMoBuff[3],KPYBuff[3],KPWBuff[2], *pHour =0, *pMin =0, *pDD = 0, *pMM = 0, *pYY = 0, *pWW = 0 ;
static INT8U *MENU[] ={"      MENU      ", "                ", "<   Set Time   >", "<Light ON  Time>", "<Light OFF Time>", "< Reset System >", "< Default Sett >", " HH:MM - __:__  ", "      Error     ", "->Yes '0'    Cnl", "Yes '0'   No '1'", "< Packet Send  >", " Invalid Entery ", "< Enable Debug >", "<Disable Dimmer>", "      ____      ", "   Enter Key    ","    Set Date    "," DD:MM:YY DOW W "};
static INT32U Screen_Cnt = 0;
INT8U LCD_Buff[15] ;
static INT8U EnteredPW[5], KP_Data = 0;				//*pEnteredPW = 0;
INT8U MasterPW[5] = "8722";
INT8U ExstingPW[5] ;
INT8U Password = 0;						//used in Default settings in previous program
KEYPAD KP;

ERR_STS ES;
INT8U LCDSCREEN = 0;


static void MainScreen(void );
static void Init_Keypad(void);
static void KP_Scan(void);
extern void ClearBuffer(unsigned char *ptr, int length);
static void SetDateFunction(void );
static void SetKPPort(unsigned char chr);
static unsigned char GetKPPort(void);
static void Init_Keypad(void);
static void KP_Scan(void);
unsigned char MemoryCMP(unsigned char *Source, unsigned char *Destination);
static void Screen_LM(void);
unsigned char stringlength(unsigned char *ptr);
void SetTimeFunction(unsigned char WHAT_TIME );
void CustomScreens(unsigned char Zaxis);
void MenuScreens(unsigned char Xaxis );
unsigned char ConvertSTRING_CHAR(unsigned char *ptr);

void   SetResetFunction(void );
void CustomFunction( void );
void MenuFunction(void );
void PasswordEnter( unsigned char PswdType );


#define     ShowScreen(x,y,z) Xaxis = x, Yaxis = y, Zaxis = z
#define 	LCD_TIME				0
#define 	LCD_ON_TIME				1
#define 	LCD_OFF_TIME			2




void  AppLCDTask (void *p_arg)
{

	INT8S * rxMessage;
	INT8U *err;  
	
	XLCDInit();
	Init_Keypad();
	
	
    while(1)
    {
		Task_WDT[0] = 1;				//	LCD_TASK
		rxMessage = (INT8S*)OSMboxPend(LCDMboxPtr, 9000, err);	  
//		 if((*rxMessage) == COUNT_2000MS_TIMER)
		 {	  
			 LCD_BL_P = 1;//DR ;
			 KP_Scan();
			 switch(Yaxis)
			{
				case 0:	MainScreen();
						break;
				case 1:	if( Zaxis != 0)
							CustomFunction();
						else
							MenuFunction();
						break;
				case 2:	if( Lcd_Z == 0 )
							Lcd_Z = 1;
						PasswordEnter(0);
						break;
				case 3:	//ChangePassword();
						break;
				default:
						Screen_LM();
						break;
			}
		 }  
	
	}
}




static void MainScreen(void )
{		
	if(BUTTON1 && !BUTTON2)
	{
		LCDSCREEN = 1;
		Screen_Cnt = 0;
	}
	else if(!BUTTON1 && BUTTON2)
	{
		LCDSCREEN = 2;
		Screen_Cnt = 0;
	}
	else if(BUTTON1 && BUTTON2)
	{
		LCDSCREEN = 3;
		Screen_Cnt = 0;
	}
						
	if(LCDSCREEN == 0)
	{
		XLCDL1home();
		ClearBuffer(LCD_Buff,sizeof(LCD_Buff));
		if(Screen_Cnt <= 40)
		{	
			sprintf(LCD_Buff, "%02d:%02d:%02d CPU:%02d ",Sys_Time.StructTime.Hrs,Sys_Time.StructTime.Mins,Sys_Time.StructTime.Secs, OSCPUUsage);
		}
		else if(Screen_Cnt > 40 && Screen_Cnt <= 80)
		{
			if(ES.Err > 0)		
				sprintf(LCD_Buff, "%02d%cC BV:%02d %s",Tmpr, 223, Bat_V, Err_Str);
			else
				sprintf(LCD_Buff, "%02d%cC BV:%d.%d V   ",Tmpr, 223, Bat_V/10,Bat_V%10);	
		}
		else if(Screen_Cnt > 80 && Screen_Cnt <= 120)
		{
			sprintf(LCD_Buff, "%02d:%02d:%02d CPU:%02d ",Sys_Time.StructTime.Hrs,Sys_Time.StructTime.Mins,Sys_Time.StructTime.Secs, OSCPUUsage);
		}
		else if(Screen_Cnt > 120 && Screen_Cnt <= 160)												//	Display Voltages from External Energy Meter
		{
			sprintf(LCD_Buff, "V %03d A%03d PF%02d", Ex_V1, Ex_A1, Ex_PF1);				
		}
		else if(Screen_Cnt > 160 && Screen_Cnt <= 200)												//	Display Voltages from External Energy Meter
		{
			sprintf(LCD_Buff, "V %03d A%03d PF%02d", Ex_V1, Ex_A1, Ex_PF1);		
		}
		else if(Screen_Cnt > 200 && Screen_Cnt <= 240)												//	Display Voltages from External Energy Meter
		{
			
			sprintf(LCD_Buff, "VA R%05d Y%05d", R_D_VA, Y_D_VA);				
		}
		
		XLCDPutRamString(LCD_Buff);
		
		XLCDL2home();
		ClearBuffer(LCD_Buff,sizeof(LCD_Buff));
		if( Screen_Cnt <= 40)
		{
			sprintf(LCD_Buff, "RTC %02d:%02d:%02d  %dB",Hour,Min, Sec, SB); 
		} 
		else if( (Screen_Cnt > 40) && (Screen_Cnt <= 80) )
		{
			sprintf(LCD_Buff, " A%03d B%03d C%03d ",SLOT1, SLOT2, SLOT3);   
		}
		else if( (Screen_Cnt > 80) && (Screen_Cnt <= 120) )
		{
			sprintf(LCD_Buff, "IVR%03d Y%03d B%03d", R_D_IPV, Y_D_IPV, B_D_IPV);					//	Display Input Voltages from Digital Stabilizer 
		}
		else if( (Screen_Cnt > 120) && (Screen_Cnt <= 160) )
		{
			if( SR2 && SR4 && !SR3)
				R_D_OPV = Ex_V1;
			if( SY2 && SY4 && !SY3)
				Y_D_OPV = Ex_V2;
			if( SB2 && SB4 && !SB3)
				B_D_OPV = Ex_V3;
				
			sprintf(LCD_Buff, "OVR%03d Y%03d B%03d", R_D_OPV, Y_D_OPV, B_D_OPV);					//	Display Output Voltages from Digital Stabilizer 
		}
		else if( (Screen_Cnt > 160) && (Screen_Cnt <= 200) )
		{
			sprintf(LCD_Buff, "PFR.%02d Y.%02d B.%02d ", Ex_PF1, Ex_PF2, Ex_PF2);					//	Display Power Factor from External Energy Meter 
		}
		else if( (Screen_Cnt > 200) && (Screen_Cnt <= 240) )
		{
			sprintf(LCD_Buff, "B%05d Kwh%06ld", B_D_VA, Ex_KWH);									//	Display VA from DS and Kwh from External Energy Meter 
		}
		
		if(Screen_Cnt >= 240)
			Screen_Cnt = 0;
		Screen_Cnt++;
		
		XLCDPutRamString(LCD_Buff); 
		XLCD_BLINKOFF();
	}	
	else if(LCDSCREEN == 1)
	{
		if( Screen_Cnt <= 40 )
		{
			XLCDL1home();
			ClearBuffer(LCD_Buff,sizeof(LCD_Buff));
			sprintf(LCD_Buff,"    A  L  P     ");
			XLCDPutRamString(LCD_Buff);
			XLCDL2home();
			ClearBuffer(LCD_Buff,sizeof(LCD_Buff));
			if(ALP)
				sprintf(LCD_Buff,"    ENABLED     ");
			else
				sprintf(LCD_Buff,"    DISABLED    ");
			XLCDPutRamString(LCD_Buff); 
			XLCD_BLINKOFF();
		}
		else if( (Screen_Cnt > 40) && (Screen_Cnt <= 80)  )											//	DISPLAY PHASE - R ONTIME & OFFTIME 
		{
			XLCDL1home();
			ClearBuffer(LCD_Buff,sizeof(LCD_Buff));
			sprintf(LCD_Buff,"R-PHASE SLT %3dV",R_D_M_ExpV);
			XLCDPutRamString(LCD_Buff);
			XLCDL2home();
			ClearBuffer(LCD_Buff,sizeof(LCD_Buff));
			sprintf(LCD_Buff,"ON%02d:%02d OFF%02d:%02d",ONTime.StructTime.Hrs,ONTime.StructTime.Mins,OFFTime.StructTime.Hrs,OFFTime.StructTime.Mins);
			XLCDPutRamString(LCD_Buff); 
			XLCD_BLINKOFF();
		}
		if(ALP)
		{
			 if( (Screen_Cnt > 80) && (Screen_Cnt <= 120)   )
			{
				XLCDL1home();
				ClearBuffer(LCD_Buff,sizeof(LCD_Buff));
				sprintf(LCD_Buff,"Y-PHASE SLT %3dV",Y_D_M_ExpV);
				XLCDPutRamString(LCD_Buff);
				XLCDL2home();
				ClearBuffer(LCD_Buff,sizeof(LCD_Buff));
				sprintf(LCD_Buff,"ON%02d:%02d OFF%02d:%02d",Y_ONTime.StructTime.Hrs,Y_ONTime.StructTime.Mins,Y_OFFTime.StructTime.Hrs,Y_OFFTime.StructTime.Mins);
				XLCDPutRamString(LCD_Buff); 
				XLCD_BLINKOFF();
			}
			else if(  (Screen_Cnt > 120) && (Screen_Cnt <= 160)  )
			{
				XLCDL1home();
				ClearBuffer(LCD_Buff,sizeof(LCD_Buff));
				sprintf(LCD_Buff,"B-PHASE SLT %3dV",B_D_M_ExpV);
				XLCDPutRamString(LCD_Buff);
				XLCDL2home();
				ClearBuffer(LCD_Buff,sizeof(LCD_Buff));
				sprintf(LCD_Buff,"ON%02d:%02d OFF%02d:%02d",B_ONTime.StructTime.Hrs,B_ONTime.StructTime.Mins,B_OFFTime.StructTime.Hrs,B_OFFTime.StructTime.Mins );
				XLCDPutRamString(LCD_Buff); 
				XLCD_BLINKOFF();
			}
		}
		else if(Screen_Cnt > 80)
			Screen_Cnt = 161;
		if( Screen_Cnt > 160 )
		{
			Screen_Cnt = 0;
			LCDSCREEN = 0;
			ClearBuffer(LCD_Buff,sizeof(LCD_Buff));
		}
		Screen_Cnt++;
	}
	else if(LCDSCREEN == 2)
	{		
		if( Screen_Cnt <= 40 )													//	DISPLAY PHASE-R SENSOR STATUS
		{
			XLCDL1home();
			ClearBuffer(LCD_Buff,sizeof(LCD_Buff));
			sprintf(LCD_Buff,"R PHASE SENSORS ");
			XLCDPutRamString(LCD_Buff);
			XLCDL2home();
			ClearBuffer(LCD_Buff,sizeof(LCD_Buff));
			sprintf(LCD_Buff,"  %c  %c  %c  %c  %c ",(SR1 + 48),(SR2 + 48),(SR3 + 48),(SR4 + 48),(SR5 + 48));
			XLCDPutRamString(LCD_Buff); 
			XLCD_BLINKOFF();
		}
		else if( (Screen_Cnt > 40) && (Screen_Cnt <= 80)   )					//	DISPLAY PHASE-Y SENSOR STATUS	
		{
			XLCDL1home();
			ClearBuffer(LCD_Buff,sizeof(LCD_Buff));
			sprintf(LCD_Buff,"Y PHASE SENSORS ");
			XLCDPutRamString(LCD_Buff);
			XLCDL2home();
			ClearBuffer(LCD_Buff,sizeof(LCD_Buff));
			sprintf(LCD_Buff,"  %c  %c  %c  %c  %c ",(SY1 + 48),(SY2 + 48),(SY3 + 48),(SY4 + 48),(SY5 + 48));
			XLCDPutRamString(LCD_Buff); 
			XLCD_BLINKOFF();
		}
		else if((Screen_Cnt > 80) && (Screen_Cnt <= 120) )						//	DISPLAY PHASE-B SENSOR STATUS
		{
			XLCDL1home();
			ClearBuffer(LCD_Buff,sizeof(LCD_Buff));
			sprintf(LCD_Buff,"B PHASE SENSORS ");
			XLCDPutRamString(LCD_Buff);
			XLCDL2home();
			ClearBuffer(LCD_Buff,sizeof(LCD_Buff));
			sprintf(LCD_Buff,"  %c  %c  %c  %c  %c ",(SB1 + 48),(SB2 + 48),(SB3 + 48),(SB4 + 48),(SB5 + 48));
			XLCDPutRamString(LCD_Buff); 
			XLCD_BLINKOFF();
		}
		else if(Screen_Cnt > 120 )
		{
			Screen_Cnt = 0;
			LCDSCREEN = 0;
			ClearBuffer(LCD_Buff,sizeof(LCD_Buff));
		}
		Screen_Cnt++;
	}
	else if(LCDSCREEN == 3)														// DISPLAY ERRORS
	{		
		XLCDL1home();
		ClearBuffer(LCD_Buff,sizeof(LCD_Buff));
		sprintf(LCD_Buff,"*****ERRORS*****");
		XLCDPutRamString(LCD_Buff);
		XLCDL2home();
		ClearBuffer(LCD_Buff,sizeof(LCD_Buff));
		if(ES.Err == 0)		
		{
			Err_LED = 0;
			sprintf(LCD_Buff,"    NO ERRORS   ");
			Screen_Cnt = 601;
		}
		else
			Err_LED = 1;			
		if(Screen_Cnt <= 40)
		{
			if(ES.LOWGSMSIG)
				sprintf(LCD_Buff," LOW GSM SIGNAL ");
			else
				Screen_Cnt = 41;
		}
		if(Screen_Cnt > 40 && Screen_Cnt <= 80)
		{
			if(ES.NOGSMSERV)
				sprintf(LCD_Buff," NO GSM SERVICE ");
			else
				Screen_Cnt = 81;
		}
		if(Screen_Cnt > 80 && Screen_Cnt <= 120)
		{
			if(ES.LOWBAL)
				sprintf(LCD_Buff,"  LOW  BALANCE  ");
			else
				Screen_Cnt = 121;
		}
		if(Screen_Cnt > 120 && Screen_Cnt <= 160)
		{
			if(ES.UPLOAD)
				sprintf(LCD_Buff," UPLOAD  FAILED ");
			else
				Screen_Cnt = 161;
		}
		if(Screen_Cnt > 160 && Screen_Cnt <= 200)
		{
			if(ES.IMEI)
				sprintf(LCD_Buff,"DIV. REG. FAILED");
			else
				Screen_Cnt = 201;
		}
		if(Screen_Cnt > 200 && Screen_Cnt <= 240 )
		{
			if(ES.EEPROM)
				sprintf(LCD_Buff,"   CHK EEPROM   ");
			else
				Screen_Cnt = 241;
		}	
		if(Screen_Cnt > 240 && Screen_Cnt <= 280 )
		{
			if(ES.PHB)
				sprintf(LCD_Buff,"  CHK PH-B COM  ");
			else
				Screen_Cnt = 281;
		}	
		if(Screen_Cnt > 280 && Screen_Cnt <= 320 )
		{
			if(ES.PHY)
				sprintf(LCD_Buff,"  CHK PH-Y COM  ");
			else
				Screen_Cnt = 321;
		}
		if( Screen_Cnt > 320 && Screen_Cnt <= 360 )
		{
			if(ES.PHR)
				sprintf(LCD_Buff,"  CHK PH-R COM  ");
			else
				Screen_Cnt = 361;
		}
		if( Screen_Cnt > 360 && Screen_Cnt <= 400 )
		{
			if(ES.EM)
				sprintf(LCD_Buff," CHK EX.EM COM  ");
			else
				Screen_Cnt = 401;
		}
		if( Screen_Cnt > 400 && Screen_Cnt <= 440 )
		{
			if(ES.RTC)
				sprintf(LCD_Buff," CHK RTC STATUS ");
			else
				Screen_Cnt = 441;
		}	
		if( Screen_Cnt > 440 && Screen_Cnt <= 480)
		{
			if(ES.SIM)
				sprintf(LCD_Buff,"  CHK SIM CARD  ");
			else
				Screen_Cnt = 481;
		}
		if( Screen_Cnt > 480 && Screen_Cnt <= 520)
		{
			if(ES.Modem)
				sprintf(LCD_Buff,"  CHK MODEM COM ");
			else
				Screen_Cnt = 521;
		}
		if( Screen_Cnt > 520 && Screen_Cnt <= 560)
		{
			if(ES.GPRS)
				sprintf(LCD_Buff,"   GPRS   DOWN  ");
			else
				Screen_Cnt = 561;
		}
		if( Screen_Cnt > 560 && Screen_Cnt <= 600)
		{
			if(ES.Server)
				sprintf(LCD_Buff,"  SERVER DOWN  ");
			else
				Screen_Cnt = 601;
		}
		if(Screen_Cnt >= 600)
		{
			Screen_Cnt = 0;
			LCDSCREEN = 0;
		}
		Screen_Cnt++;
		XLCDPutRamString(LCD_Buff); 
		XLCD_BLINKOFF();
		ClearBuffer(LCD_Buff,sizeof(LCD_Buff));
	}	
}

static void Screen_LM(void)
{
	XLCDL1home();
 	ClearBuffer(LCD_Buff,sizeof(LCD_Buff));
    sprintf(LCD_Buff, "  LIGHT MATRIX  ");
	XLCDPutRamString(LCD_Buff); 
    XLCDL2home();
   	ClearBuffer(LCD_Buff,sizeof(LCD_Buff));
    sprintf(LCD_Buff, "     Hyderabad  "); 
   	XLCDPutRamString(LCD_Buff);   	
}

static void Init_Keypad(void)
{
	PMCONbits.PMPEN = 0;	
	DDR_KP_C1 = 0;
	DDR_KP_C2 = 0;
	DDR_KP_C3 = 0;
	DDR_KP_C4 = 0;
	DDR_KP_R1 = 1;
	DDR_KP_R2 = 1;
	DDR_KP_R3 = 1;
	DDR_KP_R4 = 1;
	
	BUTTON1_D = 1;
	BUTTON2_D = 1;
	
	KP_C1 = 0;
	KP_C2 = 0;
	KP_C3 = 0;
	KP_C4 = 0;
	KP_R1 = 0;
	KP_R2 = 0;
	KP_R3 = 0;
	KP_R4 = 0;	
	
	KP_PW[0] = K1;
	KP_PW[1] = K2;
	KP_PW[2] = K3;
	KP_PW[3] = K4;
	
}

static unsigned char GetKPPort(void)
{
	KP.C1 = KP_C1;
	KP.C2 = KP_C2;
	KP.C3 = KP_C3;
	KP.C4 = KP_C4;
	KP.R1 = KP_R1;
	KP.R2 = KP_R2;
	KP.R3 = KP_R3;
	KP.R4 = KP_R4;

	return KP.Key_Port;
}

static void SetKPPort(unsigned char chr)
{
	KP.Key_Port = chr;
	KP_C1 = KP.C1;
	KP_C2 = KP.C2;
	KP_C3 = KP.C3;
	KP_C4 = KP.C4;
	KP_R1 = KP.R1;
	KP_R2 = KP.R2;
	KP_R3 = KP.R3;
	KP_R4 = KP.R4;
}

static void KP_Scan(void)
{
	unsigned char ii =0, i =0, cKeyPad =0;
	SetKPPort(0x0F);                							// re-init Keypad port 
	if(GetKPPort != 0x0F)
	{
		for(i=0;i<4;i++)
		{
			SetKPPort(1<<i);
			for(ii=0;ii<16;ii++)
			{
				if(GetKPPort() == keypad[ii])
				{ 								                           
					KP_Data = cKeyPad = character[ii];
					i=4,TimeOut =0;
					break;
				}
			}
		}
	}
	if(cKeyPad == 'M')
	{
		if(Yaxis < 1)
			Yaxis++;
	}
	else if(cKeyPad == 'C' && !Zaxis )
	{
		if (Yaxis > 0)
		{
			Yaxis--,Xaxis = 0; 
			if (Yaxis == 0)
				Password = 0;
		}
	}
	else if( (Yaxis == 1) && cKeyPad == 'U')
	{
		if(Xaxis >= 1)
			Xaxis--;
	}
	else if( (Yaxis == 1) && cKeyPad == 'D')
	{
		if(Xaxis < 3)
			Xaxis++;
	}
	if( Yaxis )
		TimeOut++;
	if( TimeOut >= 50 )
	{
		Xaxis = 0, Yaxis = 0, Zaxis = 0,TimeOut =0, Password = 0;
		Lcd_Z =0;
	}
	if( cKeyPad != 0 && Password == 0)
	{
		Yaxis = 2;
		cKeyPad = 0;
	}
	cKeyPad = 0; 
}

void MenuFunction(void )
{	
	XLCDL1home();
	XLCDPutRamString(MENU[0]);
	XLCDL2home();
	MenuScreens( Xaxis );
	if( Xaxis == 4 )
		Lcd_Z = 1;
	else 
		Lcd_Z = 0;
	if( KP_Data == 'E')
		Zaxis = Xaxis + 1;
}

void MenuScreens(unsigned char Xaxis )
{
	switch( Xaxis )
	{
		case 0:
			XLCDPutRamString(MENU[2]);  
			break;
		case 1:
			XLCDPutRamString(MENU[17]);  
			break;
		case 2:
			XLCDPutRamString(MENU[3]);  
			break;    
		case 3:
			XLCDPutRamString(MENU[4]);  
			break;  
		default:
			XLCDPutRamString(MENU[1]);
			break;
	}
}

void CustomFunction( void )
{
	if( !Lcd_Z )
	{
		ClearBuffer(KPHBuff,sizeof(KPHBuff));
		ClearBuffer(KPMBuff,sizeof(KPMBuff));
		XLCDL1home();
		MenuScreens( Zaxis-1 );
		XLCDL2home();
		CustomScreens( Zaxis-1 );
		XLCD_BLINKOON();
		Lcd_Z = 1,KP_Data = 0,CursorPos =0;
		pHour = KPHBuff, pMin = KPMBuff;
		pDD = KPDBuff, pMM = KPMoBuff, pYY = KPYBuff, pWW = KPWBuff;
	}
	switch(Zaxis)
  	{
		case 1:	SetTimeFunction(LCD_TIME);
				break;
		case 2:	SetDateFunction();
				break;
		case 3:	SetTimeFunction(LCD_ON_TIME);
				break;
		case 4:	SetTimeFunction(LCD_OFF_TIME);
         		break;
		default:
				break;
	}
}

void CustomScreens(unsigned char Zaxis)
{
	switch(Zaxis)
	{
		case 0:
			XLCDPutRamString(MENU[7]);
			break;
		case 1:
			XLCDPutRamString(MENU[18]);
			break;
		case 2:
			XLCDPutRamString(MENU[7]);
			break;
		case 3:
			XLCDPutRamString(MENU[7]);
			break;
		default:
			XLCDPutRamString(MENU[10]);
			break;	
	}
}


void PasswordEnter( unsigned char PswdType )
{
	if( Lcd_Z == 1)
	{
		XLCDL1home();
		XLCDPutRamString(MENU[16]); 
		XLCDL2home();
		Lcd_Z = 2,KP_Data = 0,CursorPos =0;
		ClearBuffer(EnteredPW,sizeof(EnteredPW));
		XLCDPutRamString(MENU[15]);                               	
		MoveLeft(10);	                                    	
		XLCD_BLINKOON();	                     			    //Blinking the cursor                                 

	}
	
	if(Lcd_Z == 2)
	{
		if( KP_Data > 47 && KP_Data < 58 )       				// Checking if any numerical values are pressed         
		{
			CursorPos++;				          				// Counting the number of characters                    
			if(CursorPos > 0 && CursorPos < 5) 					// Only first 4 characters are taken as password       
			{
				XLCDPut('*');					  				// Masking the key by '*'                               
				EnteredPW[CursorPos - 1] = KP_Data;
				EnteredPW[CursorPos] = 0;
			}
			if(CursorPos == 4)
				XLCD_BLINKOFF();
			KP_Data =0;
		}
		else if ( KP_Data == 'E')
		{
			
		  	ExstingPW[0] = KP_PW[0];
		  	ExstingPW[1] = KP_PW[1]; 
		   	ExstingPW[2] = KP_PW[2]; 
	    	ExstingPW[3] = KP_PW[3];
			ExstingPW[4] = KP_PW[4];
			
			XLCDPutRamString("\n\r"); 
			XLCDPutRamString(EnteredPW); 
			XLCDPutRamString("\n\r"); 
			
			Debug_PutString("\n\r ENTERED :");	
			Debug_PutString(EnteredPW);	
			Debug_PutString("\n\r EXISTING :");	
			Debug_PutString(ExstingPW);	
			Debug_PutString("\n\r");	
			
			if( !PswdType )														// check for main password
			{
				if( MemoryCMP(ExstingPW , EnteredPW) || MemoryCMP(MasterPW , EnteredPW) )
				{
					
					Lcd_Z = 0,KP_Data =0, Password = 1;//,pEnteredPW = 0;
					ShowScreen(0,1,0);
				}
				else
				{
					ShowScreen(0,0,0);                 							// Go to the Main Menu if Wrong Password is pressed 
					Lcd_Z = 0,KP_Data =0, Password = 0;
				}
			}
			else if( PswdType )													// check for Hard reset password
			{
				if( MemoryCMP(MasterPW,EnteredPW) )
				{
					KP_Data =0;//,pEnteredPW = 0;
					Lcd_Z = 1;
				}
				else
				{
					ShowScreen(Zaxis-1,1,0);                 					// Go to the Main Menu if Wrong Password is pressed 
					KP_Data =0;
				}
			}	
		}
		else if (KP_Data == 'C')
		{
			if( !PswdType )
			{
				Lcd_Z = 0,KP_Data =0, Password = 0;
				ShowScreen(0,0,0);
			}
			else if( PswdType )
			{
				KP_Data =0;
				ShowScreen(Zaxis-1,1,0);
			}
			ClearBuffer(EnteredPW,sizeof(EnteredPW));
		}
	}	
}


unsigned char MemoryCMP(unsigned char *Source, unsigned char *Destination)
{
	unsigned char compare = 0;
	if( stringlength(Source) == stringlength(Destination))
	{
		while(*Source)
		{
			if(*Source++ != *Destination++)
			 {
				compare = 0;
				break;
			 }
			else compare = 1;
		}
		return compare;
	}
	else return 0;
}

unsigned char stringlength(unsigned char *ptr)
{
	unsigned char length = 0;

	while(*ptr++)
		length++;
	return length;
}

void SetTimeFunction(unsigned char WHAT_TIME )
{
	unsigned char bHour =0, bMin =0;
	if(Lcd_Z == 1)
	{
		Lcd_Z++;
		MoveLeft(7);
	}
	if(Lcd_Z == 2)
	{
		if( KP_Data > 47 && KP_Data < 58 && CursorPos < 4)
		{
			CursorPos++;
			if(CursorPos >= 0 && CursorPos<= 4)
			{
				XLCDPut(KP_Data);
				if(CursorPos == 1 || CursorPos == 2 )
					*pHour++ = KP_Data, *pHour ='\0';
				else if(CursorPos >= 3)
					*pMin++ = KP_Data, *pMin = '\0'; 
				if( CursorPos == 2)
					XLCDPut(':');
			}
			if(CursorPos == 4)
				XLCD_BLINKOFF();
			KP_Data =0;
		}
		if( KP_Data == 'C')
		{
			Lcd_Z = 0,KP_Data =0, pMin =0, pHour =0;
			ShowScreen(WHAT_TIME,1,0);
		}
		else if (KP_Data == 'S')
		{
			bHour =  ConvertSTRING_CHAR(KPHBuff);
			bMin  =  ConvertSTRING_CHAR(KPMBuff);
			
			if( bHour < 24 &&  bHour >= 0 && bMin < 60 && bMin >= 0 && CursorPos == 4 )
			{

				if(WHAT_TIME == LCD_TIME)
				{
					Sys_Time.StructTime.Hrs = bHour;
					Sys_Time.StructTime.Mins = bMin;
					RTC_SET = 1;
				}
				else if(WHAT_TIME == LCD_ON_TIME)
				{
					ALP = 0;
					ONTime.StructTime.Hrs = Y_ONTime.StructTime.Hrs = B_ONTime.StructTime.Hrs = bHour;
					ONTime.StructTime.Mins	= Y_ONTime.StructTime.Mins = B_ONTime.StructTime.Mins = bMin;
				}
				else if(WHAT_TIME == LCD_OFF_TIME)
				{
					ALP = 0;
					OFFTime.StructTime.Hrs	= Y_OFFTime.StructTime.Hrs = B_OFFTime.StructTime.Hrs = bHour;
					OFFTime.StructTime.Mins	= Y_OFFTime.StructTime.Mins = B_OFFTime.StructTime.Mins = bMin;
				}
			}
			Lcd_Z = 0,KP_Data =0,pMin =0, pHour =0,CursorPos = 0;
			ShowScreen(WHAT_TIME,1,0);
		}
	}
}

static void SetDateFunction(void )
{
	unsigned char DD =0, MM =0, YY = 0, WW = 0;

	if(Lcd_Z == 1)
	{
		Lcd_Z++;
		MoveLeft(15);
	}
	if(Lcd_Z == 2)
	{
		if( KP_Data > 47 && KP_Data < 58 && CursorPos < 14)
		{
			CursorPos++;
			if(CursorPos == 1 || CursorPos == 2 || CursorPos == 4 || CursorPos == 5 || CursorPos == 7 || CursorPos == 8 || CursorPos == 14)
			{
				XLCDPut(KP_Data);
				if(CursorPos == 1 || CursorPos == 2 )
					*pDD++ = KP_Data, *pDD ='\0';
				else if(CursorPos == 4 || CursorPos == 5 )
					*pMM++ = KP_Data, *pMM = '\0'; 
				else if(CursorPos == 7 || CursorPos == 8 )
					*pYY++ = KP_Data, *pYY = '\0'; 
				else if(CursorPos == 14)
					*pWW++ = KP_Data, *pWW = '\0'; 
			}
			if( CursorPos == 2 || CursorPos == 5)
				XLCDPut(':');
			
				
			else if( CursorPos == 8)
			{
				XLCDPut(' ');
				XLCDPut('D');
				XLCDPut('O');
				XLCDPut('W');
				XLCDPut(' ');
				CursorPos++;
				CursorPos++;
				CursorPos++;
				CursorPos++;
			}
			if(CursorPos == 14)
				XLCD_BLINKOFF();
			KP_Data =0;
		}
		if( KP_Data == 'C')
		{
			Lcd_Z = 0,KP_Data =0, pDD = 0, pMM = 0, pYY = 0, pWW = 0 ;
			ShowScreen(1,1,0);
		}
		else if (KP_Data == 'S')
		{

			DD =  ConvertSTRING_CHAR(KPDBuff);
			MM  =  ConvertSTRING_CHAR(KPMoBuff);
			YY = ConvertSTRING_CHAR(KPYBuff);
			WW = ConvertSTRING_CHAR(KPWBuff);
			if(DD > 0 && DD <32)
				RTC_DD = DD;
			if(MM > 0 && MM <13)
				RTC_MM = MM;
			if(YY > 10 && YY < 99)
				RTC_YY = YY;
			if(WW > 0 && WW < 8)
				RTC_WW = WW;
			RTC_SET = 1;	
			Lcd_Z = 0,KP_Data =0, pDD = 0, pMM = 0, pYY = 0, pWW = 0 ;
			ShowScreen(1,1,0);		
		}
	}
}

unsigned char ConvertSTRING_CHAR(unsigned char *ptr)
{
	unsigned char c = 0,i = 1,len = 0;
	len = stringlength(ptr);
	while(--len)
		i = i*10;
	while(*ptr)
	{
		c = c + i*(*ptr++ - 48);
		i = i/10;
	}
	return c;
}


void   SetResetFunction(void )
{
	if(Lcd_Z == 1)
	{
		if( KP_Data == 'C')
			ShowScreen(5,1,0),Lcd_Z = 0,KP_Data =0;
		else if(KP_Data == 'E')
		{
			
		}
		else if (KP_Data == '0')
			__asm__ volatile ("reset") ;
	}
}

