#include <includes.h>
#include "LibraryFunctions.h"






extern INT8U	Task_WDT[5];				//	Definition in app_cfg.c

volatile INT8U ModemRecievedData[UART1_BUFF_SIZE];
volatile INT16S Uart1RxDataIndex = 0;
volatile INT8U ModemRecievedDataCopy[UART1_BUFF_SIZE];
volatile INT16S ModemRecievedDataCopyIndex = 0;

void EEPROM_write(INT8U Address, INT8U *Data, INT8U Len);				//	Definition eeprom_router.c
void EEPROM_read(INT8U Address, INT8U *Data, INT8U Len);				//	Definition eeprom_router.c
void EEPROM_ReadAll(void);

volatile INT8U RTC_DD = 23, RTC_MM = 6, RTC_YY = 11, RTC_WW = 5;

static void Tx2Modem(void);

void AppModemCommTask (void *p_arg)
{
	INT8S * rxMessage;
	INT8U *err;
    INT16S Modem_Rx_Data_Len = 0;
	OpenI2C1( I2C_ON, I2C_BRG ); 
	Modem_Init();
   	while(1)
    {

	    Task_WDT[1] = 1;								//	MODEM
	    rxMessage = (INT8S*)OSMboxPend(UART1MboxPtr, 9000, err);
	    
 		//		if((*rxMessage) == COUNT_1000MS_TIMER)
		{
			Read_Rx_Data();
		}
	}
}

