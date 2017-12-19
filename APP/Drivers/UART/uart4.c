#include <includes.h>
#include <queue.h>

CPU_INT08U uartRx4Buffer[UART4_BUFF_SIZE];
CPU_INT08U uartTx4Buffer[UART4_BUFF_SIZE];
CPU_INT16S Read_BytesRecieved(void);

QUEUE uart4RxQueue =       { 0, 0, uartRx4Buffer, (UART4_BUFF_SIZE - 1) }; 

CPU_INT08U uartTx4Buffer[UART4_BUFF_SIZE];

QUEUE uart4TxQueue =       { 0, 0, uartTx4Buffer, (UART4_BUFF_SIZE - 1) }; 
   
      // Put  Get  BuffPtr  Mask 
      // ---  ---  -------  ----

/*****************************************************************************
 * Function: Dimm_Comm_Init
 *
 * Precondition: None.
 *
 * Overview: Setup UART4 module.
 *
 * Input: None.
 *
 * Output: None.
 *
 *****************************************************************************/

void Dimm_Comm_Init()
{
	DIMM_COMM_TX_TRIS = 0;
	DIMM_COMM_RX_TRIS = 1;
	U4MODE = 0;
	U4STA = 0;
//	U4MODEbits.BRGH = 1;
	U4BRG = BAUDRATEREG4;//207;	
		
	U4MODEbits.UARTEN = 1;
	U4STAbits.UTxEN = 1;
	
	IEC5bits.U4RXIE = 1;
	IPC22bits.U4RXIP = 6;  			//  Rx Priority DIM_COMM = 6, MODEM_COMM = 5, EM_COM = 4
	
	U4STAbits.URXISEL0 = 0; 		// Genrate Int after One Byte is Recieved
	U4STAbits.URXISEL1 = 0;			// Genrate Int after One Byte is Recieved
	
 	IFS5bits.U4RXIF = 0;		  	// Reset RX Intrupt flag
 	IFS5bits.U4TXIF = 0;			// reset Tx Intrupt flag
	InitQ(&uart4RxQueue);
	//InitQ(&uart4TxQueue);
}


/*****************************************************************************
 * Function: Dimm_Comm_PutChar
 *
 * Precondition: Dimm_Comm_Init must be called before.
 *
 * Overview: Wait for free UART transmission buffer and send a byte.
 *
 * Input: Byte to be sent.
 *
 * Output: None.
 *
 *****************************************************************************/

void  Dimm_Comm_PutChar(char Ch)
{   
	#ifdef	Debug_DIM_Tx
   		Debug_PutChar(Ch);									//	Through Data on Debug PORT			
    #endif
    while(U4STAbits.UTXBF == 1);						 	// wait for empty buffer  
      U4TXREG = Ch;        
}

/*****************************************************************************
 * Function: Dimm_Comm_IsPressed
 *
 * Precondition: Dimm_Comm_Init must be called before.
 *
 * Overview: Check if there's a new byte in UART reception buffer.
 *
 * Input: None.
 *
 * Output: Zero if there's no new data received.
 *
 *****************************************************************************/

char Dimm_Comm_IsPressed()
{
    if(IFS5bits.U4RXIF == 1)
        return 1;
    return 0;
}

/*****************************************************************************
 * Function: Dimm_Comm_GetChar
 *
 * Precondition: Dimm_Comm_Init must be called before.
 *
 * Overview: Wait for a byte.
 *
 * Input: None.
 *
 * Output: Byte received.
 *
 *****************************************************************************/
 char Dimm_Comm_GetChar()
{
	char Temp;
    while(IFS5bits.U4RXIF == 0);
    Temp = U4RXREG;
    IFS5bits.U4RXIF = 0;
    return Temp;
}

/*****************************************************************************
 * Function: Dimm_Comm_PutDec
 *
 * Precondition: Dimm_Comm_Init must be called before.
 *
 * Overview: This function converts decimal data into a string
 * and outputs it into UART.
 *
 * Input: Binary data.
 *
 * Output: None.
 *
 *****************************************************************************/
void  Dimm_Comm_PutDec(unsigned char Dec){
unsigned char Res;
    Res = Dec;

    if(Res/100) 
        Dimm_Comm_PutChar(Res/100+'0');
    Res = Res - (Res/100)*100;

    if(Res/10) 
        Dimm_Comm_PutChar(Res/10+'0');
    Res = Res - (Res/10)*10;
 
    Dimm_Comm_PutChar(Res+'0');
}


/*****************************************************************************
 * Function: Dimm_Comm_PutInt
 *
 * Precondition: Dimm_Comm_Init must be called before.
 *
 * Overview: This function converts decimal data into a string
 * and outputs it into UART.
 *
 * Input: Binary data.
 *
 * Output: None.
 *
 *****************************************************************************/
void Dimm_Comm_PutInt(unsigned long i)
{
	char j[10], x = 0;
	if( i == 0 )
		Dimm_Comm_PutChar('0');
	else
	{
		while( i )
		{
			j[x++] = i%10;
			i = i/10;
		}
		while( x-- )
			Dimm_Comm_PutChar( j[x] + 48 );
	}
}


/*****************************************************************************
 * Function: Dimm_Comm_PutString
 *
 * Precondition: Dimm_Comm_Init must be called before.
 *
 * Overview: Call Dimm_Comm_PutChar till pointer encounter EOS
 *
 * Input: String to be sent.
 *
 * Output: None.
 *
 *****************************************************************************/

void  Dimm_Comm_PutString(const char *Ch)
{    
	char c;
	while( c = *Ch )
	{
		Dimm_Comm_PutChar(c);
		Ch++;
	} 
}

/*****************************************************************************
 * Function: Dimm_Comm_PutDouble
 *
 * Precondition: Dimm_Comm_Init must be called before.
 *
 * Overview: Call Dimm_Comm_PutString after converting Double to string
 *
 * Input: String to be sent.
 *
 * Output: None.
 *
 *****************************************************************************/
/*
void  Dimm_Comm_PutDouble(double D){

    char temp[20];
    sprintf(temp,"%f",D );
    Dimm_Comm_PutString(temp);
}

*/



void	UART4_RxISRHandler (void)
{
	CPU_INT08U data;
	IFS5bits.U4RXIF = 0; // clear the Rx Interrupt
	while(U4STAbits.URxDA != 0)
	{		
		data = U4RXREG;
		WriteQ8(&uart4RxQueue, data);
		#ifdef	Debug_DIM_Rx
			Debug_PutChar(data);
		#endif	
	}	
}

/*
void Dimm_Comm_Tx_Data(void)
{
	CPU_INT08U RxData;
	CPU_INT16S BytesRecieved = (UART4_BUFF_SIZE - 1) - SpaceOnQ(&uart4RxQueue);
	if (BytesRecieved >= 4 )
	    BytesRecieved = 4;
	while (BytesRecieved > 0)
	{
	   BytesRecieved--;	
	   ReadQ8(&uart4RxQueue, &RxData);
	   Dimm_Comm_PutChar(RxData);
	}
}	
*/
void	UART4_TxISRHandler(void)
{
//	Dummy function to avoid linking error
	/*
	CPU_INT08U TxData;
	int BytesRecieved = (UART4_BUFF_SIZE - 1)  - SpaceOnQ(&uart4RxQueue); // for testing, change uart4RxQueue to uart4TxQueue
	IFS5bits.U4TXIF  = 0;
	if (BytesRecieved >= 4 )
	    BytesRecieved = 4;
	while(BytesRecieved>0)		
	{
	   BytesRecieved--;	
	   ReadQ8(&uart4RxQueue, &TxData); // for testing, change uart4RxQueue to uart4TxQueue
	   Dimm_Comm_PutChar(TxData);
	}	*/
}	


INT16S Read_Dimmer_Data(void)
{
	INT16S BytesRecieved =	Read_BytesRecieved();
	INT8U RxData;
    RxDataIndex = 0;
	while(BytesRecieved)
	{
		ReadQ8(&uart4RxQueue, &RxData);
		RecievedData[RxDataIndex++] = RxData;
		BytesRecieved--;
	}
	return RxDataIndex;	
}	

CPU_INT16S Read_BytesRecieved(void)
{
	CPU_INT16S BytesRecieved = (UART4_BUFF_SIZE - 1) - SpaceOnQ(&uart4RxQueue);
	return BytesRecieved;
}	

/*****************************************************************************
 * EOF
 *****************************************************************************/


