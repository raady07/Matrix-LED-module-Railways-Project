#include <includes.h>
#include "LibraryFunctions.h"

typedef union Sensors
{
	INT16U Val;
	struct
	{
		unsigned R1:1;
		unsigned R2:1;
		unsigned R3:1;
		unsigned R4:1;
		unsigned R5:1;
		unsigned Y1:1;
		unsigned Y2:1;
		unsigned Y3:1;
		unsigned Y4:1;
		unsigned Y5:1;
		unsigned B1:1;
		unsigned B2:1;
		unsigned B3:1;
		unsigned B4:1;
		unsigned B5:1;
		unsigned Dr:1;
			
	};

}SEN;

SEN Sen;


UnionTimer ONTime;
UnionTimer OFFTime;
UnionTimer Y_ONTime;
UnionTimer B_ONTime;
UnionTimer Y_OFFTime;
UnionTimer B_OFFTime;
UnionTimer M_Time;


BOOL Packet1Start 	= FALSE;
BOOL Packet1End 	= FALSE;
BOOL Packet2Start 	= FALSE;
BOOL Packet2End 	= FALSE;
BOOL Packet3Start 	= FALSE;
BOOL Packet3End 	= FALSE;
BOOL Packet4Start 	= FALSE;
BOOL Packet4End 	= FALSE;
BOOL PH3 			= FALSE;		//Single Phase
//BOOL PH3 			= TRUE;			//Three Phase
BOOL ALP 			= ALP_DISABLE;

unsigned char Bat_V = 0, Tmpr = 0, SB = 0;
extern INT8U R_CSLOT, Y_CSLOT, B_CSLOT;

INT16U M_Err = 0;
INT8U KP_PW[5];
INT8U RD_Val = 'D';
INT8U YD_Val = 'D';
INT8U BD_Val = 'D';

INT16U HV_Cut = Default_HV_Cut, R_HV_Cut = Default_HV_Cut, Y_HV_Cut = Default_HV_Cut, B_HV_Cut = Default_HV_Cut;
INT8U LV_Cut = Default_LV_Cut, R_LV_Cut = Default_LV_Cut, Y_LV_Cut = Default_LV_Cut, B_LV_Cut = Default_LV_Cut;
INT16U VA_Max = Default_Max_VA, R_VA_Max = Default_Max_VA, Y_VA_Max = Default_Max_VA, B_VA_Max = Default_Max_VA;
INT8U RD_Bypass = FALSE, YD_Bypass = FALSE, BD_Bypass = FALSE;

extern INT8U	Task_WDT[5];				//	Definition in app_cfg.c

INT8U ModemRecievedData[UART1_BUFF_SIZE];
INT16S Uart1RxDataIndex = 0;
INT8U ModemRecievedDataCopy[UART1_BUFF_SIZE];
INT16S ModemRecievedDataCopyIndex = 0;

extern void EEPROM_write(INT16U Address, INT8U *Data, INT8U Len);				//	Definition eeprom_router.c
extern void EEPROM_read(INT16U Address, INT8U *Data, INT8U Len);				//	Definition eeprom_router.c
void EEPROM_ReadAll(void);

INT8U RTC_DD = 23, RTC_MM = 6, RTC_YY = 11, RTC_WW = 5;

static void ParsePacket1(void);
static void ParsePacket2(void);
static void ParsePacket3(void);
static void ParsePacket4(void);
static void Tx2Modem(void);

void AppModemCommTask (void *p_arg)
{
	INT8S * rxMessage;
	INT8U *err;
    INT16S Modem_Rx_Data_Len = 0;
    INT8U Tx_Cnt = 0;					//	 Transimit Counter

    // For RTC
	INT8U SW_RTC_Sync 		=  TRUE; 
	INT16U SW_RTC_Sync_Time = 0;
    Sys_Time.TimeValue 		= 0;
    ONTime.TimeValue 		= 0;
	OFFTime.TimeValue 		= 0;
	Y_ONTime.TimeValue 		= 0;
	B_ONTime.TimeValue 		= 0;
	Y_OFFTime.TimeValue 	= 0;
	B_OFFTime.TimeValue 	= 0;
	M_Time.TimeValue 		= 0;

    	
	OpenI2C1( I2C_ON, I2C_BRG ); 
	Modem_Init();
	KP_PW[0]  = '1';
	KP_PW[1]  = '2';
	KP_PW[2]  = '8';
	KP_PW[3]  = '9';
	EEPROM_ReadAll();										//	READ DATA FROM EEPROM	
   	while(1)
    {
//	    OSTimeDly(100);										// OSTimeDly(100) 1 Sec		
	    Task_WDT[1] = 1;				//	MODEM
	    rxMessage = (INT8S*)OSMboxPend(UART1MboxPtr, 9000, err);
	    
 		Read_RTC();        
		if(++SW_RTC_Sync_Time == SW_RTC_SYNC_TIME_VAL )
		{
			Sys_Time.StructTime.Secs = Sec;
			Sys_Time.StructTime.Mins = Min;
			Sys_Time.StructTime.Hrs = Hour;
			Sys_Time.StructTime.Reserved = 0;					// To avoid junk in MSB w.r.t Int Value of Union
			SW_RTC_Sync_Time = FALSE;
		}	
		if(RTC_SET)
		{
				W_RTC_Time(Sys_Time.StructTime.Hrs,Sys_Time.StructTime.Mins,Sys_Time.StructTime.Secs);
				W_RTC_Date(RTC_DD,RTC_MM,RTC_YY,RTC_WW);
				RTC_SET = 0;
		}
//		if((*rxMessage) == COUNT_1000MS_TIMER)
		{
			if(Tx_Cnt++ >= 25)
			{
				Tx_Cnt = 0;
				Tx2Modem();
			}
			Modem_Rx_Data_Len = Read_Modem_Data();
			if(Modem_Rx_Data_Len>0)
			{
				Parse_Modem_Data(Modem_Rx_Data_Len);
			}
		}
		ON_OFF_Switch();										// Switch ON/OFF Load
	}

}

void Parse_Modem_Data(INT16S Len)
{
	INT16S i = 0;
    static	BOOL PacketStart = FALSE;
	INT16S LenCopy = Len;
	#ifdef	M_DEBUG
		Debug_PutString("\n\r");
		Debug_PutString(ModemRecievedData);	
		Debug_PutString("\n\r");
	#endif
	 
	if((ModemRecievedData[i] == '~') && (Packet1Start == FALSE))
	{
	    Packet1Start = TRUE;
	    PacketStart = TRUE;
	}
	else if((ModemRecievedData[i] == '@') && (Packet2Start == FALSE))
	{
	    Packet2Start = TRUE;
	    PacketStart = TRUE;	
	}
	else if((ModemRecievedData[i] == '$') && (Packet3Start == FALSE))
	{
	    Packet3Start = TRUE;	
	    PacketStart = TRUE;
	}
	else if((ModemRecievedData[i] == '&') && (Packet4Start == FALSE))
	{
	    Packet4Start = TRUE;	
	    PacketStart = TRUE;
	}

	if(PacketStart)
	{
	   while(LenCopy)
	   {
	   	  ModemRecievedDataCopy[ModemRecievedDataCopyIndex] = ModemRecievedData[i];
	   	  LenCopy--;
	   	  i++;
	   	  ModemRecievedDataCopyIndex++;
	   }
	   PacketStart = FALSE;				// Added on 12th May 2012 Chaitu 
	   ModemRecievedDataCopyIndex = 0;	// Added on 12th May 2012 Chaitu
	}	
	
	if((ModemRecievedData[Len-1] == '!') && (Packet1Start == TRUE) && (Packet1End == FALSE))
	{
		Packet1End = TRUE;	
	}
	if((ModemRecievedData[Len-1] == '#') && (Packet2Start == TRUE) && (Packet2End == FALSE))
	{
		Packet2End = TRUE;
	}
	if((ModemRecievedData[Len-1] == '%') && (Packet3Start == TRUE) && (Packet3End == FALSE))
	{
		Packet3End = TRUE;
	}
	if((ModemRecievedData[Len-1] == '*') && (Packet4Start == TRUE) && (Packet4End == FALSE))
	{
		Packet4End = TRUE;
	}
	if(Packet1End == TRUE)	
	{
		ParsePacket1();	   		
		Packet1End = FALSE;
		Packet1Start = FALSE;
		PacketStart = FALSE;
		ModemRecievedDataCopyIndex = 0;
	}
	if(Packet2End == TRUE)	
	{
		ParsePacket2();	   
		Packet2End = FALSE;
		Packet2Start = FALSE;
		PacketStart = FALSE;
		ModemRecievedDataCopyIndex = 0;
	}
	if(Packet3End == TRUE)
	{
		ParsePacket3();			    
		Packet3End = FALSE;
		Packet3Start = FALSE;
		PacketStart = FALSE;
		ModemRecievedDataCopyIndex = 0;	
	}
	if(Packet4End == TRUE)
	{
		ParsePacket4();			    
		Packet4End = FALSE;
		Packet4Start = FALSE;
		PacketStart = FALSE;
		ModemRecievedDataCopyIndex = 0;	
	}	
}

static void ParsePacket1(void)
{
	INT8U TMP[6] = {0};				//Start EEPROM ADDRESS is 0x00(PAGE 1)
	
	Sys_Time.StructTime.Hrs 	= 	((ModemRecievedDataCopy[5]-'0')  * 10 + (ModemRecievedDataCopy[6]-'0'));
	M_Time.StructTime.Hrs  		=  	Sys_Time.StructTime.Hrs;

	Sys_Time.StructTime.Mins 	=  	((ModemRecievedDataCopy[8]-'0')  * 10 + (ModemRecievedDataCopy[9]-'0'));
	M_Time.StructTime.Mins  	= 	Sys_Time.StructTime.Mins;
	
	Sys_Time.StructTime.Secs 	=   ((ModemRecievedDataCopy[11]-'0') * 10 + (ModemRecievedDataCopy[12]-'0'));
	M_Time.StructTime.Secs		= 	Sys_Time.StructTime.Secs;	
		
	RTC_SET	=	1;
	
	ONTime.StructTime.Hrs  		=  ((ModemRecievedDataCopy[22]-'0') * 10 + (ModemRecievedDataCopy[23]-'0'));
	TMP[0] = ONTime.StructTime.Hrs;
	
	ONTime.StructTime.Mins  	=  ((ModemRecievedDataCopy[25]-'0') * 10 + (ModemRecievedDataCopy[26]-'0'));
	TMP[1] = ONTime.StructTime.Mins;
	
	ONTime.StructTime.Secs  	=  ((ModemRecievedDataCopy[28]-'0') * 10 + (ModemRecievedDataCopy[29]-'0'));
	
	OFFTime.StructTime.Hrs  	=  ((ModemRecievedDataCopy[34]-'0') * 10 + (ModemRecievedDataCopy[35]-'0'));
	TMP[2] = OFFTime.StructTime.Hrs;
	
	OFFTime.StructTime.Mins  	=  ((ModemRecievedDataCopy[37]-'0') * 10 + (ModemRecievedDataCopy[38]-'0'));
	TMP[3] = OFFTime.StructTime.Mins;
	
	OFFTime.StructTime.Secs  	=  ((ModemRecievedDataCopy[40]-'0') * 10 + (ModemRecievedDataCopy[41]-'0'));
	
	if(ModemRecievedDataCopy[17] == '3')
	{
		PH3 = TRUE;
		TMP[4] = TRUE;								//	THREE PHASE
	}
	else
	{
		PH3 = FALSE;
		TMP[4] = FALSE;								
	}							//	SINGLE PHASE			
	if(ModemRecievedDataCopy[47] == 'Y' || ModemRecievedDataCopy[47] == 'y')
	{
		ALP = TRUE;								//	ENABLE ALP
		TMP[5] = TRUE;
	}
	else
	{
		ALP = FALSE;							//	DISABLE ALP
		TMP[5] = FALSE;
	}
	
	M_Err = (((ModemRecievedDataCopy[53] - '0') *10000) + ((ModemRecievedDataCopy[54]-'0') * 1000) + ((ModemRecievedDataCopy[55]-'0') * 100) + ((ModemRecievedDataCopy[56]-'0') * 10)+ (ModemRecievedDataCopy[57]-'0')) ;

	EEPROM_write(EEPROM_PAGE1, TMP, 6);
}

static void ParsePacket2(void)
{

	INT8U TMP[12] = {0};//Start EEPROM ADDRESS is 0x41(PAGE 2)
	
	TMP[0] = 0;
	TMP[1] = 0;
	TMP[2] = 0;
	TMP[3] = 0;
	
	Y_ONTime.StructTime.Hrs  		=	((ModemRecievedDataCopy[5]-'0') * 10 + (ModemRecievedDataCopy[6]-'0'));
	TMP[4] = Y_ONTime.StructTime.Hrs;
	
	Y_ONTime.StructTime.Mins  		=  ((ModemRecievedDataCopy[8]-'0') * 10 + (ModemRecievedDataCopy[9]-'0'));
	TMP[5] = Y_ONTime.StructTime.Mins;
	
	Y_ONTime.StructTime.Secs  		=  ((ModemRecievedDataCopy[11]-'0') * 10 + (ModemRecievedDataCopy[12]-'0'));
	
	Y_OFFTime.StructTime.Hrs  		=  ((ModemRecievedDataCopy[17]-'0') * 10 + (ModemRecievedDataCopy[18]-'0'));
	TMP[6] = Y_OFFTime.StructTime.Hrs;
	
	Y_OFFTime.StructTime.Mins  	= ((ModemRecievedDataCopy[20]-'0') * 10 + (ModemRecievedDataCopy[21]-'0'));
	TMP[7] = Y_OFFTime.StructTime.Mins;
	
	Y_OFFTime.StructTime.Secs  	= ((ModemRecievedDataCopy[23]-'0') * 10 + (ModemRecievedDataCopy[24]-'0'));
	
	B_ONTime.StructTime.Hrs  		=  ((ModemRecievedDataCopy[29]-'0') * 10 + (ModemRecievedDataCopy[30]-'0'));
	TMP[8] = B_ONTime.StructTime.Hrs;
	
	B_ONTime.StructTime.Mins  		=  ((ModemRecievedDataCopy[32]-'0') * 10 + (ModemRecievedDataCopy[33]-'0'));
	TMP[9] = B_ONTime.StructTime.Mins;
	
	B_ONTime.StructTime.Secs  		=  ((ModemRecievedDataCopy[35]-'0') * 10 + (ModemRecievedDataCopy[36]-'0'));	
	
	B_OFFTime.StructTime.Hrs  		=  ((ModemRecievedDataCopy[41]-'0') * 10 + (ModemRecievedDataCopy[42]-'0'));
	TMP[10] = B_OFFTime.StructTime.Hrs;
	
	B_OFFTime.StructTime.Mins  	=  ((ModemRecievedDataCopy[44]-'0') * 10 + (ModemRecievedDataCopy[45]-'0'));
	TMP[11] = B_OFFTime.StructTime.Mins;
	
	B_OFFTime.StructTime.Secs  	=  ((ModemRecievedDataCopy[47]-'0') * 10 + (ModemRecievedDataCopy[48]-'0'));
	
	EEPROM_write(EEPROM_PAGE2, TMP, 12);

}

static void ParsePacket3(void)	
{
	INT8U TMP[21] = {0};//Start EEPROM ADDRESS is 81(PAGE 3)
	INT8_16U Tmp;

	KP_PW[0] 	= ModemRecievedDataCopy[5];// - 48;
	TMP[0]	 	= KP_PW[0];
	
	KP_PW[1] 	= ModemRecievedDataCopy[6];// - 48;
	TMP[1] 		= KP_PW[1];
	
	KP_PW[2] 	= ModemRecievedDataCopy[7];// - 48;
	TMP[2] 		= KP_PW[2];
	
	KP_PW[3] 	= ModemRecievedDataCopy[8];// - 48;
	TMP[3] 		= KP_PW[3];
	
	KP_PW[4] 	= '\0';
	
	if((ModemRecievedDataCopy[15] != 'B') && (ModemRecievedDataCopy[15] != 'D'))
	{
			RD_Val   =  (((ModemRecievedDataCopy[13] - '0') *100) + ((ModemRecievedDataCopy[14]-'0') * 10) + (ModemRecievedDataCopy[15]-'0'));
			TMP[4] = RD_Val;
	}
	else if(ModemRecievedDataCopy[15] == 'B')
	{
		RD_Bypass	=	TRUE;
		TMP[5] = RD_Bypass;
	}	
	else
	{
		RD_Val   = ModemRecievedDataCopy[15];
		TMP[4] = RD_Val;
		
		RD_Bypass	=	FALSE;
		TMP[5] = RD_Bypass;
	}	
	if((ModemRecievedDataCopy[22] != 'B') && (ModemRecievedDataCopy[22] != 'D'))
	{	
		YD_Val   =  (((ModemRecievedDataCopy[20] - '0') *100) + ((ModemRecievedDataCopy[21]-'0') * 10) + (ModemRecievedDataCopy[22]-'0'));
		TMP[6] = YD_Val;
	}
	else if(ModemRecievedDataCopy[22] == 'B')
	{
		YD_Bypass	=	TRUE;
		TMP[7] = YD_Bypass;
	}	
	else
	{
		YD_Val   = ModemRecievedDataCopy[22];
		TMP[6] = YD_Val;
		YD_Bypass	=	FALSE;
		TMP[7] = YD_Bypass;
	}
	if((ModemRecievedDataCopy[29] != 'B') && (ModemRecievedDataCopy[29] != 'D'))
	{
		BD_Val   	=  (((ModemRecievedDataCopy[27] - '0') *100) + ((ModemRecievedDataCopy[28]-'0') * 10) + (ModemRecievedDataCopy[29]-'0'));
		TMP[8] 		= 	BD_Val;
	}	
	else if(ModemRecievedDataCopy[29] == 'B')
	{
		BD_Bypass	=	TRUE;
		TMP[9] 		= 	BD_Bypass;
	}	
	else
	{	
		BD_Val  	= 	ModemRecievedDataCopy[29];
		TMP[8] 		= 	BD_Val;
		BD_Bypass	=	FALSE;
		TMP[9] 		= 	BD_Bypass;
	}	
	
	HV_Cut =   (((ModemRecievedDataCopy[34] - '0') *100) + ((ModemRecievedDataCopy[35]-'0') * 10) + (ModemRecievedDataCopy[36]-'0'));
	
	R_HV_Cut = HV_Cut;
	Y_HV_Cut = HV_Cut;
	B_HV_Cut = HV_Cut;

	Tmp.Val = R_HV_Cut;
	TMP[10] = Tmp.A;
	TMP[11] = Tmp.B;
	
	Tmp.Val = Y_HV_Cut;
	TMP[12] = Tmp.A;
	TMP[13] = Tmp.B;
	
	Tmp.Val = B_HV_Cut;
	TMP[14] = Tmp.A;
	TMP[15] = Tmp.B;
	
	LV_Cut =   (((ModemRecievedDataCopy[41] - '0') *100) + ((ModemRecievedDataCopy[42]-'0') * 10) + (ModemRecievedDataCopy[43]-'0')); 
	
	R_LV_Cut 	= 	LV_Cut;
	Y_LV_Cut 	= 	LV_Cut;
	B_LV_Cut 	= 	LV_Cut;
	TMP[16] 	=  	LV_Cut;
	TMP[17] 	=  	LV_Cut;
	TMP[18] 	=  	LV_Cut;
	
	R_VA_Max = (((ModemRecievedDataCopy[48] - '0') *10000) + ((ModemRecievedDataCopy[49]-'0') * 1000) + ((ModemRecievedDataCopy[50]-'0') * 100) + ((ModemRecievedDataCopy[51]-'0') * 10)+ (ModemRecievedDataCopy[52]-'0')) ;    ;

	Tmp.Val 	= 	R_VA_Max;
	TMP[19] 	= 	Tmp.A;
	TMP[20] 	= 	Tmp.B;

	EEPROM_write(EEPROM_PAGE3, TMP, 21);
}

static void ParsePacket4(void)
{
	unsigned char tmp = 0;
	
	INT8U TMP[4] = {0};//Start EEPROM ADDRESS is C1(PAGE 4)
	INT8_16U Tmp;

	tmp 	= ((ModemRecievedDataCopy[4] - '0') *10) + ((ModemRecievedDataCopy[5]-'0'));
	SB 		= tmp / 7;																		//	SIGNAL STRENGTH
	Bat_V 	= ((ModemRecievedDataCopy[10] - '0') *10) + ((ModemRecievedDataCopy[11]-'0'));
	
	Tmp.Val = Y_VA_Max =   (((ModemRecievedDataCopy[19] - '0') *10000) + ((ModemRecievedDataCopy[20]-'0') * 1000) + ((ModemRecievedDataCopy[21]-'0') * 100) + ((ModemRecievedDataCopy[22]-'0') * 10)+ (ModemRecievedDataCopy[23]-'0'));    
	TMP[0] 	= Tmp.A;
	TMP[1] 	= Tmp.B;
	
	Tmp.Val = B_VA_Max =   (((ModemRecievedDataCopy[29] - '0') *10000) + ((ModemRecievedDataCopy[30]-'0') * 1000) + ((ModemRecievedDataCopy[31]-'0') * 100) + ((ModemRecievedDataCopy[32]-'0') * 10)+ (ModemRecievedDataCopy[33]-'0')) ;    
	TMP[2] 	= Tmp.A;
	TMP[3] 	= Tmp.B;
	
	Tmpr 	= ((ModemRecievedDataCopy[37] - '0') *10) + ((ModemRecievedDataCopy[38]-'0'));

	EEPROM_write(EEPROM_PAGE4, TMP, 4);	
//	EEPROM_ReadAll();
}
	
static void Tx2Modem(void)
{
	Sen.Val 	= 	0;
	Sen.R1 	= SR1;
	Sen.R2 	= SR2;
	Sen.R3 	= SR3;
	Sen.R4	= SR4;
	Sen.R5 	= SR5;
	
	Sen.Y1 	= SY1;
	Sen.Y2 	= SY2;
	Sen.Y3 	= SY3;
	Sen.Y4	= SY4;
	Sen.Y5 	= SY5;
	
	Sen.B1 	= SB1;
	Sen.B2 	= SB2;
	Sen.B3 	= SB3;
	Sen.B4	= SB4;
	Sen.B5 	= SB5;
	Sen.Dr 	= DR;
	SNRS 	= Sen.Val;
	
	if(!SR4)
	{
		R_D_IPV 	= 0;
		R_D_OPV 	= 0;
		R_D_Flags 	= 0;
		R_D_VA 		= 0;
	}
	if(!SY4)
	{
		
		Y_D_IPV 	= 0;
		Y_D_OPV 	= 0;
		Y_D_Flags 	= 0;
		Y_D_VA 		= 0;
	}	
	if(!SB4)	
	{
		B_D_IPV 	= 0;
		B_D_OPV 	= 0;
		B_D_Flags 	= 0;
		B_D_VA 		= 0;
	}
	
	if(R_VA_Max == 120)
		R_D_OPV = Ex_V1;
	if(Y_VA_Max == 120)
		Y_D_OPV = Ex_V2;
	if(B_VA_Max == 120)
		B_D_OPV = Ex_V3;

	Modem_PutString("<UART><TIMES><T1>");													//	START PACKET with < and T1
	Modem_PutInt(Sys_Time.TimeValue);
	Modem_PutString("</T1><T2>");
	Modem_PutInt(ONTime.TimeValue);
	Modem_PutString("</T2><T3>");
	Modem_PutInt(OFFTime.TimeValue);	
	Modem_PutString("</T3><T4>");
	Modem_PutInt(Y_ONTime.TimeValue);
	Modem_PutString("</T4><T5>");
	Modem_PutInt(Y_OFFTime.TimeValue);	
	Modem_PutString("</T5><T6>");
	Modem_PutInt(B_ONTime.TimeValue);
	Modem_PutString("</T6><T7>");
	Modem_PutInt(B_OFFTime.TimeValue);					
	Modem_PutString("</T7></TIMES><RP><R_EMV>");
	Modem_PutInt(Ex_V1);
	Modem_PutString("</R_EMV><R_DIV>");
	Modem_PutInt(R_D_IPV);
	Modem_PutString("</R_DIV><R_DOV>");
	Modem_PutInt(R_D_OPV);
	Modem_PutString("</R_DOV><R_AMP>");
	Modem_PutInt(Ex_A1);
	Modem_PutString("</R_AMP><R_EMPF>");
	Modem_PutInt(Ex_PF1);
	Modem_PutString("</R_EMPF><R_EM_KWH>");
	Modem_PutInt(Ex_KWH);	
	Modem_PutString("</R_EM_KWH><R_DFLAG>");
	Modem_PutInt(R_D_Flags);
	Modem_PutString("</R_DFLAG><R_VA>");
	Modem_PutInt(R_D_VA);
	Modem_PutString("</R_VA><R_SLOT>");
	Modem_PutInt(R_D_M_ExpV);
	Modem_PutString("</R_SLOT><R_VAC>");
	Modem_PutInt(R_VA_Max);
	Modem_PutString("</R_VAC></RP><YP><Y_EMV>");
	Modem_PutInt(Ex_V2);
	Modem_PutString("</Y_EMV><Y_DIV>");
	Modem_PutInt(Y_D_IPV);
	Modem_PutString("</Y_DIV><Y_DOV>");
	Modem_PutInt(Y_D_OPV);
	Modem_PutString("</Y_DOV><Y_AMP>");
	Modem_PutInt(Ex_A2);
	Modem_PutString("</Y_AMP><Y_EMPF>");
	Modem_PutInt(Ex_PF2);
	Modem_PutString("</Y_EMPF><Y_EM_KWH>");
	Modem_PutInt(Ex_KVARH);	
	Modem_PutString("</Y_EM_KWH><Y_DFLAG>");
	Modem_PutInt(Y_D_Flags);
	Modem_PutString("</Y_DFLAG><Y_VA>");
	Modem_PutInt(Y_D_VA);
	Modem_PutString("</Y_VA><Y_SLOT>");
	Modem_PutInt(Y_D_M_ExpV);	
	Modem_PutString("</Y_SLOT><Y_VAC>");
	Modem_PutInt(Y_VA_Max);
	Modem_PutString("</Y_VAC></YP><BP><B_EMV>");
	Modem_PutInt(Ex_V3);
	Modem_PutString("</B_EMV><B_DIV>");
	Modem_PutInt(B_D_IPV);
	Modem_PutString("</B_DIV><B_DOV>");
	Modem_PutInt(B_D_OPV);
	Modem_PutString("</B_DOV><B_AMP>");
	Modem_PutInt(Ex_A3);
	Modem_PutString("</B_AMP><B_EMPF>");
	Modem_PutInt(Ex_PF3);
	Modem_PutString("</B_EMPF><B_EM_KWH>");
	Modem_PutInt(Ex_KWH);	
	Modem_PutString("</B_EM_KWH><B_DFLAG>");
	Modem_PutInt(B_D_Flags);
	Modem_PutString("</B_DFLAG><B_VA>");
	Modem_PutInt(B_D_VA);
	Modem_PutString("</B_VA><B_SLOT>");
	Modem_PutInt(B_D_M_ExpV);
	Modem_PutString("</B_SLOT><B_VAC>");
	Modem_PutInt(B_VA_Max);	
	Modem_PutString("</B_VAC></BP><PS>");
	Modem_PutInt(PS);	
	Modem_PutString("</PS><SNRS>");													// Sensors Status 16 Bit Value
	Modem_PutInt(SNRS);
	Modem_PutString("</SNRS><VER>");													// PIC Software Version
	Modem_PutInt(SW_V);
	Modem_PutString("</VER><CD>");														// PIC Program Complie Date
	Modem_PutInt(CD);
	Modem_PutString("</CD><PWD>");													// Keypad Password
	Modem_PutChar(KP_PW[0]);
	Modem_PutChar(KP_PW[1] );
	Modem_PutChar(KP_PW[2]);
	Modem_PutChar(KP_PW[3]);

	Modem_PutString("</PWD><POR>");													// Power On Reset Count
	Modem_PutInt(POR);
	Modem_PutString("</POR><BOR>");													// Brown Out Reser Count
	Modem_PutInt(BOR);
	Modem_PutString("</BOR><WDT>");													// Watch Dog reset count
	Modem_PutInt(WDT);
	Modem_PutString("</WDT><HVC>");													// Low Voltage Reset Count		Modem_PutInt(LVDT);
	Modem_PutInt(HV_Cut);
	Modem_PutString("</HVC><LVC>");
	Modem_PutInt(LV_Cut);
	Modem_PutString("</LVC><VDT>");
	Modem_PutInt(LVDT);
	Modem_PutString("</VDT><VCC>");													// PIC Operating Voltage
	Modem_PutInt(Vcc);
	Modem_PutString("</VCC><T_KWH>");
	Modem_PutInt(Ex_KWH);	
	Modem_PutString("</T_KWH><T_KVARH>");
	Modem_PutInt(Ex_KVARH);	
	Modem_PutString("</T_KVARH><EM_SER>12345678</EM_SER><EM_NAME>12345678</EM_NAME><EM_ERR>12345678</EM_ERR>");
	Modem_PutString("<EM_DATE>123456</EM_DATE>");
	Modem_PutString("</UART>");														// END OF PACKET
}

void EEPROM_ReadAll(void)
{
	INT8U TMP1[8] = {0};  
	INT8U TMP2[16] = {0};  
	INT8U TMP3[24] = {0};  
	INT8U TMP4[8] = {0};  
	INT8_16U Tmp;
	
	EEPROM_read(EEPROM_PAGE1, TMP1, 6);
	
	ONTime.StructTime.Hrs 	= TMP1[0];
	ONTime.StructTime.Mins 	= TMP1[1];
	OFFTime.StructTime.Hrs 	= TMP1[2];
	OFFTime.StructTime.Mins = TMP1[3];
	PH3 = TMP1[4];
 	ALP = TMP1[5];
 
 #ifdef EPPROM_Debug
 	Debug_PutString("\n\r ON TIME - ");
 	Debug_PutInt(ONTime.StructTime.Hrs);
 	Debug_PutString(":");
 	Debug_PutInt(ONTime.StructTime.Mins);
 	Debug_PutString("\n\r OFF TIME - ");
 	Debug_PutInt(OFFTime.StructTime.Hrs);
 	Debug_PutString(":");
 	Debug_PutInt(OFFTime.StructTime.Mins);
 	Debug_PutString("\n\r PHASE - ");
 	Debug_PutInt(PH3);
 	Debug_PutString("\n\r ALP - ");
 	Debug_PutInt(ALP);
 	Debug_PutString("\n\r");
#endif
 /*
    EEPROM_read(EEPROM_PAGE2, TMP2, 8);
	
	Y_ONTime.StructTime.Hrs = TMP2[0];
	Y_ONTime.StructTime.Mins = TMP2[1];
	Y_OFFTime.StructTime.Hrs = TMP2[2];
	Y_OFFTime.StructTime.Mins = TMP2[3];
	B_ONTime.StructTime.Hrs = TMP2[4];
	B_ONTime.StructTime.Mins = TMP2[5];
	B_OFFTime.StructTime.Hrs = TMP2[6];
	B_OFFTime.StructTime.Mins = TMP2[7];
*/		
    EEPROM_read(EEPROM_PAGE2, TMP2, 12);
	
	Y_ONTime.StructTime.Hrs 	= TMP2[4];
	Y_ONTime.StructTime.Mins 	= TMP2[5];
	Y_OFFTime.StructTime.Hrs 	= TMP2[6];
	Y_OFFTime.StructTime.Mins 	= TMP2[7];
	B_ONTime.StructTime.Hrs 	= TMP2[8];
	B_ONTime.StructTime.Mins 	= TMP2[9];
	B_OFFTime.StructTime.Hrs 	= TMP2[10];
	B_OFFTime.StructTime.Mins 	= TMP2[11];
	
#ifdef EPPROM_Debug	
	Debug_PutString("\n\r Y ON TIME - ");
 	Debug_PutInt(Y_ONTime.StructTime.Hrs);
 	Debug_PutString(":");
 	Debug_PutInt(Y_ONTime.StructTime.Mins);
 	Debug_PutString("\n\r Y OFF TIME - ");
 	Debug_PutInt(Y_OFFTime.StructTime.Hrs);
 	Debug_PutString(":");
 	Debug_PutInt(Y_OFFTime.StructTime.Mins);
	Debug_PutString("\n\r B ON TIME - ");
 	Debug_PutInt(B_ONTime.StructTime.Hrs);
 	Debug_PutString(":");
 	Debug_PutInt(B_ONTime.StructTime.Mins);
 	Debug_PutString("\n\r B OFF TIME - ");
 	Debug_PutInt(B_OFFTime.StructTime.Hrs);
 	Debug_PutString(":");
 	Debug_PutInt(B_OFFTime.StructTime.Mins);
	Debug_PutString("\n\r");
#endif
	EEPROM_read(EEPROM_PAGE3, TMP3, 21);

	KP_PW[0] = TMP3[0];
	KP_PW[1] = TMP3[1];
	KP_PW[2] = TMP3[2];
	KP_PW[3] = TMP3[3];

	RD_Val 		= 	TMP3[4];
	RD_Bypass 	=	TMP3[5];
	YD_Val 		= 	TMP3[6];
	YD_Bypass 	=	TMP3[7];
	BD_Val 		= 	TMP3[8];
	BD_Bypass 	=	TMP3[9];

	Tmp.A 		= 	TMP3[10];
	Tmp.B 		= 	TMP3[11];
	R_HV_Cut 	= 	Tmp.Val;
	HV_Cut 		= 	Tmp.Val;
	
	Tmp.A 		= 	TMP3[12];
	Tmp.B 		= 	TMP3[13];
	Y_HV_Cut 	= 	Tmp.Val;
	
	Tmp.A 		= 	TMP3[14];
	Tmp.B 		= 	TMP3[15];
	B_HV_Cut 	= 	Tmp.Val;
	
	LV_Cut 		= 	TMP3[16];
	R_LV_Cut 	= 	TMP3[16];
	Y_LV_Cut 	= 	TMP3[17];
	B_LV_Cut 	= 	TMP3[18];

	Tmp.A 		= 	TMP3[19];
	Tmp.B 		= 	TMP3[20];	
	R_VA_Max 	= 	Tmp.Val;
	
#ifdef EPPROM_Debug
 	Debug_PutString("\n\r Password : ");
 	Debug_PutChar(KP_PW[0]);
	Debug_PutChar(KP_PW[1]);
	Debug_PutChar(KP_PW[2]);
	Debug_PutChar(KP_PW[3]);
	Debug_PutString("\n\r RDVAL : ");
	if(RD_Val == 'D' || RD_Val == 'B' )
		Debug_PutChar(RD_Val);
	else
		Debug_PutInt(RD_Val);
	Debug_PutString("\n\r RDByPass : ");
	Debug_PutInt(RD_Bypass);
	Debug_PutString("\n\r YDVAL : ");
	if(YD_Val == 'D' || YD_Val == 'B' )
		Debug_PutChar(YD_Val);
	else
		Debug_PutInt(YD_Val);
	Debug_PutString("\n\r YDByPass : ");
	Debug_PutInt(YD_Bypass);
	Debug_PutString("\n\r BDVAL : ");
	if(BD_Val == 'D' || BD_Val == 'B' )
		Debug_PutChar(BD_Val);
	else
		Debug_PutInt(BD_Val);
	Debug_PutString("\n\r BDByPass : ");
	Debug_PutInt(BD_Bypass);
	Debug_PutString("\n\r R - HV Cut : ");
	Debug_PutInt(R_HV_Cut);
	Debug_PutString("\n\r Y - HV Cut : ");
	Debug_PutInt(Y_HV_Cut);	
	Debug_PutString("\n\r B - HV Cut : ");
	Debug_PutInt(B_HV_Cut);
	Debug_PutString("\n\r R - LV Cut : ");
	Debug_PutInt(R_LV_Cut);
	Debug_PutString("\n\r Y - LV Cut : ");
	Debug_PutInt(Y_LV_Cut);	
	Debug_PutString("\n\r B - LV Cut : ");
	Debug_PutInt(B_LV_Cut);	
	Debug_PutString("\n\r R - VA Max : ");
	Debug_PutInt(R_VA_Max);	
#endif	
	
	EEPROM_read(EEPROM_PAGE4, TMP4, 4);	
	Tmp.A = TMP4[0];
	Tmp.B = TMP4[1];	
	Y_VA_Max = Tmp.Val;
#ifdef EPPROM_Debug
	Debug_PutString("\n\r Y - VA Max : ");
	Debug_PutInt(Y_VA_Max);	
#endif
	Tmp.A = TMP4[2];
	Tmp.B = TMP4[3];	
	B_VA_Max = Tmp.Val;
#ifdef EPPROM_Debug
	Debug_PutString("\n\r B - VA Max : ");
	Debug_PutInt(B_VA_Max);	
#endif


}


static void Slot_LEDS(void)
{
	if(RD_Val == 'D')																			//	Time Based Dimming
	{
		R_CSLOT = Dim_Slots();	
		if(R_CSLOT >= Min_Dim_OP_Volt && R_CSLOT <= Max_Dim_OP_Volt)
			R_D_M_ExpV = R_CSLOT;				 
		else 
			R_D_M_ExpV =  Default_OP_Volt;	
	}
	else if(RD_Val != 'D' && RD_Val != 'B' )													//	Server Controlled Output Voltage	
	{
		if(RD_Val >= Min_Dim_OP_Volt && RD_Val <= Max_Dim_OP_Volt)
			R_D_M_ExpV = RD_Val;
		else 
			R_D_M_ExpV = Default_OP_Volt;
	}
	else
			R_D_M_ExpV = Default_OP_Volt;
	return	R_D_M_ExpV;			

}