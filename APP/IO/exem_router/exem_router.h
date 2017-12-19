#ifndef  ExEM_H
#define  ExEM_H


extern INT16U Ex_V1;
extern INT16U Ex_V2;
extern INT16U Ex_V3;
//extern float Ex_A1 ;
//extern float Ex_A2 ;
//extern float Ex_A3 ;
extern INT16U Ex_A1 ;
extern INT16U Ex_A2;
extern INT16U Ex_A3;
extern INT32U Ex_KWH ;
extern INT32U Ex_Imp_KWH;			//Added by vamsi to test EM KWH values
extern INT32U Ex_KVARH;
extern INT8S Ex_PF1 ;
extern INT8S Ex_PF2 ;
extern INT8S Ex_PF3;
extern INT8S Ex_PF;
extern INT16U Ex_AN ;
extern INT8U ExEMFailureFlg;

//////////////1Ph////////////////////////
	#define	Ex_EM_DEV_ID_1	01
	#define READ_1			03
	#define WRITE_1			10
	#define EMV1_1			0
	#define EMA1_1			1
	#define EMKVA_1			2
	#define EMKW_1			3
	#define EMPF_1			4
	#define EMFREQ_1		5
	#define EM_Ex_KWH_1		6
	#define EMREGCNT_1		6
/////////////////////////////////////////

//////////////3Ph////////////////////////
#define	Ex_EM_DEV_ID_3	01
#define READ_3			04
#define READ_HOLDING_3	03
#define WRITE_3			10

#define SRN_3			0
#define EMV1_3			0
#define EMV2_3			2
#define EMV3_3			4
#define EMA1_3			6
#define EMA2_3			8
#define EMA3_3			10
//#define EMPF1_3			30
//#define EMPF2_3			32
//#define EMPF3_3			34
#define EMPF1_3			36
#define EMPF2_3			38
#define EMPF3_3			40
#define EMPF_3			30					// Not required
#define EM_Im_KWH_3		6420
#define EM_Ex_KWH_3		2182

#define EMREGCNT_3		12
/////////////////////////////////////////

#define MB_POLY 	0xA001

//void ExEM_Send_Cmd(unsigned int Pmtr);
void ExEM_Send_Cmd(unsigned int Pmtr,unsigned char ADDR,unsigned char FUNC_TYPE);
INT8U Read_ExEM_Resp(void);
void Asign_16BitValue(INT16U data);
void Asign_32BitValue(INT16U data);
void Asign_8ByteValue(INT16U data);
#endif

