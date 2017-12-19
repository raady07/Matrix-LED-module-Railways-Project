#ifndef  MODEM_ROUTER_H
#define  MODEM_ROUTER_H

volatile extern INT8U ModemRecievedDataCopy[];
volatile extern INT16S ModemRecievedDataCopyIndex;
volatile extern UnionTimer ONTime;
volatile extern UnionTimer OFFTime;
volatile extern UnionTimer Y_ONTime;
volatile extern UnionTimer B_ONTime;
volatile extern UnionTimer Y_OFFTime;
volatile extern UnionTimer B_OFFTime;
volatile extern UnionTimer M_Time;
volatile extern BOOL PH3;
volatile extern BOOL ALP;

volatile extern INT16U M_Err;
volatile extern INT8U KP_PW[];
volatile extern INT8U RD_Val;
volatile extern INT8U YD_Val;
volatile extern INT8U BD_Val;
volatile extern INT16U HV_Cut;
volatile extern INT8U LV_Cut;
volatile extern INT16U VA_Max;


#define SINGLE_PHASE 	0
#define THREE_PHASE 	1
#define ALP_DISABLE 	0
#define ALP_ENABLE 		1

#endif // MODEM_ROUTER_H
