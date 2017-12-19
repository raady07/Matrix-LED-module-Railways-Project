#include <includes.h>
#include <queue.h>

extern QUEUE uart3RxQueue;

//#define DIR_EM_R_W TRISDbits.TRISD8
//#define EM_R_W PORTDbits.RD8

typedef union MBLen
{
	unsigned int Val;
	struct
	{
		unsigned char L;
		unsigned char H;	
	};
}MBLen;

typedef union MBINT
{
	unsigned int Val;
	struct
	{
		unsigned char L;
		unsigned char H;	
	};
}MBINT;

typedef union MBLINT
{
	unsigned long int Val;
	struct
	{
		unsigned char H3;
		unsigned char H2;
		unsigned char H1;
		unsigned char H0;
			
	};
}MBLINT;
typedef union MBF
{
	float Val;
	struct
	{
		unsigned char H3;
		unsigned char H2;
		unsigned char H1;
		unsigned char H0;
			
	};
}MBF;
static void Write_MB(unsigned char *Msg, unsigned char Len);
static unsigned int CalcCRC(unsigned char *Msg,unsigned char Len);
static unsigned int update_crc_16( unsigned int crc, char c );
static void init_crc16_tab( void ) ;

static unsigned int   crc_tab16[256];
static char crc_tab16_init = 0;

unsigned long int EMRegLVal = 0;
unsigned int EMReglVal = 0;
signed int EMRegSLVal = 0;
signed int EMRegVal = 0;
unsigned char EMDetail[8];

INT8U size_resp = 0;
MBLen CRC;
MBINT MBRES0;

static unsigned int CalcCRC(unsigned char *Msg,unsigned char Len)
{
	unsigned int CRCMB = 0xFFFF;
	unsigned char c;
	
	while(Len)
	{
		c= *Msg;
		CRCMB = update_crc_16(CRCMB, c);
		Len--;
		*Msg++;
	}
	return CRCMB;
}

static unsigned int update_crc_16( unsigned int crc, char c )
{
	unsigned int tmp, short_c;
    	
    	short_c = 0x00ff & (unsigned int) c;
    	if ( ! crc_tab16_init ) 
    		init_crc16_tab();
    	tmp =  crc ^ short_c;
   	crc = (crc >> 8) ^ crc_tab16[ tmp & 0xff ];
	return crc;
}  

static void init_crc16_tab( void ) 
{
	int i, j;
    unsigned int crc, c;

	for (i=0; i<256; i++) 
	{
		crc = 0;
		c   = (unsigned int) i;
		for (j=0; j<8; j++) 
		{
			if ( (crc ^ c) & 0x0001 ) 
			{
				crc = ( crc >> 1 ) ^ MB_POLY;
			}
			else
			{
				crc =   crc >> 1;
			}	
			c = c >> 1;
		}
		crc_tab16[i] = crc;
	}
    crc_tab16_init = 1;
}

static void Write_MB(unsigned char *Msg, unsigned char Len)
{
	unsigned char c;
	#ifdef EM_DEBUG
		Debug_PutString("\n\r EM Request :- ");
	#endif
	while(Len)
	{
		c = *Msg;
		ExEM_PutChar(c);
		#ifdef EM_DEBUG
			Debug_PutInt(c);
			Debug_PutString(" ");
		#endif
		Len--;
		*Msg++;
	}
	#ifdef EM_DEBUG
		Debug_PutString("\n\r");
	#endif
}

void ExEM_Send_Cmd(unsigned int Pmtr,unsigned char ADDR,unsigned char FUNC_TYPE)
{
	unsigned char Req, MBCMD[15] = {0};
	MBLen Fun, Len;

	Fun.Val = Pmtr;
	MBCMD[0] = ADDR;
	MBCMD[1] = FUNC_TYPE;
	MBCMD[2] = Fun.H;
	MBCMD[3] = Fun.L;
	
	if(PH3 == FALSE)
	{
		switch(Pmtr)
		{
	
			case EM_Ex_KWH_1:	Len.Val = 2;
								size_resp = 9;
								Req = 8;
								break;
			case EMV1_1:
			case EMA1_1:
			case EMKVA_1:
			case EMKW_1:
			case EMPF_1: 
			case EMFREQ_1:	Len.Val = 1;
							size_resp = 7;
							Req = 8;
							break;
			default:
					break;				
	
		}
	}
	if(PH3 == TRUE)
	{
		
		if(FUNC_TYPE == READ_HOLDING_3)
		{
			Len.Val = 5;
			size_resp = 15;
			Req = 8;
		}
		else
		{
			switch(Pmtr)
			{
				case EMV1_3:
				case EMV2_3:
				case EMV3_3:
				case EMA1_3:
				case EMA2_3:
				case EMA3_3:
				case EMPF1_3:
				case EMPF2_3:
				case EMPF3_3:
				case EMPF_3:
				case EM_Im_KWH_3:
				case EM_Ex_KWH_3:	Len.Val = 2;
									size_resp = 9;
									Req = 8;
									break;
			}
		}
	}

	MBCMD[4] = Len.H;
	MBCMD[5] = Len.L;
	CRC.Val = CalcCRC(MBCMD,(Req - 2));
	MBCMD[6] = CRC.L;
	MBCMD[7] = CRC.H;
	Write_MB(MBCMD,Req);
}

INT8U Read_ExEM_Resp(void)
{
	MBLINT MBRES1;
	MBF MBFRES1;
	INT8U MBCMD[15] = {0}, i,RxData;
	int BytesRecieved = (UART3_BUFF_SIZE - 1)  - SpaceOnQ(&uart3RxQueue);

    for(i = 0;i< BytesRecieved;i++)
	{
		ReadQ8(&uart3RxQueue, &RxData);
		MBCMD[i] = RxData;
	}
	#ifdef EM_DEBUG
		Debug_PutString("\n\r EM Responce :- ");
		for(i = 0;i< BytesRecieved;i++)
		{
			Debug_PutInt(MBCMD[i]);
			Debug_PutString(" ");				
		}
		Debug_PutString("\n\r");
	#endif
	CRC.Val = CalcCRC(MBCMD,(size_resp - 2));
	if(MBCMD[size_resp - 2] == CRC.L && MBCMD[size_resp - 1] == CRC.H)	
	{
		if(MBCMD[2] == 2)
		{
			MBRES0.H = MBCMD[3];
			MBRES0.L = MBCMD[4];			
			EMRegVal = MBRES0.Val;
			return 1;													//	16 Bit Data Received
		}
		else if(MBCMD[2] == 4)
		{
			MBRES1.H3 = MBCMD[6];
			MBRES1.H2 = MBCMD[5];
			MBRES1.H1 = MBCMD[4];
			MBRES1.H0 = MBCMD[3];
			
			MBFRES1.H3 = MBCMD[6];
			MBFRES1.H2 = MBCMD[5];
			MBFRES1.H1 = MBCMD[4];
			MBFRES1.H0 = MBCMD[3];
			
			
		
			if(PH3 == FALSE)
				EMRegLVal = MBRES1.Val;
			else if(PH3 == TRUE)	
				EMReglVal =(unsigned int) MBFRES1.Val;
			return 2;													//	32 Bit Data Received
		}
		else if(MBCMD[2] == 8)
		{
			for(i=0;i<8;i++)
				EMDetail[i] = 0;
			EMDetail[0] = MBCMD[3];
			EMDetail[1] = MBCMD[4];
			EMDetail[2] = MBCMD[5];
			EMDetail[3] = MBCMD[6];
			EMDetail[4] = MBCMD[7];
			EMDetail[5] = MBCMD[8];
			EMDetail[6] = MBCMD[9];
			EMDetail[7] = MBCMD[10];
			return 3;												//	8 Bytes Data Received
		}
		return 0;
	}	
	else															//Broken Packet...
	{
		return 0;
	}
}

