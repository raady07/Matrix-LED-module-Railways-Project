
#ifndef     LOAD_CONT_H
#define     LOAD_CONT_H

#define DR		PORTAbits.RA9		// Door Sensor

#define SR1		!PORTBbits.RB1		// R_SR1
#define SR2		!PORTBbits.RB2		// R_SR2
#define SR3		!PORTBbits.RB3		// R_SR3
#define SR4		!PORTBbits.RB4		// R_SR4
#define SR5		!PORTBbits.RB5		// R_SR5

#define SY1		!PORTBbits.RB6		// R_SY1
#define SY2		!PORTBbits.RB7		// R_SY2
#define SY3		!PORTBbits.RB8		// R_SY3
#define SY4		!PORTBbits.RB9		// R_SY4
#define SY5		!PORTBbits.RB10		// R_SY5


#define SB1		!PORTBbits.RB11		// R_SY1
#define SB2		!PORTBbits.RB12		// R_SY2
#define SB3		!PORTBbits.RB13		// R_SY3
#define SB4		!PORTBbits.RB14		// R_SY4
#define SB5		!PORTBbits.RB15		// R_SY5


#define ON_HH		18				// Default On Time Hrs.
#define ON_MM		00				// Default On Time Mins.

#define OFF_HH		6				// Default OFF Time Hrs.
#define OFF_MM		0				// Default OFF Time Mins.

#define DS_HH 		19				// Default Dim Start Hrs
#define DS_MM 		0 				// Default Dim Start Mins.

#define DE_HH 		5				// Default Dim End Hrs
#define DE_MM 		0				// Default Dim End Mins.

#define S1_HH 		22				// Default Slot1 Start Hrs
#define S1_MM 		0 				// Default Slot1 Start Mins.

#define S2_HH 		23				// Default Slot2 Start Hrs
#define S2_MM 		0 				// Default Slot2 Start Mins.

#define	ON			1
#define	OFF			0

#define OUTPUT		0
#define	INPUT		1

#define	ENABLE		1
#define DISABLE		0



#define	DDR_DR				TRISAbits.TRISA9					//	PIC_RDO_Pin_72

#define	DDR_A_Set			TRISDbits.TRISD0					//	PIC_RDO_Pin_72
#define	DDR_A_Reset			TRISDbits.TRISD1					//	PIC_RD1_Pin_76						
#define	DDR_B_Set			TRISDbits.TRISD2					//	PIC_RD2_Pin_77
#define	DDR_B_Reset			TRISDbits.TRISD3					//	PIC_RD3_Pin_78						
#define	DDR_C_Set			TRISDbits.TRISD6					//	PIC_RD4_Pin_81
#define	DDR_C_Reset			TRISDbits.TRISD7					//	PIC_RD5_Pin_82						
#define	DDR_D_Set			TRISDbits.TRISD4					//	PIC_RD6_Pin_83
#define	DDR_D_Reset			TRISDbits.TRISD5					//	PIC_RD7_Pin_84

#define	DDR_LATCH1			TRISDbits.TRISD12					//	PIC_RD14_Pin_47
#define	DDR_LATCH2			TRISDbits.TRISD8					//	PIC_RD15_Pin_48	

#define LATCH_1 			LATDbits.LATD12
#define LATCH_2 			LATDbits.LATD8

#define	A_Set				LATDbits.LATD0
#define	A_Reset				LATDbits.LATD1								
#define	B_Set				LATDbits.LATD2
#define	B_Reset				LATDbits.LATD3								
#define	C_Set				LATDbits.LATD6
#define	C_Reset				LATDbits.LATD7

//#define	D_Set			LATDbits.LATD4
//#define	D_Reset			LATDbits.LATD5							

void Reset_Z(void);
void Init_Load_SW(void);
void ON_OFF_Switch(void);
void Init_ONOFF_TIME(void);
INT8U Dim_Slots(void);


#endif  //LOAD_CONT_H

