/*
*********************************************************************************************************
*                                               uC/OS-II
*                                         The Real-Time Kernel
*
*                               (c) Copyright 2006, Micrium, Weston, FL
*                                          All Rights Reserved
*
*
*                                      Microchip Application Code
*
* File : APP.C
* By   : Eric Shufo
*********************************************************************************************************
*/

#include <includes.h>



/*
*********************************************************************************************************
*                                                CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                VARIABLES
*********************************************************************************************************
*/

OS_STK  AppStartTaskStk[APP_TASK_START_STK_SIZE];
OS_STK  AppUART1TaskStk[UART_TASK_STK_SIZE];
OS_STK  AppUART2TaskStk[UART_TASK_STK_SIZE];
OS_STK  AppUART3TaskStk[UART_TASK_STK_SIZE];
OS_STK  AppUART4TaskStk[UART_TASK_STK_SIZE];

//extern UnionTimer Sys_Time;


/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppStartTask(void *p_arg);
static  void  AppTaskCreate(void);


OS_EVENT * UART1MboxPtr = NULL;
OS_EVENT * UART2MboxPtr = NULL;
OS_EVENT * UART3MboxPtr = NULL;
OS_EVENT * UART4MboxPtr = NULL;
//OS_EVENT * SPIMboxPtr = NULL;
OS_EVENT * I2CMboxPtr = NULL;
//OS_EVENT * LCDMboxPtr = NULL;



/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.
* Arguments   : none
*********************************************************************************************************
*/

CPU_INT16S  main (void)
{
    CPU_INT08U  err;


    BSP_IntDisAll();                                                    /* Disable all interrupts until we are ready to accept them */

    OSInit();                                                           /* Initialize "uC/OS-II, The Real-Time Kernel"              */

    OSTaskCreateExt(AppStartTask,
                    (void *)0,
                    (OS_STK *)&AppStartTaskStk[0],
                    APP_TASK_START_PRIO,
                    APP_TASK_START_PRIO,
                    (OS_STK *)&AppStartTaskStk[APP_TASK_START_STK_SIZE-1],
                    APP_TASK_START_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

#if OS_TASK_NAME_SIZE > 11
    OSTaskNameSet(APP_TASK_START_PRIO, (CPU_INT08U *)"Start Task", &err);
#endif

    OSStart();                                                          /* Start multitasking (i.e. give control to uC/OS-II)       */

	return (-1);                                                        /* Return an error - This line of code is unreachable       */
}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppStartTask()' by 'OSTaskCreate()'.
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*               2) Interrupts are enabled once the task start because the I-bit of the CCR register was
*                  set to 0 by 'OSTaskCreate()'.
*********************************************************************************************************
*/



static  void  AppStartTask (void *p_arg)
{
    (void)p_arg;
    BSP_Init();                                                         /* Initialize BSP functions                                 */

	#if OS_TASK_STAT_EN > 0
	    OSStatInit();                                                       /* Determine CPU capacity                                   */
	#endif
	UART1MboxPtr = OSMboxCreate((void *)0);
	UART2MboxPtr = OSMboxCreate((void *)0);
	UART3MboxPtr = OSMboxCreate((void *)0);
   

	Remap_UART();	    
    Debug_Init();
    AppTaskCreate();                                                    	/* Create additional user tasks                             */
	Debug_PutString("\n\n\r START\n\r");
	TRISB = 0XFFFF;
	DDR_DR = 1;
	AD1CON1bits.ADON	= 0;												//	Analog mode diabled
	AD1PCFG 	= 	0xFFFF;													//	Configure analog channel to digital mode	
	PMCONbits.PMPEN = 0;													//	Configure PORTB as Input (Sensors)
    OSTaskDel(APP_TASK_START_PRIO)  ;										//	Delete Startup Task;
}


/*
*********************************************************************************************************
*                              CREATE ADDITIONAL APPLICATION TASKS
*********************************************************************************************************
*/

static  void  AppTaskCreate (void)
{
    CPU_INT08U  err;
 

	    
    OSTaskCreateExt(AppDebugTask,
                    (void *)0,
                    (OS_STK *)&AppUART2TaskStk[0],
                    OS_UART2_TASK_PRIO,
                    OS_UART2_TASK_ID,
                    (OS_STK *)&AppUART2TaskStk[UART_TASK_STK_SIZE-1],
                    UART_TASK_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

	#if OS_TASK_NAME_SIZE > 11
    	OSTaskNameSet(OS_UART2_TASK_PRIO, (CPU_INT08U *)"Debug Task", &err);
	#endif 
 	

 	OSTaskCreateExt(AppMCommTask,// Master communication
                    (void *)0,
                    (OS_STK *)&AppUART1TaskStk[0],
                    OS_UART1_TASK_PRIO,
                    OS_UART1_TASK_ID,
                    (OS_STK *)&AppUART1TaskStk[UART_TASK_STK_SIZE-1],
                    UART_TASK_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

	#if OS_TASK_NAME_SIZE > 11
    	OSTaskNameSet(OS_UART1_TASK_PRIO, (CPU_INT08U *)"MComm Task", &err);
	#endif	
}
