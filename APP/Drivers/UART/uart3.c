#include <includes.h>
#include <queue.h>

CPU_INT08U uart3RxBuffer[UART3_BUFF_SIZE];

QUEUE uart3RxQueue =       { 0, 0, uart3RxBuffer, (UART3_BUFF_SIZE - 1) }; 
   
      // Put  Get  BuffPtr  Mask 
      // ---  ---  -------  ----
/*****************************************************************************
 * Function: ExEM_Init
 *
 * Precondition: None.
 *
 * Overview: Setup ExEM_ module.
 *
 * Input: None.
 *
 * Output: None.
 *
 *****************************************************************************/

void Init_ExEM()
{
  
	ExEM_TX_TRIS = 0;
	ExEM_RX_TRIS = 1;
	U3MODE = 0;
	U3STA = 0;
//	U3MODEbits.BRGH = 1;
	U3BRG = BAUDRATEREG3;//207;	
	U3MODEbits.UARTEN = 1;
	U3STAbits.UTxEN = 1;
	#ifdef HANDSHAKE
		U3MODEbits.UEN1 = 1;	
		U3MODEbits.UEN0 = 0;
	#endif
	IEC5bits.U3RXIE = 1;
	IPC20bits.U3RXIP = 4; 			//  Rx Priority DIM_COMM = 6, MODEM_COMM = 5, EM_COM = 4.

	U3STAbits.URXISEL0 = 0; 		// Genrate Int after One Byte is Recieved
	U3STAbits.URXISEL1 = 0;			// Genrate Int after One Byte is Recieved
	
//	U3STAbits.UTXISEL0 = 0;
//	U3STAbits.UTXISEL1 = 1;
	
 	IFS5bits.U3RXIF = 0;  			// Reset RX Intrupt flag
 	IFS5bits.U3TXIF = 0;   			// Reset Tx Intrupt flag
	InitQ(&uart3RxQueue);
}


/*****************************************************************************
 * Function: ExEM_PutChar
 *
 * Precondition: ExEM_Init must be called before.
 *
 * Overview: Wait for free UART transmission buffer and send a byte.
 *
 * Input: Byte to be sent.
 *
 * Output: None.
 *
 *****************************************************************************/

void  ExEM_PutChar(char Ch){
    // wait for empty buffer  
    while(U3STAbits.UTXBF == 1);
      U3TXREG = Ch;
}

/*****************************************************************************
 * Function: ExEM_IsPressed
 *
 * Precondition: ExEM_Init must be called before.
 *
 * Overview: Check if there's a new byte in UART reception buffer.
 *
 * Input: None.
 *
 * Output: Zero if there's no new data received.
 *
 *****************************************************************************/

char ExEM_IsPressed()
{
    if(IFS5bits.U3RXIF == 1)
        return 1;
    return 0;
}

/*****************************************************************************
 * Function: ExEM_GetChar
 *
 * Precondition: ExEM_Init must be called before.
 *
 * Overview: Wait for a byte.
 *
 * Input: None.
 *
 * Output: Byte received.
 *
 *****************************************************************************/
 char ExEM_GetChar(){
char Temp;
    while(IFS5bits.U3RXIF == 0);
    Temp = U3RXREG;
    IFS5bits.U3RXIF = 0;
    return Temp;
}

/*****************************************************************************
 * Function: ExEM_PutDec
 *
 * Precondition: ExEM_Init must be called before.
 *
 * Overview: This function converts decimal data into a string
 * and outputs it into UART.
 *
 * Input: Binary data.
 *
 * Output: None.
 *
 *****************************************************************************/
void  ExEM_PutDec(unsigned char Dec){
unsigned char Res;
    Res = Dec;

    if(Res/100) 
        ExEM_PutChar(Res/100+'0');
    Res = Res - (Res/100)*100;

    if(Res/10) 
        ExEM_PutChar(Res/10+'0');
    Res = Res - (Res/10)*10;
 
    ExEM_PutChar(Res+'0');
}

/*****************************************************************************
 * Function: ExEM_PutString
 *
 * Precondition: ExEM_Init must be called before.
 *
 * Overview: Call ExEM_PutChar till pointer encounter EOS
 *
 * Input: String to be sent.
 *
 * Output: None.
 *
 *****************************************************************************/

void  ExEM_PutString(char *Ch){
    // call ExEM_PutChar till end of string
    char c;
    while(c = *Ch) 
    {
	    ExEM_PutChar(c);
	    Ch++;
    }    
}

/*****************************************************************************
 * Function: ExEM_PutInt
 *
 * Precondition: Init_ExEM must be called before.
 *
 * Overview: This function converts decimal data into a string
 * and outputs it into UART.
 *
 * Input: Binary data.
 *
 * Output: None.
 *
 *****************************************************************************/
void ExEM_PutInt(unsigned long i)
{
	char j[10],x = 0;

	if( i == 0 )
		ExEM_PutChar('0');
	else
	{
		while( i )
		{
			j[x++] = i%10;
			i = i/10;
		}
		while( x-- )
			ExEM_PutChar( j[x] + 48 );
	}
}


void  UART3_RxISRHandler (void)
{
	CPU_INT08U data;
	IFS5bits.U3RXIF = 0; // clear the Rx Interrupt
	while(U3STAbits.URxDA != 0)
	{
		data = U3RXREG;
		WriteQ8(&uart3RxQueue, data);
	}	
	
}	

void UART3_TxISRHandler(void)
{
	IFS5bits.U3TXIF = 0;
}

/*****************************************************************************
 * EOF
 *****************************************************************************/
