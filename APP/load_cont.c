#include <includes.h>
#include    "p24FJ256GA110.h"


INT8U SLOT1 = 210, SLOT2 = 200, SLOT3 = 190, GENRAL = 210;						//	Modified by RAVI Default 210 Volts
  
UnionTimer DS, DE, S1, S2;

void Init_DimSlots(void);
static unsigned char ON_OFF(void);
static unsigned char ON_OFF_Y(void);
static unsigned char ON_OFF_B(void);

static void ALP_Dis(void);
static void ALP_En(void);
static void Ph_R_ON(void);
static void Ph_R_OFF(void);
static void Ph_Y_ON(void);
static void Ph_Y_OFF(void);
static void Ph_B_ON(void);
static void Ph_B_OFF(void);
void All_ON(void);
void All_OFF(void);
void Init_SNRS(void);
void Chk_SNRS(void);



void Init_ONOFF_TIME()
{
	ONTime.StructTime.Hrs		=	ON_HH;
	ONTime.StructTime.Mins		=	ON_MM;
	ONTime.StructTime.Secs		=	0;
	
	OFFTime.StructTime.Hrs		=	OFF_HH;
	OFFTime.StructTime.Mins		=	OFF_MM;
	OFFTime.StructTime.Secs		=	0;
	
	Y_ONTime.StructTime.Hrs		=	ON_HH;
	Y_ONTime.StructTime.Mins	=	ON_MM;
	Y_ONTime.StructTime.Secs	=	0;
	
	Y_OFFTime.StructTime.Hrs	=	OFF_HH;
	Y_OFFTime.StructTime.Mins	=	OFF_MM;
	Y_OFFTime.StructTime.Secs	=	0;
	
	B_ONTime.StructTime.Hrs		=	ON_HH;
	B_ONTime.StructTime.Mins	=	ON_MM;
	B_ONTime.StructTime.Secs	=	0;
	
	B_OFFTime.StructTime.Hrs	=	OFF_HH;
	B_OFFTime.StructTime.Mins	=	OFF_MM;
	B_OFFTime.StructTime.Secs	=	0;
}

void Init_Load_SW(void)
{	
	DDR_A_Set		=	OUTPUT;	
	DDR_A_Reset		=	OUTPUT;							
	DDR_B_Set		=	OUTPUT;	
	DDR_B_Reset		=	OUTPUT;									
	DDR_C_Set		=	OUTPUT;			
	DDR_C_Reset		=	OUTPUT;									
	DDR_D_Set		=	OUTPUT;	
	DDR_D_Reset		=	OUTPUT;	
	DDR_LATCH1		=	OUTPUT;	
	DDR_LATCH2		=	OUTPUT;		
	
	LATCH_1 = LATCH_2	 	=  0;				//	Disable Latch;
	A_Set 			=  0;				//	Phase-R Set	
	A_Reset 		=  0;				//	Phase-R Reset
	B_Set 			=  0;				//	Phase-Y Set
	B_Reset 		=  0;				//	Phase-Y Reset
	C_Set 			=  0;				//	Phase-B Set	
	C_Reset 		=  0;				//	Phase-B Reset	
	#ifdef	L_DEBUG
		Debug_PutString("\n\rINIT LOAD..");	
	#endif
}

void ON_OFF_Switch(void )
{
	
	if(!ALP)	
	{										
		ALP_Dis();						//	ALP Disabled		
	}
	else	
	{		
		ALP_En();									//	ALP Enabled			
	}
}													// End ContactorOn()


static unsigned char ON_OFF(void)
{
	ONTime.StructTime.Reserved		=	0;			//	Reserver byte in MSB w.r.t Union is Cleared
	OFFTime.StructTime.Reserved		=	0;			//	Reserver byte in MSB w.r.t Union is Cleared
	
	if(ONTime.TimeValue < OFFTime.TimeValue)  							/* On/Off are on the same day     */   
   	{
    		if( (Sys_Time.TimeValue >= ONTime.TimeValue) && ( Sys_Time.TimeValue <= OFFTime.TimeValue))
    		{
	    		#ifdef	L_DEBUG
	    		Debug_PutString("\n\r System Time Value : ");
				Debug_PutInt(Sys_Time.TimeValue);
				Debug_PutString("\n\r On Time Value : ");
				Debug_PutInt(ONTime.TimeValue);
				Debug_PutString("\n\r Off Time Value : ");
				Debug_PutInt(OFFTime.TimeValue);
				Debug_PutString("\n\rSame Day ON\n\r");
				#endif
	   			return ON;
	   		}	
	   	else
	   		{
		   		#ifdef	L_DEBUG
		   		Debug_PutString("\n\r System Time Value : ");
				Debug_PutInt(Sys_Time.TimeValue);
				Debug_PutString("\n\r On Time Value : ");
				Debug_PutInt(ONTime.TimeValue);
				Debug_PutString("\n\r Off Time Value : ");
				Debug_PutInt(OFFTime.TimeValue);
				Debug_PutString("\n\rSame Day OFF\n\r");
				#endif
	    		return OFF;
	    	}	
	}
	else                              									/* If On/Off are on different days */
	{
		if( (Sys_Time.TimeValue >= OFFTime.TimeValue) && ( Sys_Time.TimeValue <= ONTime.TimeValue))
		{
			#ifdef	L_DEBUG
			Debug_PutString("\n\r System Time Value : ");
			Debug_PutInt(Sys_Time.TimeValue);
			Debug_PutString("\n\r On Time Value : ");
			Debug_PutInt(ONTime.TimeValue);
			Debug_PutString("\n\r Off Time Value : ");
			Debug_PutInt(OFFTime.TimeValue);
			Debug_PutString("\n\rDifferent Day OFF\n\r");
			#endif
	   		return OFF;
	 	}  		
	   	else
	   	{
		   	#ifdef	L_DEBUG
	   		Debug_PutString("\n\r System Time Value : ");
			Debug_PutInt(Sys_Time.TimeValue);
			Debug_PutString("\n\r On Time Value : ");
			Debug_PutInt(ONTime.TimeValue);
			Debug_PutString("\n\r Off Time Value : ");
			Debug_PutInt(OFFTime.TimeValue);
			Debug_PutString("\n\rDifferent Day ON\n\r");
			#endif
	   		return ON;
	   	}	
   	}
}


static unsigned char ON_OFF_Y(void)
{
	Y_ONTime.StructTime.Reserved		=	0;			//	Reserver byte in MSB w.r.t Union is Cleared
	Y_OFFTime.StructTime.Reserved		=	0;			//	Reserver byte in MSB w.r.t Union is Cleared

	if(Y_ONTime.TimeValue < Y_OFFTime.TimeValue)  							/* On/Off are on the same day     */   
   	{
	    	if( (Sys_Time.TimeValue >= Y_ONTime.TimeValue) && ( Sys_Time.TimeValue <= Y_OFFTime.TimeValue))
		   		return ON;
		   	else
		    	return OFF;
	}
	else                              									/* If On/Off are on different days */
	{
		if( (Sys_Time.TimeValue >= Y_OFFTime.TimeValue) && ( Sys_Time.TimeValue <= Y_ONTime.TimeValue))
	   		return OFF;
	   	else
	   		return ON;
	}
}

static unsigned char ON_OFF_B(void)
{
	B_ONTime.StructTime.Reserved		=	0;			//	Reserver byte in MSB w.r.t Union is Cleared
	B_OFFTime.StructTime.Reserved		=	0;			//	Reserver byte in MSB w.r.t Union is Cleared
	
	if(B_ONTime.TimeValue < B_OFFTime.TimeValue)  							/* On/Off are on the same day     */   
   	{
    	if( (Sys_Time.TimeValue >= B_ONTime.TimeValue) && ( Sys_Time.TimeValue <= B_OFFTime.TimeValue))
	   		return ON;
	   	else
	    	return OFF;
	}
	else                              									/* If On/Off are on different days */
	{
		if( (Sys_Time.TimeValue >= B_OFFTime.TimeValue) && ( Sys_Time.TimeValue <= B_ONTime.TimeValue))
	   		return OFF;
	   	else
	   		return ON;
   }
}



static void ALP_Dis(void)
{
	if( ON_OFF() )
	{
		#ifdef	L_DEBUG
				Debug_PutString("*!*!*!*!*!..");				
		#endif	
		
		if( (SR2 && !SR3) || (SY2 && !SY3) || (SB2 && !SB3) )
		{
			#ifdef	L_DEBUG
				Debug_PutString("\n\rON..");
			#endif	
			if(SR2 && !SR3)
				Ph_R_ON();
			if(SY2 && !SY3)
				Ph_Y_ON();
			if(SB2 && !SB3)
				Ph_B_ON();
			Reset_Z();
		}					//	Close if()	
	}						//	Close if(ON_OFF)						
	else
	{
		#ifdef	L_DEBUG
				Debug_PutString("*$$*$$*$$..");				
		#endif	
	
		if(SR3 || SR4 || SY3 || SY4 || SB3 || SB4 )
		{
			#ifdef	L_DEBUG
				Debug_PutString("\n\rOFF..");
			#endif
			if(SR3 || SR4)
				Ph_R_OFF();
			if(SY3 || SY4)
				Ph_Y_OFF();
			if(SB3 || SB4)
				Ph_B_OFF();	
			Reset_Z();	 
		}					//	Close if()	
	}						//	Close else(ON_OFF)	
	#ifdef	L_DEBUG
		Debug_PutString("\n\n\r\t\t********************************");	
		Debug_PutString("\n\n\r HH:MM:SS ");
		Debug_PutInt(Hour);
		Debug_PutChar(':'); 
		Debug_PutInt(Min);
		Debug_PutChar(':'); 
		Debug_PutInt(Sec);
		Debug_PutString("\t Sys_Time ");
		Debug_PutInt(Sys_Time.StructTime.Hrs); 
		Debug_PutChar(':'); 
		Debug_PutInt(Sys_Time.StructTime.Mins); 
		Debug_PutChar(':'); 
		Debug_PutInt(Sys_Time.StructTime.Secs); 									
		Debug_PutString("\t   ON Time ");
		Debug_PutInt(ONTime.StructTime.Hrs); 
		Debug_PutChar(':'); 
		Debug_PutInt(ONTime.StructTime.Mins); 
		Debug_PutChar(':'); 
		Debug_PutInt(ONTime.StructTime.Secs); 
		Debug_PutString("\t OFF Time ");
		Debug_PutInt(OFFTime.StructTime.Hrs); 
		Debug_PutChar(':'); 
		Debug_PutInt(OFFTime.StructTime.Mins); 
		Debug_PutChar(':'); 
		Debug_PutInt(OFFTime.StructTime.Secs); 	
		Debug_PutString("\n\r\t\t********************************\n");
	#endif	
			
}

static void ALP_En(void)
{
	#ifdef	L_DEBUG
		Debug_PutString("\n\rALP ENABLED..");
	#endif
	if( ON_OFF() )
	{
		if( SR2 && !SR3 )
			Ph_R_ON();
		Reset_Z();
	}											
	else
	{
		if(SR3 || SR4)
			Ph_R_OFF();
		Reset_Z();	 
	}					
	
	if( ON_OFF_Y() )
	{
		if(SY2 && !SY3)
			Ph_Y_ON();
		Reset_Z();
	}											
	else
	{
		if(SY3 || SY4)
			Ph_Y_OFF();
		Reset_Z();	 
	}							
	
	if( ON_OFF_B() )
	{
		if(SB2 && !SB3)
				Ph_B_ON();
			Reset_Z();
	}									
	else
	{
		if(SB3 || SB4)
			Ph_B_OFF();	
		Reset_Z();	 
	}							
}

static void Ph_R_ON(void)
{
	#ifdef	L_DEBUG
		Debug_PutString("\n\rR ON..");
	#endif
	LATCH_1 		= 1;				//	Enable Latch;
	LATCH_2			= 1;				//	Enable Latch;
	A_Set 			= 1;				//	Phase-R Set	
	A_Reset 		= 0;				//	Phase-R Reset
	B_Set 			= 0;				//	Phase-Y Set
	B_Reset 		= 0;				//	Phase-Y Reset
	C_Set 			= 0;				//	Phase-B Set	
	C_Reset 		= 0;				//	Phase-B Reset	
	OSTimeDly(150);						//	700 milli Seconds minimum Delay Required
	LATCH_1 		= 0;
	LATCH_2			= 0;				//	Disable Latch;
}


static void Ph_R_OFF(void)
{
	#ifdef	L_DEBUG
		Debug_PutString("\n\rR OFF..");
	#endif
	LATCH_1 	= 1;
	LATCH_2	 	= 1;				//	Enable Latch;
	A_Set 		= 0;				//	Phase-R Set	
	A_Reset 	= 1;				//	Phase-R Reset
	B_Set 		= 0;				//	Phase-Y Set
	B_Reset 	= 0;				//	Phase-Y Reset
	C_Set 		= 0;				//	Phase-B Set	
	C_Reset 	= 0;				//	Phase-B Reset	
	OSTimeDly(150);					//	700 milli Seconds minimum Delay Required
	LATCH_1 	= 0;
	LATCH_2	 	= 0;				//	Disable Latch;
}

static void Ph_Y_ON(void)
{
	#ifdef	L_DEBUG
		Debug_PutString("\n\rY ON..");
	#endif
	LATCH_1 	= 1;
	LATCH_2	 	= 1;				//	Enable Latch;
	A_Set 		= 0;				//	Phase-R Set	
	A_Reset 	= 0;				//	Phase-R Reset
	B_Set 		= 1;				//	Phase-Y Set
	B_Reset 	= 0;				//	Phase-Y Reset
	C_Set 		= 0;				//	Phase-B Set	
	C_Reset 	= 0;				//	Phase-B Reset	
	OSTimeDly(150);					//	700 milli Seconds minimum Delay Required
	LATCH_1 	= 0;
	LATCH_2	 	= 0;				//	Disable Latch;
}

static void Ph_Y_OFF(void)
{
	#ifdef	L_DEBUG
		Debug_PutString("\n\rY OFF..");
	#endif
	LATCH_1 	= 1;
	LATCH_2	 	= 1;				//	Enable Latch;
	A_Set 		= 0;				//	Phase-R Set	
	A_Reset 	= 0;				//	Phase-R Reset
	B_Set 		= 0;				//	Phase-Y Set
	B_Reset 	= 1;				//	Phase-Y Reset
	C_Set 		= 0;				//	Phase-B Set	
	C_Reset 	= 0;				//	Phase-B Reset	
	OSTimeDly(150);					//	700 milli Seconds minimum Delay Required
	LATCH_1 	= 0;
	LATCH_2	 	= 0;				//	Disable Latch;

}

static void Ph_B_ON(void)
{
	#ifdef	L_DEBUG
		Debug_PutString("\n\rB ON..");
	#endif
	LATCH_1 	= 1;
	LATCH_2	 	= 1;				//	Enable Latch;
	A_Set 		= 0;				//	Phase-R Set	
	A_Reset 	= 0;				//	Phase-R Reset
	B_Set 		= 0;				//	Phase-Y Set
	B_Reset 	= 0;				//	Phase-Y Reset
	C_Set 		= 1;				//	Phase-B Set	
	C_Reset 	= 0;				//	Phase-B Reset	
	OSTimeDly(150);					//	700 milli Seconds minimum Delay Required
	LATCH_1 	= 0;
	LATCH_2	 	= 0;				//	Disable Latch;

}

static void Ph_B_OFF(void)
{
	#ifdef	L_DEBUG
		Debug_PutString("\n\rB OFF..");
	#endif
	LATCH_1 	= 1;				//	Enable Latch;
	LATCH_2 	= 1;
	A_Set 		= 0;				//	Phase-R Set	
	A_Reset 	= 0;				//	Phase-R Reset
	B_Set 		= 0;				//	Phase-Y Set
	B_Reset 	= 0;				//	Phase-Y Reset
	C_Set 		= 0;				//	Phase-B Set	
	C_Reset 	= 1;				//	Phase-B Reset	
	OSTimeDly(150);					//	700 milli Seconds minimum Delay Required
	LATCH_1 	= 0;				//	Disable Latch;
	LATCH_2	 	= 0;
}

void Reset_Z(void)
{
	#ifdef	L_DEBUG
		Debug_PutString("\n\rReset Zettlers..");
	#endif
	LATCH_1 	=  1;				//	Enable Latch;
	LATCH_2	 	= 1;
	A_Set 		= 0;				//	Phase-R Set	
	A_Reset 	= 0;				//	Phase-R Reset
	B_Set 		= 0;				//	Phase-Y Set
	B_Reset 	= 0;				//	Phase-Y Reset
	C_Set 		= 0;				//	Phase-B Set	
	C_Reset 	= 0;				//	Phase-B Reset	
	OSTimeDly(150);					//	700 milli Seconds minimum Delay Required
	LATCH_1 	= 0;				//	Disable Latch;
	LATCH_2	 	= 0;	
}

void All_ON(void)
{
	#ifdef	L_DEBUG
		Debug_PutString("\n\rAll Zettlers ON..");
	#endif
	LATCH_1		= 1;				//	Enable Latch;
	LATCH_2	 	= 1;
	A_Set 		= 1;				//	Phase-R Set	
	A_Reset 	= 0;				//	Phase-R Reset
	B_Set 		= 1;				//	Phase-Y Set
	B_Reset 	= 0;				//	Phase-Y Reset
	C_Set 		= 1;				//	Phase-B Set	
	C_Reset 	= 0;				//	Phase-B Reset	
	OSTimeDly(150);					//	700 milli Seconds minimum Delay Required
	LATCH_1 	= 0;				//	Disable Latch;
	LATCH_2	 	= 0;
}

void All_OFF(void)
{
	#ifdef	L_DEBUG
		Debug_PutString("\n\rAll Zettlers OFF..");
	#endif
	LATCH_1 = 	 1;				//	Enable Latch;
	LATCH_2	 	= 1;
	A_Set 		= 0;				//	Phase-R Set	
	A_Reset 	= 1;				//	Phase-R Reset
	B_Set 		= 0;				//	Phase-Y Set
	B_Reset 	= 1;				//	Phase-Y Reset
	C_Set 		= 0;				//	Phase-B Set	
	C_Reset 	= 1;				//	Phase-B Reset	
	OSTimeDly(150);					//	700 milli Seconds minimum Delay Required
	LATCH_1 	= 0;				//	Disable Latch;
	LATCH_2	 	= 0;
}



void Init_DimSlots(void)
{
	DS.TimeValue = 0;
	DE.TimeValue = 0;
	S1.TimeValue = 0;
	S2.TimeValue = 0;
	
	DS.StructTime.Hrs = DS_HH;
	DS.StructTime.Mins = DS_MM;
	DS.StructTime.Secs = 0;	

	if((OFFTime.StructTime.Mins - 30) < 0)
	{
		DE.StructTime.Hrs = OFFTime.StructTime.Hrs - 1;
		if(OFFTime.StructTime.Mins == 0)
			DE.StructTime.Mins = 30;
		else
			DE.StructTime.Mins = 30 + (30 - OFFTime.StructTime.Mins);
	}
	else
	{
		DE.StructTime.Hrs = OFFTime.StructTime.Hrs;
		DE.StructTime.Mins = OFFTime.StructTime.Mins - 30;
	}
	DE.StructTime.Secs = 0;	
	S1.StructTime.Hrs = S1_HH;
	S1.StructTime.Mins = S1_MM;
	S1.StructTime.Secs = 0;	
	
	S2.StructTime.Hrs = S2_HH;
	S2.StructTime.Mins = S2_MM;
	S2.StructTime.Secs = 0;		
}

INT8U Dim_Slots(void)
{
	if(DS.TimeValue < DE.TimeValue)					// On/Off are on the same day
	{

		if( (Sys_Time.TimeValue >= DS.TimeValue) && (Sys_Time.TimeValue < DE.TimeValue) )
		{
			if( (Sys_Time.TimeValue >= DS.TimeValue) && (Sys_Time.TimeValue < S1.TimeValue) )
				return SLOT1;
			else if( (Sys_Time.TimeValue >= S1.TimeValue) && (Sys_Time.TimeValue < S2.TimeValue) )
				return SLOT2;
			else if( (Sys_Time.TimeValue >= S2.TimeValue) && (Sys_Time.TimeValue < DE.TimeValue) )
				return SLOT3;
		}
		else
			return GENRAL;
	}

	else if(DS.TimeValue > DE.TimeValue)	
	{

		if(Sys_Time.TimeValue >= DS.TimeValue && Sys_Time.TimeValue < S1.TimeValue)
			return SLOT1;
		else if(S2.TimeValue > DE.TimeValue)
		{

			if(Sys_Time.TimeValue >= S1.TimeValue && Sys_Time.TimeValue < S2.TimeValue)
				return SLOT2;
			else if (Sys_Time.TimeValue >= S2.TimeValue || Sys_Time.TimeValue < DE.TimeValue)
				return SLOT3;
			else
				return GENRAL;
		}		
		else if (S2.TimeValue < DE.TimeValue)
		{
		
			if(Sys_Time.TimeValue >= S1.TimeValue || Sys_Time.TimeValue < S2.TimeValue)
				return SLOT2;
			else if(Sys_Time.TimeValue >= S2.TimeValue && Sys_Time.TimeValue < DE.TimeValue)
				return SLOT3;
			else
				return GENRAL;
		}
	}
	return;
}

