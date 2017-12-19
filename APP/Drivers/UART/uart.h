
#ifndef  UART_H
#define  UART_H

#define MODEM_TX_TRIS   		TRISFbits.TRISF13
#define MODEM_RX_TRIS   		TRISFbits.TRISF12


#define UART1_BUFF_SIZE 128

#define BAUDRATE1   			9600
#define BAUDRATEREG1 			SYSCLK/32/BAUDRATE1-1			// BRGH = 0
//#define BAUDRATEREG1 			415			// BRGH = 1
#if BAUDRATEREG1 > 255
#error Cannot set up UART1 for the SYSCLK and BAUDRATE.\
 Correct values in main.h and uart1.h files.
#endif

#define BAUDRATE1_MISTAKE 1000*(BAUDRATE1-SYSCLK/32/(BAUDRATEREG1+1))/BAUDRATE1
#if (BAUDRATE1_MISTAKE > 2)||(BAUDRATE1_MISTAKE < -2)
#error UART1 baudrate mistake is too big  for the SYSCLK\
 and BAUDRATE1. Correct values in uart1.c file.
#endif

//#define HANDSHAKE				//to use RTS and CTS

void Modem_PutDec(unsigned char Dec);
char Modem_GetChar(void);
char Modem_IsPressed(void);
void Modem_PutChar(char Ch);
void Modem_Init(void);
void Modem_PutInt(unsigned long i);
//extern void  Modem_PutDouble(double D);
void Modem_PutString(char *Ch);

void UART1_RxISRHandler(void);
void UART1_TxISRHandler(void);
INT16S Read_Uart1BytesRecieved(INT8U );
INT16S Read_Rx_Data(void);
void Modem_Tx_Data(void);
void Parse_Modem_Data(INT16S Modem_Rx_Data_Len);

volatile extern INT8U ModemRecievedData[];
volatile extern INT16S Uart1RxDataIndex;

#define DEBUG_TX_TRIS   TRISFbits.TRISF5
#define DEBUG_RX_TRIS   TRISFbits.TRISF4


#define BAUDRATE2   			19200
#define BAUDRATEREG2 			SYSCLK/32/BAUDRATE2-1			// BRGH = 0

#if BAUDRATEREG2 > 255
#error Cannot set up UART2 for the SYSCLK and BAUDRATE.\
 Correct values in main.h and uart1.h files.
#endif

#define BAUDRATE2_MISTAKE 1000*(BAUDRATE2-SYSCLK/32/(BAUDRATEREG2+1))/BAUDRATE2
#if (BAUDRATE2MISTAKE > 2)||(BAUDRATE2_MISTAKE < -2)
#error UART2 baudrate mistake is too big  for the SYSCLK\
 and BAUDRATE2. Correct values in uart2.c file.
#endif

#define UART2_BUFF_SIZE 32

void	Debug_Init();
void	Debug_PutChar(char Ch);
char	Debug_IsPressed(void);
char	Debug_GetChar(void);
void	Debug_PutString(char *Ch);
void	Debug_PutDec(unsigned char Dec);
void	UART2_RxISRHandler (void);
void	UART2_TxISRHandler(void);
void Debug_PutInt(INT32U i);


#define ExEM_TX_TRIS   		TRISFbits.TRISF3
#define ExEM_RX_TRIS   		TRISFbits.TRISF2

#define BAUDRATEREG3 			SYSCLK/32/BAUDRATE3-1			// BRGH = 0
//#define BAUDRATEREG2 			415			// BRGH = 1
//#define HANDSHAKE				//to use RTS and CTS

#define UART3_BUFF_SIZE	32

#if BAUDRATEREG3 > 255
	#error Cannot set up UART3 for the SYSCLK and BAUDRATE.\
	 Correct values in main.h and uart3.h files.
#endif

#define BAUDRATE3_MISTAKE 1000*(BAUDRATE3-SYSCLK/32/(BAUDRATEREG3+1))/BAUDRATE3
#if (BAUDRATE3_MISTAKE > 2)||(BAUDRATE3_MISTAKE < -2)
	#error UART3 baudrate mistake is too big  for the SYSCLK\
	 and BAUDRATE3. Correct values in uart3.c file.
#endif



void ExEM_PutDec(unsigned char Dec);
char ExEM_GetChar(void);
char ExEM_IsPressed(void);
void ExEM_PutChar(char Ch);
void Init_ExEM(void);
//void ExEM_PutDouble(double D);
void ExEM_PutString(char *Ch);
void  UART3_RxISRHandler (void);


#define DIMM_COMM_TX_TRIS   		TRISFbits.TRISF8
#define DIMM_COMM_RX_TRIS   		TRISFbits.TRISF7

#define BAUDRATE4   			19200
#define BAUDRATEREG4 			SYSCLK/32/BAUDRATE4-1			// BRGH = 0
//#define BAUDRATEREG2 			415			// BRGH = 1

//#define HANDSHAKE				//to use RTS and CTS


#if BAUDRATEREG4 > 255
#error Cannot set up UART4 for the SYSCLK and BAUDRATE.\
 Correct values in main.h and uart4.h files.
#endif

#define BAUDRATE4_MISTAKE 1000*(BAUDRATE4-SYSCLK/32/(BAUDRATEREG4+1))/BAUDRATE4
#if (BAUDRATE4_MISTAKE > 2)||(BAUDRATE4_MISTAKE < -2)
#error UART4 baudrate mistake is too big  for the SYSCLK\
 and BAUDRATE4. Correct values in uart4.c file.
#endif


#define UART4_BUFF_SIZE 32

void Dimm_Comm_PutDec(unsigned char Dec);
char Dimm_Comm_GetChar(void);
char Dimm_Comm_IsPressed(void);
void Dimm_Comm_PutChar(char Ch);
void Dimm_Comm_Init(void);
//void  Dimm_Comm_PutDouble(double D);
void Dimm_Comm_PutString(const char *Ch);
void	UART4_RxISRHandler (void);
void	UART4_TxISRHandler(void);
void Dimm_Comm_Tx_Data(void);
INT16S Read_Dimmer_Data(void);
void Dimm_Comm_PutInt(unsigned long i);
#endif

