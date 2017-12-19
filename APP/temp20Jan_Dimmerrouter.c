#include <includes.h>


static INT16U 	Smooth_Dim(INT16U Rd_OPV, INT16U Exp_OPV);
INT8U 	R_CSLOT = 0, Y_CSLOT = 0, B_CSLOT = 0;
extern INT8U 	RD_Bypass, YD_Bypass, BD_Bypass	;
static INT8U 	R_Dim(void);
static INT8U 	Y_Dim(void);
static INT8U 	B_Dim(void);
static void 	Clear_Rx_Data(void);
static void 	Capture_Rx_Data(void);
static void 	Tx_Dimm_Cmds(void);
static void 	Tx_Dimm_Rd_Cmds(void);
static void 	Tx_Dimm_Wt_OpV_Cmds(void);
static void 	Tx_Dimm_Wt_Cmds(void);
static void 	Cmp_Rd_Cmds_Data(void);
static void 	Tx_Dimm_Wt_All_Cmds(void);
static INT16U 	RYB_VA(void);
static INT16U 	RYB_HVC(void);
static INT8U 	RYB_LVC(void);

void 	Debug_Dimmer(void);

extern INT8U	Task_WDT[5];										//	Definition in app_cfg.c
extern INT16U 	HV_Cut, R_HV_Cut, Y_HV_Cut, B_HV_Cut;
extern INT8U 	LV_Cut, R_LV_Cut, Y_LV_Cut, B_LV_Cut;
extern INT16U 	VA_Max, R_VA_Max, Y_VA_Max, B_VA_Max;

extern BOOL PH3;													//	Variable to represent 1PH(0) or 3PH(1) SYSTEM

BOOL En_Wt_OpV_Cmd 	 	= FALSE;									// flag for Enabling the write to OP Voltage
BOOL En_Wt_Cmd		 	= FALSE;									// flag for Enabling the write to Dimmer HV LV and VA
BOOL En_Wt_All_Cmd	 	= FALSE;									// flag for Enabling the write to Dimmer OP Voltage, HV_Cut, LV_Cut and VA
BOOL En_Cmp_Rx_Data  	= FALSE;									// flag invoke the compare function 
BOOL Set_Skip_Cycle  	= FALSE;
BOOL Dimm_Comm_En_Flag 	= TRUE;									//	Flag to Disable Dimmer Communication By default
BOOL RW_Mode			= Rd_DATA;									//	To avoid Caputuring data while configuring Dimmers(Dimm_R_Cmd_Indx is Incremented in Capture function) 

INT8U NO_Resp_Cnt = 0;												// Counter to count No Responce sequence from Dimmer	

void AppDimmCommTask (void *p_arg)
{
	INT8S * rxMessage;
	INT8U *err;
	INT8U Dim_Comm_En_Cnt = 0;


	Dimm_Comm_Init();

	DDR_EN_PHASE_R = 0; 	// Config the pin as Output	
	DDR_EN_PHASE_Y = 0; 	// Config the pin as Output	
	DDR_EN_PHASE_B = 0; 	// Config the pin as Output

	EN_PHASE_R = 1;
	Nop();
	Nop();
	EN_PHASE_Y = 0;
	Nop();
	Nop();
	EN_PHASE_B = 0;
	Nop();
	Nop();
   	while(1)
    { 
		if(++Dim_Comm_En_Cnt >= DIMM_COMM_ENABLE_TIME)
		{
			Dim_Comm_En_Cnt = 0;
			Dimm_Comm_En_Flag = TRUE;
			Dimm_R_Cmd_Indx = 0;														//	Read Cmd Intex to Zero
			En_Cmp_Rx_Data = FALSE;														//	To Send Read Register commands 			RW_Mode			=	Rd_DATA;
			EN_PHASE_R = 1;
			Nop();
			Nop();
			EN_PHASE_Y = 0;
			Nop();
			Nop();
			EN_PHASE_B = 0;	
			Nop();
			Nop();
			#ifdef	P_Debug
				Debug_PutString("\n\rPhase - R\n\r");
			#endif
		}  
		if(Dimm_Comm_En_Flag == TRUE)
		{
			Tx_Dimm_Cmds();	
		}
		Task_WDT[2] = 1;				//	DIMMER Communication
		rxMessage 	= (INT8S*)OSMboxPend(UART4MboxPtr, 9000, err);			
		if(Dimm_Comm_En_Flag == TRUE)
		{		
			if(RW_Mode == Rd_DATA)
				Capture_Rx_Data();
			if(NO_Resp_Cnt > NO_Resp_Seq )									//	To Break Dimmer Communication after a sequence of Trails 
				Dimm_Comm_En_Flag = FALSE;									// 	Disable Dimmer Communication			
			else 
			{
				Clear_Rx_Data();											//	Clear Received data during Write Operations	
			}							
		}
		else																//	Clear Received data during Dimmer Communication Disabled	
		{
			Clear_Rx_Data();
			Debug_PutString("\n\rCnt:");
			Dimm_Comm_PutInt(Dim_Comm_En_Cnt);
			Debug_PutChar(' ');
		}	    	
	}																		// 	END WHILE																																											// end of while(1)	
}																			// 	END of task


static void	Tx_Dimm_Cmds(void)
{
	if(!En_Cmp_Rx_Data)
	{
		Tx_Dimm_Rd_Cmds();
	}
	if(En_Cmp_Rx_Data)
	{
		if((En_Wt_OpV_Cmd == FALSE) && (En_Wt_Cmd == FALSE))
			Cmp_Rd_Cmds_Data();													//	Compare Dimmer Values with Existing Vaules
		if(Set_Skip_Cycle == FALSE)
		{		
			if( (En_Wt_All_Cmd ==	TRUE) || ((En_Wt_OpV_Cmd == TRUE) && (En_Wt_Cmd == TRUE)) )
			{
				RW_Mode =	Wt_DATA;
				Tx_Dimm_Wt_All_Cmds();												//	Configure All Parameters in a sequence
			}
			else if( En_Wt_OpV_Cmd )
			{
				RW_Mode =	Wt_DATA;												//	Configure Output Voltage Only
				Tx_Dimm_Wt_OpV_Cmds();
			}			
				
			else if( En_Wt_Cmd )
			{
				RW_Mode =	Wt_DATA;												
				Tx_Dimm_Wt_Cmds();													//	COnfigure VA, HV, LV Only
			}					
			else
			{				
				Dimm_Comm_En_Flag = FALSE;											//	DISABLE DIMMER COMMUICATION
			}
				
		}
		else 
		{
			Set_Skip_Cycle = FALSE;          // Skip 1 Cycle
		}
		if((En_Wt_OpV_Cmd == FALSE) && (En_Wt_Cmd == FALSE))
			Dimm_Comm_En_Flag = FALSE;											//	DISABLE DIMMER COMMUICATION
		
	}
}
static void Tx_Dimm_Wt_All_Cmds(void)
{
	INT16U	Data2Dim	=	0;
	INT16U	VA_Phase	= 	0;
	INT16U	HVC			=	0;
	INT8U	LVC			=	0;
	if(Dimm_W_All_Cmd_Indx >= MAX_WT_ALL_CMD_INDX)         
	{
		En_Wt_OpV_Cmd 		= FALSE;
		En_Wt_Cmd     		= FALSE;
		En_Cmp_Rx_Data 		= FALSE;
		En_Wt_All_Cmd		= FALSE;
		Dimm_Comm_En_Flag 	= FALSE;														//	Cycle Complete Disable Dimm_Comm
		Dimm_W_All_Cmd_Indx = 0;
	}
	if(En_Wt_All_Cmd ==	TRUE )
	{
		Dimm_Comm_PutString((INT8S *)Dimm_W_All_Cmds[Dimm_W_All_Cmd_Indx]);
		if(Dimm_W_All_Cmd_Indx == 1)
		{
			if(EN_PHASE_R)
				Data2Dim	=	(INT16U)R_Dim()	;
			else if(EN_PHASE_Y)
				Data2Dim	=	(INT16U)Y_Dim()	;
			else if(EN_PHASE_B)
				Data2Dim	=	(INT16U)B_Dim()	;	
			else 
				Data2Dim	=	(INT16)Default_OP_Volt;
			if(Data2Dim >= Min_Dim_OP_Volt && Data2Dim <= Max_Dim_OP_Volt)	
			 	Dimm_Comm_PutInt(Data2Dim * 100);
			else 
				Dimm_Comm_PutInt((Default_OP_Volt * 100));									// configure default OP voltage
			Dimm_Comm_PutChar('\r');	
		}
		if(Dimm_W_All_Cmd_Indx == 2)
		{
			VA_Phase	=RYB_VA();
			Dimm_Comm_PutInt(VA_Phase);														// configure default Maximum VA
			Dimm_Comm_PutChar('\r');
		}
		if(Dimm_W_All_Cmd_Indx == 3)
		{
			HVC	=	RYB_HVC();
			Dimm_Comm_PutInt(HVC * 100);									// configure default HV_Cut Off voltage
			Dimm_Comm_PutChar('\r');	
		}
		if(Dimm_W_All_Cmd_Indx == 4)
		{
			LVC	=	RYB_LVC();
			Dimm_Comm_PutInt(LVC * 100);									// configure default LV_Cut Off voltage
			Dimm_Comm_PutChar('\r');
		}
		
		Dimm_W_All_Cmd_Indx++;
	}	
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                  Changes Dimmer Output Voltage
*
* Function 		:static void Tx_Dimm_Wt_OpV_Cmds(void)
*
* Description	: This function will change the Dimmer Output Voltage
*
* Arguments  	: void
*
* Returns    	: void
*
* Note(s)    	: 	1) Transmit Dimm_W_OpV_Cmd and Increment Index only when En_Wt_OpV_Cmd is Enabled 
*
*              		2) En_Wt_OpV_Cmd is Enabled in Cmp_Rd_Cmds_Data()
*
*              		3) One Cycle is completed when Dimm_W_OpV_Cmd_Indx reaches end of the Buffer.
*
*					4) When One Cycle is completed Dimmer Communication should be disabled.	 
*                 
*********************************************************************************************************
*/



static void Tx_Dimm_Wt_OpV_Cmds(void)
{
	INT8U Data2Dim	= 0;
	if(Dimm_W_OpV_Cmd_Indx >= MAX_WT_OPV_CMD_INDX)         
	{
		En_Cmp_Rx_Data = FALSE;
		En_Wt_OpV_Cmd = FALSE;
		Dimm_Comm_En_Flag = FALSE;														//	Cycle Complete Disable Dimm_Comm
		Dimm_W_OpV_Cmd_Indx = 0;
	}
	if(En_Wt_OpV_Cmd == TRUE)
	{
		Dimm_Comm_PutString((INT8S *)Dimm_W_OpV_Cmd[Dimm_W_OpV_Cmd_Indx]);
		if(Dimm_W_OpV_Cmd_Indx == 1)
		{
			if(EN_PHASE_R)
				Data2Dim	=	R_Dim()	;
			else if(EN_PHASE_Y)
				Data2Dim	=	Y_Dim()	;
			else if(EN_PHASE_B)
				Data2Dim	=	B_Dim()	;	
			else 
				Data2Dim	=	Default_OP_Volt;
			if(Data2Dim >= Min_Dim_OP_Volt && Data2Dim <= Max_Dim_OP_Volt)	
			 	Dimm_Comm_PutInt(Data2Dim * 100);
			else 
				Dimm_Comm_PutInt((Default_OP_Volt * 100));									// configure default OP voltage
			Dimm_Comm_PutChar('\r');	
		}
		Dimm_W_OpV_Cmd_Indx++;
	}
}

static void Tx_Dimm_Wt_Cmds(void)
{
	INT16U	VA_Phase	= 	0;
	INT16U	HVC			=	0;
	INT8U	LVC			=	0;
	if(Dimm_W_Cmd_Indx >= MAX_WT_CMD_INDX)
	{
		En_Cmp_Rx_Data = FALSE;
		En_Wt_Cmd     = FALSE;
		Dimm_Comm_En_Flag = FALSE;														//	Cycle Complete Disable Dimm_Comm
		Dimm_W_Cmd_Indx = 0;
	}
	if(En_Wt_Cmd == TRUE)
	{
		Dimm_Comm_PutString((INT8S *)Dimm_W_Cmd[Dimm_W_Cmd_Indx]);
		if(Dimm_W_Cmd_Indx == 1)
		{
			VA_Phase	= RYB_VA();
			Dimm_Comm_PutInt(VA_Phase);											// configure default Maximum VA
			Dimm_Comm_PutChar('\r');
		}
		if(Dimm_W_Cmd_Indx == 2)
		{
			HVC	=	RYB_HVC();
			Dimm_Comm_PutInt(HVC * 100);									// configure default HV_Cut Off voltage
			Dimm_Comm_PutChar('\r');	
		}
		if(Dimm_W_Cmd_Indx == 3)
		{
			LVC	=	RYB_LVC();
			Dimm_Comm_PutInt(LVC * 100);									// configure default LV_Cut Off voltage
			Dimm_Comm_PutChar('\r');
		}	
		Dimm_W_Cmd_Indx++;	
	}	
}
																						
static void Tx_Dimm_Rd_Cmds(void)		// This function will transmit read commands from dimmer
{
	// Dimm_R_Cmd_Indx will be incremented in Capture_Rx_Data function only when there is data recieved for the prevoius command
	if(Dimm_R_Cmd_Indx > (INT16S)D_QUIT)
	{
		Dimm_R_Cmd_Indx = 0;
		En_Cmp_Rx_Data = TRUE;
	}
	Dimm_Comm_PutString((INT8S *)Dimm_R_Cmd[Dimm_R_Cmd_Indx]);	
}

static void Cmp_Rd_Cmds_Data(void)
{
	if(PH3)
	{
		if( (R_D_ExpV != R_D_M_ExpV) || (Y_D_ExpV != Y_D_M_ExpV) || (B_D_ExpV != B_D_M_ExpV) )
		{
			En_Wt_OpV_Cmd = TRUE;
		}	
		else
		{
			En_Wt_OpV_Cmd = FALSE;		
		}				    	
		if( (R_D_Max_VA != R_VA_Max) || ( R_D_HV_Cut != R_HV_Cut) || ( R_D_LV_Cut != R_LV_Cut) || \
			(Y_D_Max_VA != Y_VA_Max) || ( Y_D_HV_Cut != Y_HV_Cut) || ( Y_D_LV_Cut != Y_LV_Cut) || \
			(B_D_Max_VA != B_VA_Max) || ( B_D_HV_Cut != B_HV_Cut) || ( B_D_LV_Cut != B_LV_Cut) )
		{
			En_Wt_Cmd = TRUE;
		}		    	   
		else
		{
			En_Wt_Cmd = FALSE;
		}
		if( (En_Wt_OpV_Cmd == TRUE) && (En_Wt_Cmd == TRUE) )
			En_Wt_All_Cmd	=	TRUE;
		else
			En_Wt_All_Cmd	=	FALSE;
	}
	else
	{
		Debug_Dimmer();
		if(R_D_ExpV != R_D_M_ExpV) 
		{
			En_Wt_OpV_Cmd = TRUE;
		}	
		else
		{
			En_Wt_OpV_Cmd = FALSE;		
		}				    	
		if( (R_D_Max_VA != R_VA_Max) || ( R_D_HV_Cut != R_D_M_HV_Cut) || ( R_D_LV_Cut != R_D_M_LV_Cut))	
		{
			En_Wt_Cmd = TRUE;
		}		    	   
		else
		{
			En_Wt_Cmd = FALSE;
		}
		if( (En_Wt_OpV_Cmd == TRUE) && (En_Wt_Cmd == TRUE) )
			En_Wt_All_Cmd	=	TRUE;
		else
			En_Wt_All_Cmd	=	FALSE;	
	}
	Set_Skip_Cycle = TRUE;

}

static void Capture_Rx_Data(void)
{
	INT16S Rx_Data_Len = Read_Dimmer_Data();	
	if(Rx_Data_Len>0)
	{
		Parse_Dimmer_Data(Dimm_R_Cmd_Indx, Rx_Data_Len);
		Dimm_R_Cmd_Indx++;
		Rx_Data_Len = 0;
	}
}

static void Clear_Rx_Data(void)
{
	INT16S Rx_Data_Len = 0;
	Rx_Data_Len = Read_Dimmer_Data();
	if(Rx_Data_Len>0)
		Read_Dimm_Resp(RecievedData, Rx_Data_Len);	
}

void Parse_Dimmer_Data(INT16S Cmd_Indx, INT16S len)
{
	INT32U  Rx_Dimm_Data;

	switch(Cmd_Indx)
	{
		case D_Z:														//	Read Responce for cmd 'Z'.
			#ifdef	P_Debug
				Debug_PutString("Z :");
			#endif
			Read_Dimm_Resp(RecievedData, len);
			break;
		case D_TEMP:													//	Read Temperature from Dimmer
			#ifdef	P_Debug
				Debug_PutString("temp :");
			#endif
			Rx_Dimm_Data = Read_Dimm_Resp(RecievedData, len);
			if(EN_PHASE_R)
				R_D_Temp = Rx_Dimm_Data;								//	Phase-R Temperature
			else if(EN_PHASE_Y)
				Y_D_Temp = Rx_Dimm_Data;								//	Phase-Y Temperature	
			else if(EN_PHASE_B)
				B_D_Temp = Rx_Dimm_Data;								//	Phase-B Temperature
			break;
		case D_IN_V:													//	Read Input Voltage from Dimmer
			#ifdef	P_Debug
				Debug_PutString("ip volt :");
			#endif
			Rx_Dimm_Data = Read_Dimm_Resp(RecievedData, len)/100;
			if(EN_PHASE_R)
				R_D_IPV = Rx_Dimm_Data;									//	Phase-R Input Voltage
			else if(EN_PHASE_Y)
				Y_D_IPV = Rx_Dimm_Data;									//	Phase-Y Input Voltage
			else if(EN_PHASE_B)
				B_D_IPV = Rx_Dimm_Data;									//	Phase-B Input Voltage
			break;
		case D_OP_V:													//	Read Output Voltage from Dimmer			
			#ifdef	P_Debug
				Debug_PutString("Op volt :");
			#endif
			Rx_Dimm_Data = Read_Dimm_Resp(RecievedData, len)/100;
			if(EN_PHASE_R)
				R_D_OPV = Rx_Dimm_Data;									//	Phase-R Output Voltage	
			else if(EN_PHASE_Y)
				Y_D_OPV = Rx_Dimm_Data;									//	Phase-Y Output Voltage	
			else if(EN_PHASE_B)
				B_D_OPV = Rx_Dimm_Data;									//	Phase-B Output Voltage	
		    break;
		case D_LOAD:													//	Read Load in VA from Dimmer
			#ifdef	P_Debug
				Debug_PutString("Load :");
			#endif
			Rx_Dimm_Data = Read_Dimm_Resp(RecievedData, len);
			if(EN_PHASE_R)
				R_D_VA = Rx_Dimm_Data;									//	Phase-R VA	
			else if(EN_PHASE_Y)
				Y_D_VA = Rx_Dimm_Data;									//	Phase-Y VA
			else if(EN_PHASE_B)
				B_D_VA = Rx_Dimm_Data;									//	Phase-B VA
			break;
		case D_FREQ:													//	Read Frequency from Dimmer
			#ifdef	P_Debug
			 	Debug_PutString("Hz :");
			 #endif
			 Rx_Dimm_Data = Read_Dimm_Resp(RecievedData, len)/1000;
			if(EN_PHASE_R)
				R_D_Freq = Rx_Dimm_Data;								//	Phase-R Frequency			
			else if(EN_PHASE_Y)
				Y_D_Freq = Rx_Dimm_Data;								//	Phase-Y Frequency	
			else if(EN_PHASE_B)
				B_D_Freq = Rx_Dimm_Data;								//	Phase-B Frequency
			break;
		case D_FLAGS:													//	Read 16 Bit flag status from Dimmer	
			#ifdef	P_Debug
			 	Debug_PutString("Flags :");
			 #endif
			 Rx_Dimm_Data = Read_Dimm_Resp(RecievedData, len);
			if(EN_PHASE_R)
				R_D_Flags = Rx_Dimm_Data;								//	Phase-R 16 bit Flags	
			else if(EN_PHASE_Y)
				Y_D_Flags = Rx_Dimm_Data;								//	Phase-Y 16 bit Flags	
			else if(EN_PHASE_B)
				B_D_Flags = Rx_Dimm_Data;								//	Phase-B 16 bit Flags

			break;
		case D_S_OP_V:													//	Read Output Voltage Set value from Dimmer
			#ifdef	P_Debug
				Debug_PutString("S OP Volts:");
			#endif
			Rx_Dimm_Data = Read_Dimm_Resp(RecievedData, len)/100;
			if(EN_PHASE_R)
				R_D_ExpV = Rx_Dimm_Data;								//	Phase-R Dimmer Output Set Voltage Value
			else if(EN_PHASE_Y)
				Y_D_ExpV = Rx_Dimm_Data;								//	Phase-Y Dimmer Output Set Voltage Value
			else if(EN_PHASE_B)
				B_D_ExpV = Rx_Dimm_Data;								//	Phase-B Dimmer Output Set Voltage Value
			break;
		case D_S_MAX_LOAD:												//	Read Max_load in VA set value from Dimmer
			#ifdef	P_Debug
				Debug_PutString("Max Load:");
			#endif
			Rx_Dimm_Data = Read_Dimm_Resp(RecievedData, len);
			if(EN_PHASE_R)
				R_D_Max_VA = Rx_Dimm_Data;
			else if(EN_PHASE_Y)
				Y_D_Max_VA = Rx_Dimm_Data;
			else if(EN_PHASE_B)
				B_D_Max_VA = Rx_Dimm_Data;
			break;
		case D_S_HV_CUT:												//	Read HV_Cut set value from Dimmer
			#ifdef	P_Debug
			 	Debug_PutString("HV Cut:");
			 #endif
			 Rx_Dimm_Data = Read_Dimm_Resp(RecievedData, len)/100;
			if(EN_PHASE_R)
				R_D_HV_Cut = Rx_Dimm_Data;
			else if(EN_PHASE_Y)
				Y_D_HV_Cut = Rx_Dimm_Data;
			else if(EN_PHASE_B)
				B_D_HV_Cut = Rx_Dimm_Data;
			break;
		case D_S_LV_CUT:
			#ifdef	P_Debug
				Debug_PutString("LV Cut:");
			#endif													//	Read LV_Cut set value from Dimmer w.r.t Phase
			
			Rx_Dimm_Data = Read_Dimm_Resp(RecievedData, len)/100;
			if(EN_PHASE_R)
				R_D_LV_Cut = Rx_Dimm_Data;
			else if(EN_PHASE_Y)
				Y_D_LV_Cut = Rx_Dimm_Data;
			else if(EN_PHASE_B)
				B_D_LV_Cut = Rx_Dimm_Data;
			break;
		case D_QUIT:												//	Read responce for Quit cmd
			Read_Dimm_Resp(RecievedData, len);	
			break;
		
		default:
				NO_Resp_Cnt++;										//	No Responce Counter	
			#ifdef	P_Debug
				Dimm_Comm_PutString("No Resp :");
			#endif	
			break;				
	}	
}	



// This function converts the String responce from dimmer to integer value.
INT32U Read_Dimm_Resp(INT8U * DataPtr, INT16 len)
{
	INT8U i = 0;
	INT8U data;
	INT32U DimmValue = 0;
	while(len > 0 )
	{
		 data = DataPtr[i];
		 if((data >= 48) && (data <= 57))								//	Capture Numeric Values Only
		 {	
			 // strings to integer conversion	 
			 DimmValue = (INT32U)(DimmValue * 10) + (data-'0'); 
		 } 
		 i++;
		 len--;
	}
	#ifdef	P_Debug
		Debug_PutInt(DimmValue);
		Debug_PutString("\n\r");
	#endif
	return DimmValue;
}	


static INT8U R_Dim(void)
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

static INT8U Y_Dim(void)
{
	if(YD_Val == 'D')																			//	Time Based Dimming
    {
	    Y_CSLOT = Dim_Slots();
		if(Y_CSLOT >= Min_Dim_OP_Volt && Y_CSLOT <= Max_Dim_OP_Volt)
			Y_D_M_ExpV = Y_CSLOT;				 
	    	else 
	    		Y_D_M_ExpV =  Default_OP_Volt;	
    }
    else if(YD_Val != 'D' && YD_Val != 'B' )													//	Server Controlled Output Voltage
    {
	   	if(YD_Val >= Min_Dim_OP_Volt && YD_Val <= Max_Dim_OP_Volt)
	   		Y_D_M_ExpV = YD_Val;
	    	else 
	    		Y_D_M_ExpV = Default_OP_Volt;
    }
    else
    	Y_D_M_ExpV = Default_OP_Volt;	
    return	Y_D_M_ExpV;		
}

static INT8U B_Dim(void)
{	
	if(BD_Val == 'D')																			//	Time Based Dimming
    {
	    B_CSLOT = Dim_Slots();
		if(B_CSLOT >= Min_Dim_OP_Volt && B_CSLOT <= Max_Dim_OP_Volt)
			B_D_M_ExpV = B_CSLOT;				 
		else 
	   		B_D_M_ExpV =  Default_OP_Volt;		
    }
    else if(BD_Val != 'D' && BD_Val != 'B' )													//	Server Controlled Output Voltage
   	{
		if(BD_Val >= Min_Dim_OP_Volt && BD_Val <= Max_Dim_OP_Volt)
			B_D_M_ExpV = BD_Val;
		else 
	    		B_D_M_ExpV = Default_OP_Volt;
    }
    else 
    	B_D_M_ExpV = Default_OP_Volt;	
    return	B_D_M_ExpV;		
}


static INT16U Smooth_Dim(INT16U Rd_OPV, INT16U Exp_OPV)											//	Regulate Output Voltage Smoothly
{
	INT16U S_Dimm_Value = 0;
	if( Rd_OPV > Exp_OPV )																		//	Decrease Ouput Voltage if Existing Output is greater than expected		
	{
		if( (Rd_OPV - Exp_OPV) < Adj_Volts)														//	Difference BTN Output Voltage and Expected Voltage is less than Smooth Dim Value
			S_Dimm_Value  = Exp_OPV;
		else 																					//	Decrease Adjustable value to the Output
			S_Dimm_Value  = Rd_OPV - Adj_Volts;
	}
	else if( Rd_OPV < Exp_OPV )																	//	Increase Ouput Voltage if Existing Output is less than expected
	{
		if( (Exp_OPV - Rd_OPV ) < Adj_Volts)													//	Difference BTN Output Voltage and Expected Voltage is less than Smooth Dim Value
			S_Dimm_Value  = Exp_OPV;
		S_Dimm_Value  = Rd_OPV + Adj_Volts;														//	Increase Adjustable value to the Output
	}
	else 
		S_Dimm_Value  = Exp_OPV;
	if( S_Dimm_Value >= Min_Dim_OP_Volt && S_Dimm_Value <= Max_Dim_OP_Volt) 					//	Buck_Boost Voltage value should be always between Minimum and maximum Output Voltage.	
		return S_Dimm_Value;
	else 
	{
		S_Dimm_Value = Default_OP_Volt;															//	Default Output Voltage							
		return S_Dimm_Value;
	}
}

static INT16U RYB_VA(void)
{
	if(EN_PHASE_R)
	{	
		if(R_VA_Max < R_D_M_Max_VA)
			return R_VA_Max;
		else
			return R_D_M_Max_VA;
	}   	
	else if(EN_PHASE_Y)
	{
		if(Y_VA_Max < Y_D_M_Max_VA)
			return Y_VA_Max;
		else
			return Y_D_M_Max_VA;
	}					    			
	else if(EN_PHASE_B)
	{
		if(B_VA_Max < B_D_M_Max_VA)
			return B_VA_Max;
		else
			return B_D_M_Max_VA;
	}					    			
}

static INT16U RYB_HVC(void)
{
	if(EN_PHASE_R)
	{
		if(R_HV_Cut > HV_Cut_Min_Range && R_HV_Cut < HV_Cut_Max_Range)							//	HV CUT RANGE
			return R_HV_Cut;
		else
			return Default_HV_Cut;	
	}	
	else if(EN_PHASE_Y)
	{
		if(Y_HV_Cut > HV_Cut_Min_Range && Y_HV_Cut < HV_Cut_Max_Range)							//	HV CUT RANGE
			return Y_HV_Cut;
		else
			return Default_HV_Cut;	
	}
	else if(EN_PHASE_B)
	{
		if(B_HV_Cut > HV_Cut_Min_Range && B_HV_Cut < HV_Cut_Max_Range)							//	HV CUT RANGE
			return B_HV_Cut;
		else
			return Default_HV_Cut;		    		
	}
}

static INT8U RYB_LVC(void)
{
	if(EN_PHASE_R)
	{
		if(R_LV_Cut > LV_Cut_Min_Range && R_LV_Cut < LV_Cut_Max_Range)							//	LV CUT RANGE
			return R_LV_Cut;
		else
			return Default_LV_Cut;	
	}	
	else if(EN_PHASE_Y)
	{
		if(Y_LV_Cut > LV_Cut_Min_Range && Y_LV_Cut < LV_Cut_Max_Range)							//	LV CUT RANGE
			return Y_LV_Cut;
		else
			return Default_LV_Cut;	
	}
	else if(EN_PHASE_B)
	{
		if(B_LV_Cut > LV_Cut_Min_Range && B_LV_Cut < LV_Cut_Max_Range)							//	LV CUT RANGE
			return B_LV_Cut;
		else
			return Default_LV_Cut;		    		
	}
}


void Debug_Dimmer(void)
{
	Debug_PutString("\n\rR_D_ExpV \t:");
	Debug_PutInt(R_D_ExpV);	
	Debug_PutString(" R_D_M_ExpV \t:");
	Debug_PutInt(R_D_M_ExpV);
	
	Debug_PutString("\n\rR_D_Max_VA \t:");
	Debug_PutInt(R_D_Max_VA);	
	Debug_PutString(" R_VA_Max \t:");
	Debug_PutInt(R_VA_Max);
	
	Debug_PutString("\n\rR_D_HV_Cut \t:");
	Debug_PutInt(R_D_HV_Cut);	
	Debug_PutString(" R_D_M_HV_Cut \t:");
	Debug_PutInt(R_D_M_HV_Cut);
	
	Debug_PutString("\n\rR_D_LV_Cut \t:");
	Debug_PutInt(R_D_LV_Cut);	
	Debug_PutString(" R_D_M_LV_Cut \t:");
	Debug_PutInt(R_D_M_LV_Cut);
	Debug_PutString("\n\r");		
}		

