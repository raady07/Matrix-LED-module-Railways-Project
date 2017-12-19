/*
*********************************************************************************************************
*                              uC/OS-II Application Configuration
*
*                       DO NOT DELETE THIS FILE, IT IS REQUIRED FOR OS_VER > 2.80
*
*                                   CHANGE SETTINGS ACCORDINGLY
*
*
* File : app_cfg.h
* By   : Eric Shufro
*********************************************************************************************************
*/

#ifndef  APP_CFG_H
#define  APP_CFG_H

/*
*********************************************************************************************************
*                                       TASK PRIORITIES
*********************************************************************************************************
*/



#define  APP_TASK_START_PRIO                    0                       /* Lower numbers are of higher priority                     */
#define  OS_TASK_TMR_PRIO                      	10
#define  APP_TASK_LCD_PRIO                      11						//	LCD TASK
#define  OS_UART1_TASK_PRIO                     12						//	Modem Communication
#define  OS_UART4_TASK_PRIO                     13						//	Dimmer Communication	
#define  OS_UART3_TASK_PRIO                     14						//	External_Energy Meter	
#define  OS_UART2_TASK_PRIO                     15						//	Debuger Task 	

/*
*********************************************************************************************************
*                                       TASK ID
*********************************************************************************************************
*/
#define  OS_UART1_TASK_ID                     	1							//	Modem Communcation	
#define  OS_UART2_TASK_ID                     	2							//	Debuger
#define  OS_UART3_TASK_ID                     	3							//	External_Energy Meter				
#define  OS_UART4_TASK_ID                     	4							//	Dimmer Communication	
#define  OS_LCD_TASK_ID                         5							//	LCD TASK
/*
*********************************************************************************************************
*                                       TASK STACK SIZES
*
* Notes :   1) Warming, setting a stack size too small may result in the OS crashing. It the OS crashes
*              within a deep nested function call, the stack size may be to blame. The current maximum
*              stack usage for each task may be checked by using uC/OS-View or the stack checking
*              features of uC/OS-II.
*********************************************************************************************************
*/

#define  APP_TASK_START_STK_SIZE              	128
#define  APP_TASK_LCD_STK_SIZE                	128	
#define  UART_TASK_STK_SIZE                		128

/*
*********************************************************************************************************
*                                       Miscleneous
*********************************************************************************************************
*/
#define  TRUE  1
#define  FALSE 0


void Remap_UART(void);
#endif                                                                  /* End of file                                              */



