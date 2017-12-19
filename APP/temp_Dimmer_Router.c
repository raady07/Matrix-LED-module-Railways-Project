#include <includes.h>

static INT16U Smooth_Dim(INT16U Rd_OPV, INT16U Exp_OPV);
extern INT8U R_CSLOT, Y_CSLOT, B_CSLOT;
extern INT8U RD_Bypass, YD_Bypass, BD_Bypass	;
static void R_Dim(void);
static void Y_Dim(void);
static void B_Dim(void);


extern INT16U HV_Cut, R_HV_Cut, Y_HV_Cut, B_HV_Cut;
extern INT8U LV_Cut, R_LV_Cut, Y_LV_Cut, B_LV_Cut;
extern INT16U VA_Max, R_VA_Max, Y_VA_Max, B_VA_Max;

extern BOOL PH3;										//	Variable to represent 1PH(0) or 3PH(1) SYSTEM
BOOL R_W_Mode = READ_MODE;								//	READ Commands

void AppDimmCommTask (void *p_arg)
{
	INT8S * rxMessage;
	INT8U *err;
	INT8U WaitForTx = TRUE, WaitCnt = 0, Dimm_Comm_En_Flag = TRUE;

	INT16S Rx_Data_Len = 0;
    INT16U Dim_Comm_En_Cnt = 0;
    INT16U S_DimmValue = 0;
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
		if(EN_PHASE_R || EN_PHASE_Y || EN_PHASE_B)											//	Send Data only when any one of the EN-RYB are Enabled
		{			    			
	    	if(Dimm_Comm_En_Flag)
		    {
		    	if(WaitForTx == TRUE)
		    	{
			    	WaitForTx = FALSE;
			    	if(R_W_Mode == READ_MODE)												//	Send Read Mode Cmds
			    	{
				    	Dimm_Comm_PutString((INT8S * )Dimm_R_Cmd[Dimm_R_Cmd_Indx]);
		   	 		}
		    		else																	// send write mode cmds				
		    		{
			    		if(D_Ch_ExpV == TRUE)    											// If set op voltage is not equal exp op voltage
			    		{
				    		Dimm_Comm_PutString((INT8S * )Dimm_W_OpV_Cmd[Dimm_W_OpV_Cmd_Indx]);			    		
				    		if(Dimm_W_OpV_Cmd_Indx == 1) 
				    		{
					    		#ifdef	P_Debug
						    		Debug_PutString("\n\r R_CSLOT :");
									Debug_PutInt(R_D_M_ExpV);
									Debug_PutString("\n\r");	
									Debug_PutString("\n\r Y_CSLOT :");
									Debug_PutInt(Y_D_M_ExpV);
									Debug_PutString("\n\r");	
									Debug_PutString("\n\r B_CSLOT :");
									Debug_PutInt(B_D_M_ExpV);
									Debug_PutString("\n\r");				    		
								#endif
								if(EN_PHASE_R)
					    		{
						    	//	S_DimmValue = Smooth_Dim(R_D_ExpV,R_D_M_ExpV);
						      	//	Dimm_Comm_PutInt((S_DimmValue * 100));
						      		Dimm_Comm_PutInt((220 * 100));
					    		}					    			
					    		else if(EN_PHASE_Y)
					    		{
						    	//	S_DimmValue = Smooth_Dim(Y_D_ExpV,Y_D_M_ExpV);
					    		//	Dimm_Comm_PutInt((S_DimmValue  * 100));
					    		Dimm_Comm_PutInt((220 * 100));
					    		}
					    			
					    		else if(EN_PHASE_B)
					    		{
						    	//	S_DimmValue = Smooth_Dim(B_D_ExpV,B_D_M_ExpV);
					    		//	Dimm_Comm_PutInt((S_DimmValue  * 100));
					    			Dimm_Comm_PutInt((220 * 100));
					    		}
					    		  		
					    		Dimm_Comm_PutChar('\r');
						
					    	}
		    				Dimm_W_OpV_Cmd_Indx++;
		    				if(Dimm_W_OpV_Cmd_Indx >= 3)
		    					D_Ch_ExpV = FALSE; 											// Make it false for other phase
			    		}
			    		else if(D_Ch_Stab_Limits == TRUE) 									// If set MAX VA is not equal to Expected MAX VA
			    		{
				    		Dimm_Comm_PutString((INT8S * )Dimm_W_Cmd[Dimm_W_Cmd_Indx]);
				    		if(Dimm_W_Cmd_Indx == 1)
				    		{
					    	if(EN_PHASE_R)
							{
						
								if((R_VA_Max < R_D_M_Max_VA))
						    			Dimm_Comm_PutInt(R_VA_Max);
						    		else
									Dimm_Comm_PutInt(R_D_M_Max_VA);
						 	}   	
					    		else if(EN_PHASE_Y)
					    		{
						    		if((Y_VA_Max < Y_D_M_Max_VA))
						    			Dimm_Comm_PutInt((Y_VA_Max));
						    		else
						    			Dimm_Comm_PutInt(Y_D_M_Max_VA);
						 	}					    			
					    		else if(EN_PHASE_B)
							{
						    		//if((B_VA_Max > B_D_M_Max_VA) && (B_VA_Max <= 15000) )
						    		if((B_VA_Max < B_D_M_Max_VA))
						    			Dimm_Comm_PutInt((B_VA_Max));
						    		else
						    			Dimm_Comm_PutInt(B_D_M_Max_VA);
						 	}					    			
					    		Dimm_Comm_PutChar('\r');
				    		}
				    		if(Dimm_W_Cmd_Indx == 2)
				    		{
					    		if(EN_PHASE_R)
					    		{
						    		if(R_HV_Cut > 250 && R_HV_Cut < 300)							//	HV CUT RANGE
						    			Dimm_Comm_PutInt((R_HV_Cut * 100));
						    		else
						    			Dimm_Comm_PutInt((R_D_M_HV_Cut * 100));
					    		}
					    			
					    		else if(EN_PHASE_Y)
					    		{
					    			if(Y_HV_Cut > 250 && Y_HV_Cut < 300)							//	HV CUT RANGE	
						    			Dimm_Comm_PutInt((Y_HV_Cut * 100));
						    		else
						    			Dimm_Comm_PutInt((Y_D_M_HV_Cut * 100));
					    		}
					    			
					    		else if(EN_PHASE_B)
					    		{
								if(B_HV_Cut > 250 && B_HV_Cut < 300)
						    			Dimm_Comm_PutInt((B_HV_Cut * 100));
						    		else
						    			Dimm_Comm_PutInt((B_D_M_HV_Cut * 100));				    			
					    		}
					    			
					    		Dimm_Comm_PutChar('\r');
				    		}
				    		if(Dimm_W_Cmd_Indx == 3)
				    		{
					    	
					    		if(EN_PHASE_R)
					    		{
					    			if(R_LV_Cut > 160 && R_LV_Cut < 200)							//	LV CUT RANGE
						    			Dimm_Comm_PutInt((R_LV_Cut * 100));
						    		else
						    			Dimm_Comm_PutInt((R_D_M_LV_Cut * 100));	
					    		}
					    			
					    		else if(EN_PHASE_Y)
					    		{
					    			if(Y_LV_Cut > 160 && Y_LV_Cut < 200)							//	LV CUT RANGE
						    			Dimm_Comm_PutInt((Y_LV_Cut * 100));
						    		else
						    			Dimm_Comm_PutInt((Y_D_M_LV_Cut * 100));	
					    		}
					    		else if(EN_PHASE_B)
					    		{
					    			if(B_LV_Cut > 160 && B_LV_Cut < 200)							//	LV CUT RANGE
						    			Dimm_Comm_PutInt((B_LV_Cut * 100));
						    		else
						    			Dimm_Comm_PutInt((B_D_M_LV_Cut * 100));		    		
					    		}
					    			
					    		Dimm_Comm_PutChar('\r');
				    		}	
			    		    Dimm_W_Cmd_Indx++;
			    		}	
		    		}
		    	}
		    	if(WaitCnt >= 3)
		    	{
			   	 	WaitForTx = TRUE;
			    	WaitCnt = 0;
			    	if((Dimm_R_Cmd_Indx >= D_S_LV_CUT) && (R_W_Mode == READ_MODE ))
			    	{			    	
				    	if(EN_PHASE_R)
				    	{					    	
					    	R_Dim();
		    		
					    	if(R_D_ExpV != R_D_M_ExpV)
					    	{
						    	D_Ch_ExpV = TRUE;
						    	Dimm_R_Cmd_Indx = 0;
						    	R_W_Mode = WRITE_MODE;											//	Read_Write_Mode = Write Mode
					    	}					    	
					    	if((R_D_Max_VA != R_VA_Max) || ( R_D_HV_Cut != R_D_M_HV_Cut) || ( R_D_LV_Cut != R_D_M_LV_Cut))
					    	{
						    	D_Ch_Stab_Limits = TRUE;
						    	Dimm_R_Cmd_Indx = 0;
						    	R_W_Mode = WRITE_MODE;
					    	}		    	   
				    	}
				    	if(EN_PHASE_Y)
				    	{
					    	Y_Dim();
					    	if(Y_D_ExpV != Y_D_M_ExpV)
					    	{
						    	D_Ch_ExpV = TRUE;
						    	Dimm_R_Cmd_Indx = 0;
						    	R_W_Mode = WRITE_MODE;
					    	}					    	
					    	if ((Y_D_Max_VA != Y_VA_Max) || ( Y_D_HV_Cut != Y_D_M_HV_Cut) || ( Y_D_LV_Cut != Y_D_M_LV_Cut))	
					    	{
						    	D_Ch_Stab_Limits = TRUE;
						    	Dimm_R_Cmd_Indx = 0;
						    	R_W_Mode = WRITE_MODE;						    	
					    	}		    	   
				    	}
				    	if(EN_PHASE_B)
				    	{
					    	B_Dim();
					    	if(B_D_ExpV != B_D_M_ExpV)
					    	{
						    	D_Ch_ExpV = TRUE;
						    	Dimm_R_Cmd_Indx = 0;
						    	R_W_Mode = WRITE_MODE;
	
					    	}					    	
					    	if ((B_D_Max_VA != B_VA_Max) || ( B_D_HV_Cut != B_D_M_HV_Cut) || ( B_D_LV_Cut != B_D_M_LV_Cut))
					    	{
						    	D_Ch_Stab_Limits = TRUE;
						    	Dimm_R_Cmd_Indx = 0;
						    	R_W_Mode = WRITE_MODE;
					    	}		    	   
				    	}			    	
			    	}	
			    	if(R_W_Mode == READ_MODE) 
			    	{
			   			Dimm_R_Cmd_Indx++;
			   		}
			   		if(((Dimm_R_Cmd_Indx >= D_QUIT) && (R_W_Mode == READ_MODE)) || ( (R_W_Mode == WRITE_MODE) && ( (Dimm_W_OpV_Cmd_Indx >= 3) || (Dimm_W_Cmd_Indx >= 5)) ) )
				   	{
				   		Dimm_R_Cmd_Indx = 0;
				   		Dimm_W_OpV_Cmd_Indx =0;
				   		Dimm_W_Cmd_Indx = 0;
				   		D_Ch_ExpV = FALSE;
				   		D_Ch_Stab_Limits = FALSE;
				   		R_W_Mode = READ_MODE;
			    		if(EN_PHASE_R)
						{						
							EN_PHASE_R = 0;
							if(PH3)
							{	
								Nop();
								Nop();
							    EN_PHASE_B = 0;
							    Nop();
								Nop();
							    EN_PHASE_Y = 1;
							    Nop();
								Nop();
								#ifdef	P_Debug
							    	Debug_PutString("\n\rPhase - Y\n\r");
							    #endif
						 	} 
						 	else
						 	{
							 	EN_PHASE_R = 0;
								Nop();
								Nop();
								EN_PHASE_Y = 0;
								Nop();
								Nop();
						    	EN_PHASE_B = 0;	
						    	Nop();
								Nop();
						    	Dimm_Comm_En_Flag = FALSE;
				
						    	#ifdef	P_Debug		
						    		Debug_PutString("\n\rPORT Close\n\r");								
						    	#endif
						 	}  
						}	
						else if(EN_PHASE_Y)
						{							
							EN_PHASE_R = 0;
							Nop();
							Nop();
							EN_PHASE_Y = 0;
							Nop();
							Nop();
						    EN_PHASE_B = 1;	
						    Nop();
							Nop();	
							#ifdef	P_Debug
						    	Debug_PutString("\n\rPhase - B\n\r");								
						    #endif
						}						
						else if(EN_PHASE_B)
						{
							EN_PHASE_R = 0;
							Nop();
							Nop();
							EN_PHASE_Y = 0;
							Nop();
							Nop();
						    EN_PHASE_B = 0;	
						    Nop();
							Nop();
						    Dimm_Comm_En_Flag = FALSE;
				
						    #ifdef	P_Debug		
						    	Debug_PutString("\n\rPORT Close\n\r");								
						    #endif
						}			    	
			    	}
			    	else if((Dimm_R_Cmd_Indx >= D_QUIT) || (Dimm_W_OpV_Cmd_Indx >= 3) || (Dimm_W_Cmd_Indx >= 5))		// Added by Ravi on 13 Jan
			    	{
				    	Dimm_R_Cmd_Indx = 0;
				   		Dimm_W_OpV_Cmd_Indx =0;
				   		Dimm_W_Cmd_Indx = 0;
				   		D_Ch_ExpV = FALSE;
				   		D_Ch_Stab_Limits = FALSE;
				   		R_W_Mode = READ_MODE;
				   		
				   		EN_PHASE_R = 0;
						Nop();
						Nop();
						EN_PHASE_Y = 0;
						Nop();
						Nop();
						EN_PHASE_B = 0;	
						Nop();
						Nop();
						Dimm_Comm_En_Flag = FALSE;				    	
			    	}
				}
				WaitCnt++;	 
			}
		}
  		rxMessage = (INT8S*)OSMboxPend(UART4MboxPtr, 9000, err);		
			
	//	if((*rxMessage) == COUNT_500MS_TIMER)
		{			
			if(Dimm_Comm_En_Flag)														// Data parsing from the recieved data
			{
			    Rx_Data_Len = Read_Dimmer_Data();				    	    
			    if(Rx_Data_Len>0)
			    {
				    if(	R_W_Mode == READ_MODE)
			      		Parse_Dimmer_Data(Dimm_R_Cmd_Indx, Rx_Data_Len);
			      	else
						Read_Dimm_Resp(RecievedData, Rx_Data_Len);
			      	Rx_Data_Len = 0;
			    }	    	
			}	
			//if(Dimm_Comm_En_Flag == FALSE) 												// Read Data from Dimmer during dimmer normal mode
			else
			{
				Rx_Data_Len = Read_Dimmer_Data();
				if(Rx_Data_Len>0)
					Read_Dimm_Resp(RecievedData, Rx_Data_Len);
			}	
		}// end of (*rxMessage) == COUNT_500MS_TIMER) 	
	}// end of while(1)	
}// end of task


void Parse_Dimmer_Data(INT16S Cmd_Indx, INT16S len)
{
	INT32S  Rx_Dimm_Data;

	switch(Cmd_Indx)
	{
		case D_Z:													//	Read Responce for cmd 'Z'.
			#ifdef	P_Debug
				Debug_PutString("Z :");
			#endif
			Read_Dimm_Resp(RecievedData, len);
			break;
		case D_TEMP:												//	Read Temperature from Dimmer
			#ifdef	P_Debug
				Debug_PutString("temp :");
			#endif
			Rx_Dimm_Data = Read_Dimm_Resp(RecievedData, len);
			if(EN_PHASE_R)
				R_D_Temp = Rx_Dimm_Data;
			else if(EN_PHASE_Y)
				Y_D_Temp = Rx_Dimm_Data;
			else if(EN_PHASE_B)
				B_D_Temp = Rx_Dimm_Data;
			break;
		case D_IN_V:												//	Read Input Voltage from Dimmer
			#ifdef	P_Debug
				Debug_PutString("ip volt :");
			#endif
			Rx_Dimm_Data = Read_Dimm_Resp(RecievedData, len)/100;
			if(EN_PHASE_R)
				R_D_IPV = Rx_Dimm_Data;
			else if(EN_PHASE_Y)
				Y_D_IPV = Rx_Dimm_Data;
			else if(EN_PHASE_B)
				B_D_IPV = Rx_Dimm_Data;
			break;
		case D_OP_V:												//	Read Output Voltage from Dimmer			
			#ifdef	P_Debug
				Debug_PutString("Op volt :");
			#endif
			Rx_Dimm_Data = Read_Dimm_Resp(RecievedData, len)/100;
			if(EN_PHASE_R)
				R_D_OPV = Rx_Dimm_Data;
			else if(EN_PHASE_Y)
				Y_D_OPV = Rx_Dimm_Data;
			else if(EN_PHASE_B)
				B_D_OPV = Rx_Dimm_Data;
		    break;
		case D_LOAD:												//	Read Load in VA from Dimmer
			#ifdef	P_Debug
				Debug_PutString("Load :");
			#endif
			Rx_Dimm_Data = Read_Dimm_Resp(RecievedData, len);
			if(EN_PHASE_R)
				R_D_VA = Rx_Dimm_Data;
			else if(EN_PHASE_Y)
				Y_D_VA = Rx_Dimm_Data;
			else if(EN_PHASE_B)
				B_D_VA = Rx_Dimm_Data;
			break;
		case D_FREQ:												//	Read Frequency from Dimmer
			#ifdef	P_Debug
			 	Debug_PutString("Hz :");
			 #endif
			 Rx_Dimm_Data = Read_Dimm_Resp(RecievedData, len)/1000;
			if(EN_PHASE_R)
				R_D_Freq = Rx_Dimm_Data;
			else if(EN_PHASE_Y)
				Y_D_Freq = Rx_Dimm_Data;
			else if(EN_PHASE_B)
				B_D_Freq = Rx_Dimm_Data;
			break;
		case D_FLAGS:												//	Read 16 Bit flag status from Dimmer	
			#ifdef	P_Debug
			 	Debug_PutString("Flags :");
			 #endif
			 Rx_Dimm_Data = Read_Dimm_Resp(RecievedData, len);
			if(EN_PHASE_R)
				R_D_Flags = Rx_Dimm_Data;
			else if(EN_PHASE_Y)
				Y_D_Flags = Rx_Dimm_Data;
			else if(EN_PHASE_B)
				B_D_Flags = Rx_Dimm_Data;

			break;
		case D_S_OP_V:												//	Read Output Voltage Set value from Dimmer
			#ifdef	P_Debug
				Debug_PutString("S OP Volts:");
			#endif
			Rx_Dimm_Data = Read_Dimm_Resp(RecievedData, len)/100;
			if(EN_PHASE_R)
				R_D_ExpV = Rx_Dimm_Data;
			else if(EN_PHASE_Y)
				Y_D_ExpV = Rx_Dimm_Data;
			else if(EN_PHASE_B)
				B_D_ExpV = Rx_Dimm_Data;
			break;
		case D_S_MAX_LOAD:											//	Read Max_load in VA set value from Dimmer
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
			#ifdef	P_Debug
				Dimm_Comm_PutString("No Resp :");
			#endif	
			break;				
	}	
}	




INT16S Read_Dimm_Resp(INT8U * DataPtr, INT16 len)
{
	INT8U i = 0;
	INT8U ValueString[6];
	INT8U ValueStringIndex = 0;
	INT8U data;
	INT16S DimmValue = 0;
	while(len > 0 )
	{
		 data = DataPtr[i];
		 if((data >= 48) && (data <= 57))								//	Capture Numeric Values Only
		 {		 
			 ValueString[ValueStringIndex] = data; 
			 ValueStringIndex++;
		 } 
		 i++;
		 len--;
	}
	if(ValueStringIndex > 0)
	{
		ValueString[ValueStringIndex] = '\0';
		DimmValue = AtoI(ValueString);
	}
	#ifdef	P_Debug
		Debug_PutInt(DimmValue);
		Debug_PutString("\n\r");
	#endif
	return DimmValue;
}	


static void R_Dim(void)
{
	if(RD_Val == 'D')											//	Time Based Dimming
	{
		if(R_CSLOT >= Min_Dim_OP_Volt && R_CSLOT <= Max_Dim_OP_Volt)
			R_D_M_ExpV = R_CSLOT;				 
		else 
			R_D_M_ExpV =  Default_OP_Volt;	
	}
	else														//	Server Controlled Output Voltage	
	{
		if(RD_Val >= Min_Dim_OP_Volt && RD_Val <= Max_Dim_OP_Volt)
			R_D_M_ExpV = RD_Val;
		else 
			R_D_M_ExpV = Default_OP_Volt;
	}
}

static void Y_Dim(void)
{
	if(YD_Val == 'D')											//	Time Based Dimming
    {
		if(Y_CSLOT >= Min_Dim_OP_Volt && Y_CSLOT <= Max_Dim_OP_Volt)
			Y_D_M_ExpV = Y_CSLOT;				 
	    	else 
	    		Y_D_M_ExpV =  Default_OP_Volt;	
    }
    else														//	Server Controlled Output Voltage
    {
	   	if(YD_Val >= Min_Dim_OP_Volt && YD_Val <= Max_Dim_OP_Volt)
	   		Y_D_M_ExpV = YD_Val;
	    	else 
	    		Y_D_M_ExpV = Default_OP_Volt;
    }
}

static void B_Dim(void)
{
	if(BD_Val == 'D')											//	Time Based Dimming
    {
		if(B_CSLOT >= Min_Dim_OP_Volt && B_CSLOT <= Max_Dim_OP_Volt)
			B_D_M_ExpV = B_CSLOT;				 
		else 
	   		B_D_M_ExpV =  Default_OP_Volt;		
    }
    else														//	Server Controlled Output Voltage
   	{
		if(BD_Val >= Min_Dim_OP_Volt && BD_Val <= Max_Dim_OP_Volt)
			B_D_M_ExpV = BD_Val;
		else 
	    		B_D_M_ExpV = Default_OP_Volt;
    }
}


static INT16U Smooth_Dim(INT16U Rd_OPV, INT16U Exp_OPV)			//	Regulate Output Voltage Smoothly
{
	INT16U S_Dimm_Value = 0;
	if( Rd_OPV > Exp_OPV )										//	Decrease Ouput Voltage if Existing Output is greater than expected		
	{
		if( (Rd_OPV - Exp_OPV) < Adj_Volts)						//	Difference BTN Output Voltage and Expected Voltage is less than Smooth Dim Value
			S_Dimm_Value  = Exp_OPV;
		else 													//	Decrease Adjustable value to the Output
			S_Dimm_Value  = Rd_OPV - Adj_Volts;
	}
	else if( Rd_OPV < Exp_OPV )									//	Increase Ouput Voltage if Existing Output is less than expected
	{
		if( (Exp_OPV - Rd_OPV ) < Adj_Volts)					//	Difference BTN Output Voltage and Expected Voltage is less than Smooth Dim Value
			S_Dimm_Value  = Exp_OPV;
		S_Dimm_Value  = Rd_OPV + Adj_Volts;						//	Increase Adjustable value to the Output
	}
	else 
		S_Dimm_Value  = Exp_OPV;
	if( S_Dimm_Value >= Min_Dim_OP_Volt && S_Dimm_Value <= Max_Dim_OP_Volt) 			//	Buck_Boost Voltage value should be always between Minimum and maximum Output Voltage.	
		return S_Dimm_Value;
	else 
	{
		S_Dimm_Value = Default_OP_Volt;										//	Default Output Voltage							
		return S_Dimm_Value;
	}
}
