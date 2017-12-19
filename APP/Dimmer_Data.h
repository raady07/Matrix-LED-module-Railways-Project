#ifndef  DIMMER_DATA_H
#define  DIMMER_DATA_H

typedef enum
{
	PHASE_R = 0,
	PHASE_Y,
	PHASE_B
}Phase;	


typedef enum
{
	D_Z 	=		0,	
	D_TEMP,
	D_IN_V,   		
	D_OP_V,   		
	D_LOAD,     		
	D_FREQ,      	
	D_FLAGS,			
	D_S_OP_V,		
	D_S_MAX_LOAD, 	
	D_S_HV_CUT,		
	D_S_LV_CUT,	
	D_QUIT				
}DIMM_CMDS;


//extern BOOL D_Ch_ExpV;
//extern BOOL D_Ch_Stab_Limits;

volatile extern INT8U R_D_M_ExpV;					//	Expected Output Voltage as per master
//extern INT16U R_D_M_Max_VA;					//	Stabilizer Maximum VA as per master
//extern INT16U R_D_M_HV_Cut;					//	Stabilizer High Voltage Cut w.r.t Input as per master
//extern INT16U R_D_M_LV_Cut;					//	Stabilizer Low Voltage Cut w.r.t Input as per master

volatile extern INT8U Y_D_M_ExpV;					//	Expected Output Voltage as per master
//extern INT16U Y_D_M_Max_VA;					//	Stabilizer Maximum VA as per master
//extern INT16U Y_D_M_HV_Cut;					//	Stabilizer High Voltage Cut w.r.t Input as per master
//extern INT16U Y_D_M_LV_Cut;					//	Stabilizer Low Voltage Cut w.r.t Input as per master

volatile extern INT8U B_D_M_ExpV;					//	Expected Output Voltage as per master
//extern INT16U B_D_M_Max_VA;					//	Stabilizer Maximum VA as per master
//extern INT16U B_D_M_HV_Cut;					//	Stabilizer High Voltage Cut w.r.t Input as per master
//extern INT16U B_D_M_LV_Cut;					//	Stabilizer Low Voltage Cut w.r.t Input as per master

volatile extern INT16U Dim_OPV;
volatile extern INT16U R_D_Temp;					//	Dimmer Mosfet Temperature
volatile extern INT16U R_D_IPV;						//	Dimmer Input Voltage
volatile extern INT16U R_D_OPV;						//	Dimmer Output Voltage
volatile extern INT16U R_D_VA;						//	Load On Stabilizer in VA
volatile extern INT16U R_D_Freq;						//	Dimmer Frequency
volatile extern INT16U R_D_Flags;	                //	Dimmer 16 bit flags	
volatile extern INT16U R_D_ExpV;						//	Expected Output Voltage
volatile extern INT16U R_D_Max_VA;				    //	Stabilizer Maximum VA
volatile extern INT16U R_D_HV_Cut;				    //	Stabilizer High Voltage Cut w.r.t Input
volatile extern INT16U R_D_LV_Cut;				    //	Stabilizer Low Voltage Cut w.r.t Input

volatile extern INT16U Y_D_Temp;						//	Dimmer Mosfet Temperature
volatile extern INT16U Y_D_IPV;						//	Dimmer Input Voltage
volatile extern INT16U Y_D_OPV;						//	Dimmer Output Voltage
volatile extern INT16U Y_D_VA;						//	Load On Stabilizer in VA
volatile extern INT16U Y_D_Freq;						//	Dimmer Frequency
volatile extern INT16U Y_D_Flags;	                //	Dimmer 16 bit flags	
volatile extern INT16U Y_D_ExpV;						//	Expected Output Voltage
volatile extern INT16U Y_D_Max_VA;				    //	Stabilizer Maximum VA
volatile extern INT16U Y_D_HV_Cut;				    //	Stabilizer High Voltage Cut w.r.t Input
volatile extern INT16U Y_D_LV_Cut;				    //	Stabilizer Low Voltage Cut w.r.t Input

volatile extern INT16U B_D_Temp;						//	Dimmer Mosfet Temperature
volatile extern INT16U B_D_IPV;						//	Dimmer Input Voltage
volatile extern INT16U B_D_OPV;						//	Dimmer Output Voltage
volatile extern INT16U B_D_VA;						//	Load On Stabilizer in VA
volatile extern INT16U B_D_Freq;						//	Dimmer Frequency
volatile extern INT16U B_D_Flags;	                //	Dimmer 16 bit flags	
volatile extern INT16U B_D_ExpV;						//	Expected Output Voltage
volatile extern INT16U B_D_Max_VA;				    //	Stabilizer Maximum VA
volatile extern INT16U B_D_HV_Cut;				    //	Stabilizer High Voltage Cut w.r.t Input
volatile extern INT16U B_D_LV_Cut;				    //	Stabilizer Low Voltage Cut w.r.t Input


 extern const char * Dimm_W_OpV_Cmd[];
 extern const char * Dimm_R_Cmd[]; 			// just to avoid warning used char instead of INT8S
 extern const char * Dimm_W_Cmd[];
 extern const char * Dimm_W_All_Cmds[];
volatile extern INT8U  Dimm_R_Cmd_Indx;
volatile extern INT8U  Dimm_W_Cmd_Indx;
volatile extern INT8U  Dimm_W_All_Cmd_Indx;
volatile extern INT8U Dimm_W_OpV_Cmd_Indx;
volatile extern INT8U RecievedData[];
volatile extern INT16S RxDataIndex;

void Parse_Dimmer_Data(INT16S, INT16S );
INT32U Read_Dimm_Resp(INT8U * DataPtr, INT16 len);


#endif


