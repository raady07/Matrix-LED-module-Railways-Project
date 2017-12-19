/*****************************************************************************
 *
 * UART Driver for PIC24.
 *
 *****************************************************************************
 * FileName:        uart_debug.c
 * Dependencies:    
 * Processor:       PIC24
 * Compiler:       	MPLAB C30
 * Linker:          MPLAB LINK30
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the "Company") is intended and supplied to you, the Company's
 * customer, for use solely and exclusively with products manufactured
 * by the Company. 
 *
 * The software is owned by the Company and/or its supplier, and is 
 * protected under applicable copyright laws. All rights are reserved. 
 * Any use in violation of the foregoing restrictions may subject the 
 * user to criminal sanctions under applicable laws, as well as to 
 * civil liability for the breach of the terms and conditions of this 
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES, 
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT, 
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR 
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * A simple UART polled driver 
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Anton Alkhimenok		10/18/05	...
 * Brant Ivey			 3/14/06	Added support for PIC24FJ64004 PIM
 *****************************************************************************/

/*****************************************************************************
 * U2BRG register value and baudrate mistake calculation
 *****************************************************************************/

#include <includes.h>
#include <queue.h>

#define UART2_DEBUG

CPU_INT08U uart2RxBuffer[UART2_BUFF_SIZE];

QUEUE uart2RxQueue =       { 0, 0, uart2RxBuffer, (UART2_BUFF_SIZE - 1) }; 
   
      // Put  Get  BuffPtr  Mask 
      // ---  ---  -------  ----


/*****************************************************************************
 * Function: UART_debugInit
 *
 * Precondition: None.
 *
 * Overview: Setup UART2 module.
 *
 * Input: None.
 *
 * Output: None.
 *
 *****************************************************************************/
void Debug_Init(void)
{
    // Set directions of UART IOs
    
#ifdef UART2_DEBUG   
    DEBUG_TX_TRIS = 0;
	DEBUG_RX_TRIS = 1;	
    U2BRG = 52;//BAUDRATEREG2;//52;  
	U2MODE = 0;
	U2STA = 0;
	U2MODEbits.UARTEN = 1;
	U2STAbits.UTxEN = 1;
	
	U2STAbits.URXISEL0 = 0; // Genrate Int after One Byte is Recieved
	U2STAbits.URXISEL1 = 0;	// Genrate Int after One Byte is Recieved
	
	U2STAbits.UTXISEL0 = 0;
	U2STAbits.UTXISEL1 = 1;
	
//	IEC1bits.U2RxIE = 1;
//	IPC7bits.U2RXIP = 4;  // Rx Proirity
	
//	IEC1bits.U2TxIE = 1;
//	IPC7bits.U2TXIP = 4;  // Tx Proirity

  	// reset RX flag
 	IFS1bits.U2RxIF = 0;
 	// reset Tx flag
 	IFS1bits.U2TxIF = 0;
#endif
	InitQ(&uart2RxQueue);
}

/*****************************************************************************
 * Function: Debug_PutChar
 *
 * Precondition: Debug_PutChar must be called before.
 *
 * Overview: Wait for free UART transmission buffer and send a byte.
 *
 * Input: Byte to be sent.
 *
 * Output: None.
 *
 *****************************************************************************/
void  Debug_PutChar(char Ch){
    // wait for empty buffer
    
#ifdef UART2_DEBUG     
    while(U2STAbits.UTXBF == 1);
      U2TXREG = Ch;
#endif

/*//Modem Debug
#ifdef UART1_DEBUG
    while(U1STAbits.UTXBF == 1);
      U1TXREG = Ch;
#endif
*/
}

/*****************************************************************************
 * Function: Debug_PutString
 *
 * Precondition: UART2Init must be called before.
 *
 * Overview: Call UART2PutChar till pointer encounter EOS
 *
 * Input: String to be sent.
 *
 * Output: None.
 *
 *****************************************************************************/

void  Debug_PutString(char *Ch)
{
    // call UART_debugPutChar till end of string
    char c;
    while( (c = *Ch) )
    {
	    Debug_PutChar(c);
	    Ch++;
    }
        
}


/*****************************************************************************
 * Function: UART_debugIsPressed
 *
 * Precondition: UART_debugInit must be called before.
 *
 * Overview: Check if there's a new byte in UART reception buffer.
 *
 * Input: None.
 *
 * Output: Zero if there's no new data received.
 *
 *****************************************************************************/
char Debug_Pressed()
{
#ifdef UART2_DEBUG   
    if(IFS1bits.U2RxIF == 1)
        return 1;
#endif
#ifdef UART1_DEBUG 
    if(IFS0bits.U1RxIF == 1)
        return 1;
#endif
    return 0;

}

/*****************************************************************************
 * Function: UART_debugGetChar
 *
 * Precondition: UART2Init must be called before.
 *
 * Overview: Wait for a byte.
 *
 * Input: None.
 *
 * Output: Byte received.
 *
 *****************************************************************************/
char Debug_GetChar()
{
	char Temp;
	#ifdef UART2_DEBUG 
		while(IFS1bits.U2RxIF == 0);
		Temp = U2RXREG;
		IFS1bits.U2RxIF = 0;
		
	#endif
	
	#ifdef UART1_DEBUG 
		while(IFS0bits.U1RxIF == 0);
		Temp = U1RXREG;
		IFS0bits.U1RxIF = 0;
	#endif	
	return Temp;
}

/*****************************************************************************
 * Function: UART_debugPutDec
 *
 * Precondition: UART_debugInit must be called before.
 *
 * Overview: This function converts decimal data into a string
 * and outputs it into UART.
 *
 * Input: Binary data.
 *
 * Output: None.
 *
 *****************************************************************************/
void  Debug_PutDec(CPU_INT08U Dec){
	CPU_INT08U Res;
    Res = Dec;
    if(Res/100) 
        Debug_PutChar(Res/100+'0');
    Res = Res - (Res/100)*100;

    if(Res/10) 
        Debug_PutChar(Res/10+'0');
    Res = Res - (Res/10)*10; 
    Debug_PutChar(Res+'0');
}

/*****************************************************************************
 * Function: Debug_PutInt
 *
 * Precondition: Debug_Init must be called before.
 *
 * Overview: This function converts decimal data into a string
 * and outputs it into UART.
 *
 * Input: Binary data.
 *
 * Output: None.
 *
 *****************************************************************************/
void Debug_PutInt(INT32U i)
{
	
	
/*	if( i == 0 )
		Debug_PutChar('0');
	else	
	{
		INT32U temp =	(i & 0xf000) >> 12;
		Debug_PutChar(temp + '0');
		i = i << 4;
		temp =	(i & 0xf00) >> 8;
		Debug_PutChar(temp + '0');
		i = i << 4;
		temp =	(i & 0xf0) >> 4;
		Debug_PutChar(temp + '0');
		i = i << 4;
		temp =	(i & 0xf);
		Debug_PutChar(temp + '0');
	}*/
	char j[10],x = 0;

	if( i == 0 )
		Debug_PutChar('0');
	else
	{
		while( i )
		{
			j[x++] = i%10;
			i = i/10;
		}
		while( x-- )
			Debug_PutChar( j[x] + 48 );
	}

}

void  UART2_RxISRHandler (void)
{
	CPU_INT08U data;
	IFS1bits.U2RxIF = 0; // clear the Rx Interrupt
	while(U2STAbits.URxDA != 0)
	{
		data = U2RXREG;
		WriteQ8(&uart2RxQueue, data);
	}	
	
}	

void Tx_Data(void)
{
	CPU_INT08U RxData;
	CPU_INT16S BytesRecieved = (UART2_BUFF_SIZE - 1) - SpaceOnQ(&uart2RxQueue);
	if (BytesRecieved >= 4 )
	    BytesRecieved = 4;
	while (BytesRecieved > 0)
	{
	   BytesRecieved--;	
	   ReadQ8(&uart2RxQueue, &RxData);
	   Debug_PutChar(RxData);
	}
}	


void UART2_TxISRHandler(void)
{
	CPU_INT08U RxData;
	int BytesRecieved = (UART2_BUFF_SIZE - 1)  - SpaceOnQ(&uart2RxQueue);
	IFS1bits.U2TxIF = 0;
	if (BytesRecieved >= 4 )
	    BytesRecieved = 4;
	while(BytesRecieved>0)		
	{
	   BytesRecieved--;	
	   ReadQ8(&uart2RxQueue, &RxData);
	   Debug_PutChar(RxData);
	}	
}	



/*****************************************************************************
 * EOF
 *****************************************************************************/
