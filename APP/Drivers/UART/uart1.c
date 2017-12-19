#include <includes.h>
#include <queue.h>

INT8U uartRx1BufferTop[UART1_BUFF_SIZE];
QUEUE uart1RxQueueTop =       { 0, 0, uartRx1BufferTop, (UART1_BUFF_SIZE - 1) };

INT8U uartRx1BufferMiddle[UART1_BUFF_SIZE];
QUEUE uart1RxQueueMiddle =       { 0, 0, uartRx1BufferMiddle, (UART1_BUFF_SIZE - 1) };

INT8U uartRx1BufferBottom[UART1_BUFF_SIZE];
QUEUE uart1RxQueueBottom =       { 0, 0, uartRx1BufferBottom, (UART1_BUFF_SIZE - 1) };


INT8U	top_line_complete = 0;
INT8U	start_of_top_line = 0;
INT8U	middle_line_complete = 0;
INT8U	start_of_middle_line = 0;
INT8U	bottom_line_complete = 0;
INT8U	start_of_bottom_line = 0;
INT16U Uart1RxDataIndexTop = 0;
INT16U Uart1RxDataIndexMiddle = 0;
INT16U Uart1RxDataIndexBottom = 0;

INT8U TopRxData[UART1_BUFF_SIZE];
INT8U MiddleRxData[UART1_BUFF_SIZE];
INT8U BottomRxData[UART1_BUFF_SIZE];
/*****************************************************************************
 * Function: Modem_Init
 *
 * Precondition: None.
 *
 * Overview: Setup UART1 module.
 *
 * Input: None.
 *
 * Output: None.
 *
 *****************************************************************************/

void Modem_Init()
{
    // Set directions of UART IOs
	MODEM_TX_TRIS = 0;
	MODEM_RX_TRIS = 1;
	U1MODE = 0;
	U1STA = 0;
//	U1MODEbits.BRGH = 1;
	U1BRG = BAUDRATEREG1;//207;
	U1MODEbits.UARTEN = 1;
	U1STAbits.UTxEN = 1;
	#ifdef HANDSHAKE
		U1MODEbits.UEN1 = 1;
		U1MODEbits.UEN0 = 0;
	#endif
	IEC0bits.U1RXIE = 1;
	IPC2bits.U1RXIP = 5; 			//  Rx Priority DIM_COMM = 6, MODEM_COMM = 5, EM_COM = 4.

	U1STAbits.URXISEL0 = 0; 		// Genrate Int after One Byte is Recieved
	U1STAbits.URXISEL1 = 0;			// Genrate Int after One Byte is Recieved

 	IFS0bits.U1RXIF = 0;		  	// reset RX Intrupt flag
 	IFS0bits.U1TXIF = 0;			// reset TX Intrupt flag
 	InitQ(&uart1RxQueueTop);
 	InitQ(&uart1RxQueueMiddle);
 	InitQ(&uart1RxQueueBottom);
}



/*****************************************************************************
 * Function: Modem_PutChar
 *
 * Precondition: Modem_Init must be called before.
 *
 * Overview: Wait for free UART transmission buffer and send a byte.
 *
 * Input: Byte to be sent.
 *
 * Output: None.
 *
 *****************************************************************************/

void  Modem_PutChar(char Ch){
    // wait for empty buffer
    while(U1STAbits.UTXBF == 1);
      U1TXREG = Ch;
    #ifdef M_DEBUG
    	Debug_PutChar(Ch);
    #endif  
}

/*****************************************************************************
 * Function: Modem_IsPressed
 *
 * Precondition: Modem_Init must be called before.
 *
 * Overview: Check if there's a new byte in UART reception buffer.
 *
 * Input: None.
 *
 * Output: Zero if there's no new data received.
 *
 *****************************************************************************/

char Modem_IsPressed()
{
    if(IFS0bits.U1RXIF == 1)
        return 1;
    return 0;
}

/*****************************************************************************
 * Function: Modem_GetChar
 *
 * Precondition: Modem_Init must be called before.
 *
 * Overview: Wait for a byte.
 *
 * Input: None.
 *
 * Output: Byte received.
 *
 *****************************************************************************/
 char Modem_GetChar()
{
	char Temp;
    while(IFS0bits.U1RXIF == 0);
    Temp = U1RXREG;
    IFS0bits.U1RXIF = 0;
    return Temp;
}

/*****************************************************************************
 * Function: Modem_PutDec
 *
 * Precondition: Modem_Init must be called before.
 *
 * Overview: This function converts decimal data into a string
 * and outputs it into UART.
 *
 * Input: Binary data.
 *
 * Output: None.
 *
 *****************************************************************************/
void  Modem_PutDec(unsigned char Dec){
unsigned char Res;
    Res = Dec;

    if(Res/100)
        Modem_PutChar(Res/100+'0');
    Res = Res - (Res/100)*100;

    if(Res/10)
        Modem_PutChar(Res/10+'0');
    Res = Res - (Res/10)*10;

    Modem_PutChar(Res+'0');
}
/*****************************************************************************
 * Function: Modem_PutInt
 *
 * Precondition: Modem_Init must be called before.
 *
 * Overview: This function converts decimal data into a string
 * and outputs it into UART.
 *
 * Input: Binary data.
 *
 * Output: None.
 *
 *****************************************************************************/
void Modem_PutInt(unsigned long i)
{
	char j[10], x = 0;
	if( i == 0 )
		Modem_PutChar('0');
	else
	{
		while( i )
		{
			j[x++] = i%10;
			i = i/10;
		}
		while( x-- )
			Modem_PutChar( j[x] + 48 );
	}
}

/*****************************************************************************
 * Function: Modem_PutString
 *
 * Precondition: Modem_Init must be called before.
 *
 * Overview: Call Modem_PutChar till pointer encounter EOS
 *
 * Input: String to be sent.
 *
 * Output: None.
 *
 *****************************************************************************/

void  Modem_PutString(char *Ch)
{    
    char c;
    while( (c = *Ch) )
    {
    	Modem_PutChar(c);
    	Ch++;
    }        
}




INT16S Read_Rx_Data(void)
{
	INT16S BytesRecieved;
	INT8U RxData;
	if(top_line_complete == 1)
	{
		top_line_complete = 0;
		start_of_top_line = 0;
		
		BytesRecieved =	Read_Uart1BytesRecieved(1);
    	Uart1RxDataIndexTop = 0;
		while(BytesRecieved)
		{
			ReadQ8(&uart1RxQueueTop, &RxData);
			TopRxData[Uart1RxDataIndexTop++] = RxData;
			BytesRecieved--;
		}
	}	
	if(middle_line_complete == 1)
	{
		middle_line_complete = 0;
		start_of_middle_line = 0;
		
		BytesRecieved =	Read_Uart1BytesRecieved(2);
    	Uart1RxDataIndexMiddle = 0;
		while(BytesRecieved)
		{
			ReadQ8(&uart1RxQueueMiddle, &RxData);
			MiddleRxData[Uart1RxDataIndexMiddle++] = RxData;
			BytesRecieved--;
		}
	}
	if(bottom_line_complete == 1)
	{
		bottom_line_complete = 0;
		start_of_bottom_line = 0;
		
		BytesRecieved =	Read_Uart1BytesRecieved(3);
    	Uart1RxDataIndexBottom = 0;
		while(BytesRecieved)
		{
			ReadQ8(&uart1RxQueueBottom, &RxData);
			BottomRxData[Uart1RxDataIndexBottom++] = RxData;
			BytesRecieved--;
		}
	}	
    return 1;
}

INT16S Read_Uart1BytesRecieved(INT8U T_M_B)
{
	INT16S BytesRecieved = 0;
	if(T_M_B == 1)
	{
		BytesRecieved = (UART1_BUFF_SIZE - 1) - SpaceOnQ(&uart1RxQueueTop);
	}	
	else if(T_M_B == 1)
	{
		BytesRecieved = (UART1_BUFF_SIZE - 1) - SpaceOnQ(&uart1RxQueueMiddle);
	}
	else if(T_M_B == 1)
	{
		BytesRecieved = (UART1_BUFF_SIZE - 1) - SpaceOnQ(&uart1RxQueueBottom);
	}	
	return BytesRecieved;
}

void	UART1_RxISRHandler (void)
{
	INT8U data;
	IFS0bits.U1RXIF = 0; // clear the Rx Interrupt
	while(U1STAbits.URxDA != 0)
	{
		data = U1RXREG;
		if(data == 0x41)
		{
			start_of_top_line = 1;
		}	
		if((start_of_top_line == 1) && (data == 0x5A))
		{
			top_line_complete = 1;	
		}
		if(data == 0x42)
		{
			start_of_middle_line = 1;
		}	
		if((start_of_middle_line == 1) && (data == 0x5A))
		{
			middle_line_complete = 1;	
		}
		if(data == 0x43)
		{
			start_of_bottom_line = 1;
		}	
		if((start_of_bottom_line == 1) && (data == 0x5A))
		{
			bottom_line_complete = 1;	
		}				  
		if(start_of_top_line == 1)
		{
			WriteQ8(&uart1RxQueueTop, data);
		}
		else if (start_of_middle_line == 1)
		{
			WriteQ8(&uart1RxQueueMiddle, data);	 
		}
		else if (start_of_bottom_line == 1)
		{
			WriteQ8(&uart1RxQueueBottom, data);	 
		}
			
		#ifdef	M_Rx_DEBUG	
			Debug_PutChar(data);
		#endif
	}
}


void Modem_Tx_Data(void)
{
/*	INT8U RxData;
	INT16S BytesRecieved = (UART1_BUFF_SIZE - 1) - SpaceOnQ(&uart1RxQueue);
	if (BytesRecieved >= 4 )
	    BytesRecieved = 4;
	while (BytesRecieved > 0)
	{
	   BytesRecieved--;
	   ReadQ8(&uart1RxQueue, &RxData);
	}*/
}

void	UART1_TxISRHandler(void)
{
}


/*****************************************************************************
 * EOF
 *****************************************************************************/
