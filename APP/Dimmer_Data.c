#include <includes.h>

const char * Dimm_R_Cmd[] = {"z\r","v 0\r" , "v 1\r", "v 2\r",  "v 3\r", "v 4\r", "v 5\r", "s 0\r", "s 1\r", "s 2\r","s 3\r","q\r"};
const char * Dimm_W_OpV_Cmd[] = {"t 12345\r", "w 0 ", "f\r","q\r"};
const char * Dimm_W_Cmd[] = { "t 12345\r", "w 1 ", "w 2 ", "w 3 ", "f\r","q\r"};
const char * Dimm_W_All_Cmds[] = { "t 12345\r", "w 0 ", "w 1 ", "w 2 ", "w 3 ", "f\r","q\r"};

volatile INT8U  Dimm_W_All_Cmd_Indx = 0;
volatile INT8U  Dimm_R_Cmd_Indx = 0;
volatile INT8U  Dimm_W_Cmd_Indx = 0;
volatile INT8U  Dimm_W_OpV_Cmd_Indx = 0;
volatile INT8U RecievedData[25];
volatile INT16S RxDataIndex = 0;



/*
*********************************************************************************************************
*                                       Variables to Hold Default Output Voltage's w.r.t Phase
* Macro: Default_OP_Volt in App.
*********************************************************************************************************
*/


volatile INT8U R_D_M_ExpV = Default_OP_Volt;						//	Expected Output Voltage as per master 
volatile INT8U Y_D_M_ExpV = Default_OP_Volt;						//	Expected Output Voltage as per master
volatile INT8U B_D_M_ExpV = Default_OP_Volt;						//	Expected Output Voltage as per master


/*
*********************************************************************************************************
*                                       Variables to Hold PHASE - R parameters after reading
*********************************************************************************************************
*/



volatile INT16U R_D_Temp 	= 0;									//	Dimmer Mosfet Temperature
volatile INT16U R_D_IPV 	= 0;									//	Dimmer Input Voltage

volatile INT16U Dim_OPV 	= Default_OP_Volt;						//	Dimmer Output Voltage
volatile INT16U R_D_OPV 	= Default_OP_Volt;						//	Dimmer Output Voltage
volatile INT16U R_D_VA 		= Default_Max_VA;						//	Load On Stabilizer in VA
volatile INT16U R_D_Freq 	= 0;									//	Dimmer Frequency	
volatile INT16U R_D_Flags	= 0;									//	Dimmer 16 bit flags	

volatile INT16U R_D_ExpV 	= Default_OP_Volt;						//	Expected Output Voltage Set in Dimmer Register
volatile INT16U R_D_Max_VA 	= Default_Max_VA;						//	Stabilizer Maximum VA
volatile INT16U R_D_HV_Cut 	= Default_HV_Cut;						//	Stabilizer High Voltage Cut w.r.t Input
volatile INT16U R_D_LV_Cut 	= Default_LV_Cut;						//	Stabilizer Low Voltage Cut w.r.t Input

/*
*********************************************************************************************************
*                                       Variables to Hold PHASE - Y parameters after reading
*********************************************************************************************************
*/

volatile INT16U Y_D_Temp 	= 0;									//	Dimmer Mosfet Temperature
volatile INT16U Y_D_IPV 	= 0;									//	Dimmer Input Voltage
volatile INT16U Y_D_OPV 	= Default_OP_Volt;						//	Dimmer Output Voltage
volatile INT16U Y_D_VA 		= Default_Max_VA;						//	Load On Stabilizer in VA
volatile INT16U Y_D_Freq 	= 0;									//	Dimmer Frequency	
volatile INT16U Y_D_Flags	= 0;									//	Dimmer 16 bit flags	

volatile INT16U Y_D_ExpV 	= Default_OP_Volt;						//	Expected Output Voltage Set in Dimmer Register
volatile INT16U Y_D_Max_VA 	= Default_Max_VA;						//	Stabilizer Maximum VA
volatile INT16U Y_D_HV_Cut 	= Default_HV_Cut;						//	Stabilizer High Voltage Cut w.r.t Input
volatile INT16U Y_D_LV_Cut 	= Default_LV_Cut;						//	Stabilizer Low Voltage Cut w.r.t Input


/*
*********************************************************************************************************
*                                       Variables to Hold PHASE - B parameters after reading
*********************************************************************************************************
*/

volatile INT16U B_D_Temp 	= 0;									//	Dimmer Mosfet Temperature
volatile INT16U B_D_IPV 	= 0;									//	Dimmer Input Voltage
volatile INT16U B_D_OPV 	= Default_OP_Volt;						//	Dimmer Output Voltage
volatile INT16U B_D_VA 		= Default_Max_VA;						//	Load On Stabilizer in VA
volatile INT16U B_D_Freq 	= 0;									//	Dimmer Frequency	
volatile INT16U B_D_Flags	= 0;									//	Dimmer 16 bit flags	

volatile INT16U B_D_ExpV 	= Default_OP_Volt;						//	Expected Output Voltage Set in Dimmer Register
volatile INT16U B_D_Max_VA 	= Default_Max_VA;						//	Stabilizer Maximum VA
volatile INT16U B_D_HV_Cut 	= Default_HV_Cut;						//	Stabilizer High Voltage Cut w.r.t Input
volatile INT16U B_D_LV_Cut 	= Default_LV_Cut;						//	Stabilizer Low Voltage Cut w.r.t Input

