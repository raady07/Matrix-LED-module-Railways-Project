#ifndef  app_H
#define  app_H


//#define BAUDRATE3   			9600							//	Single Phase Energy Meter


#define BAUDRATE3   			19200							//	Three Phase Energy Meter




/*
*********************************************************************************************************
*                                       DIMMER COMMUNICATION
*********************************************************************************************************
*/

//#define	Debug_Task									//	To display Debug information on PC

/*
*********************************************************************************************************
*                                       Energy Meter COMMUNICATION
*********************************************************************************************************
*/

//#define EM_DEBUG
/*
*********************************************************************************************************
*                                       MODEM COMMUNICATION
*********************************************************************************************************
*/


//#define M_DEBUG									//	To Debug Modem Communication Task (UART1.C Modem_PutChar)
//#define M_Rx_DEBUG								//	To Debug Data received from Modem in ISR w.r.t Modem Task (UART1.C Modem_PutChar)

/*
*********************************************************************************************************
*                                       LOAD CONTROL
*********************************************************************************************************
*/


#define L_DEBUG									//	To Debug LOAD CONTROL In detail (UART2.C Debug_PutChar)


/*
*********************************************************************************************************
*                                       DIMMER COMMUNICATION
*********************************************************************************************************
*/

//#define	Debug_DIM_Tx								//	To Debug data transfer from uC to Dimmer on Debug Port.
//#define	Debug_Dim_Resp								//	To Debug data Cmd responce received from Dimmer to uC on Debug Port.	
//#define	Debug_DIM_Rx							//	To Debug data transfer from Dimmer to uC on Debug Port.	
//#define	P_Debug										//	To Debug Dimmer Task

#define Adj_Volts				5					//	Buck Boost Voltage Value
#define Rd_DATA					1
#define Wt_DATA					0
#define	NO_Resp_Seq				25					//	No of Counts to break Dimmer communication on No(Integer) responces from dimmer after parsceing
#define MAX_WT_CMD_INDX   		5                   // 	Max write command Buff index    
#define MAX_WT_OPV_CMD_INDX		3         			// 	Max write command OP Voltage Buff index
#define MAX_WT_ALL_CMD_INDX		6         			// 	Max write command ALL Commands Buff index
#define DIMM_COMM_ENABLE_TIME  	150   				//  3 Minutes Computation (180S/1.5S ) 

#define	Default_OP_Volt			210					//	Default Output Voltage
#define	Default_HV_Cut			295					//	Default High Voltage Cutt-off Value	
#define HV_Cut_Min_Range		250					//	HV Cut Minimum Value
#define HV_Cut_Max_Range		300					//	HV Cut Maximum Value	
#define	Default_LV_Cut			170					//	Default Low Voltage Cutt-off Value
#define LV_Cut_Min_Range		160					//	LV Cut Minimum Value
#define LV_Cut_Max_Range		190					//	LV Cut Maximum Value	
#define Min_Dim_OP_Volt			180					//	Minimum Dimmer Output Voltage after Dimming
#define Max_Dim_OP_Volt			220					//	Maximum Dimmer Output Voltage after Dimming
#define Default_Max_VA 			8000				//	Stabilizer Maximum VA as per master
#define	Tr_BB_Voltage			50					//	Transformer Buck Boost Voltage	


#define R_D_M_HV_Cut 			Default_HV_Cut		//	Stabilizer High Voltage Cut w.r.t Input as per master
#define R_D_M_LV_Cut 			Default_LV_Cut		//	Stabilizer Low Voltage Cut w.r.t Input as per master

#define Y_D_M_HV_Cut 			Default_HV_Cut		//	Stabilizer High Voltage Cut w.r.t Input as per master
#define Y_D_M_LV_Cut 			Default_LV_Cut		//	Stabilizer Low Voltage Cut w.r.t Input as per master

#define B_D_M_HV_Cut 			Default_HV_Cut		//	Stabilizer High Voltage Cut w.r.t Input as per master
#define B_D_M_LV_Cut 			Default_LV_Cut		//	Stabilizer Low Voltage Cut w.r.t Input as per master

#define R_D_M_Max_VA 			Default_Max_VA		//	Stabilizer Maximum VA as per master
#define Y_D_M_Max_VA 			Default_Max_VA		//	Stabilizer Maximum VA as per master
#define B_D_M_Max_VA  			Default_Max_VA		//	Stabilizer Maximum VA as per master



/*
*********************************************************************************************************
*                                       EXTERNAL EEPROM
*	EEPROM 				: 	24LC256	
*	Operating Voltage	:	2.5V to 5.5V
*	Memory Size			: 	( 32K * 8 )256 Kilo Bits
*	Number of Pages		:	4
*	Size of Page		:	64 Bytes
*	Write Protect		: 	Disabled in Hardware Temporarly
*	No of Write Cycles	: 	1 Million Erase/Write Cycles
*	Device Address		:	0xA0
*	Definition Files	:	
*********************************************************************************************************
*/

//#define EPPROM_Debug								//	To Debug EEPROM WRITE OPERATIONS
//#define	EEP_RW_Debug								//	To Debug EEPROM Read OPERATIONS

#define	EEPROM_READ		0xA1						//	Read Command
#define	EEPROM_WRITE	0xA0						//	Write Command
#define EEPROM_PAGE1	0x00						//	
#define EEPROM_PAGE2	0x40
#define EEPROM_PAGE3	0x80
#define EEPROM_PAGE4	0xC0

typedef union INT8_16U{	INT16U Val;	struct	{		INT8U A;		INT8U B;	};}INT8_16U;


#endif

