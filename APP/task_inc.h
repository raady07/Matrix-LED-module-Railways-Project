
#ifndef  TASK_INC_H
#define  TASK_INC_H


#define APP_TAST_ROUTER                         10

#define COUNT_30MS_TIMER                       	3
#define COUNT_100MS_TIMER                       10
#define COUNT_250MS_TIMER						25
#define COUNT_300MS_TIMER						30
#define COUNT_500MS_TIMER						50
#define COUNT_1000MS_TIMER                      100  
#define COUNT_1500MS_TIMER                      150 
#define COUNT_2000MS_TIMER                      200   
#define COUNT_3000MS_TIMER                      300  
#define COUNT_13000MS_TIMER                     1300 						//	 13 Seconds timer

extern OS_EVENT * UART1MboxPtr;
extern OS_EVENT * UART2MboxPtr;
extern OS_EVENT * UART3MboxPtr;
extern OS_EVENT * UART4MboxPtr;
extern OS_EVENT * LCDMboxPtr;


void SendMsgToRouter(void);
void  AppLCDTask(void *p_arg);
void  AppDebugTask(void *p_arg);
void AppMCommTask(void *p_arg);
void AppDimmCommTask(void *p_arg);
void AppModemCommTask(void *p_arg);



#endif 
