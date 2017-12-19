#ifndef  UART_ROUTR_H
#define  UART_ROUTR_H


#define	DDR_MCLR_LED	TRISCbits.TRISC1
#define	DDR_SLOTA_LED	TRISCbits.TRISC2
#define	DDR_SLOTB_LED	TRISCbits.TRISC3
#define	DDR_SLOTC_LED	TRISCbits.TRISC4


#define	DDR_LOAD_LED	TRISDbits.TRISD13
#define	DDR_HLV_LED		TRISDbits.TRISD9
#define	DDR_Err_LED		TRISAbits.TRISA7


#define MCLR_LED 		LATCbits.LATC1
#define SLOTA_LED 		LATCbits.LATC2
#define SLOTB_LED 		LATCbits.LATC3
#define SLOTC_LED 		LATCbits.LATC4
#define LOAD_LED		LATDbits.LATD13
#define HLV_LED			LATDbits.LATD9
#define Err_LED			LATAbits.LATA7

extern INT16U	PS 	;								// Peripheral Status 16 Bit Value
extern INT16U SNRS  ;								// Sensors Status 16 Bit Value
extern INT16U SW_V  ;								// PIC Software Version
extern INT16U CD  	;								// PIC Program Complie Date
extern INT16U POR  	;								// Power On Reset Count
extern INT16U BOR  	;								// Brown Out Reser Count
extern INT16U WDT  	;								// Watch Dog reset count
extern INT16U LVDT	;								// Low Voltage Reset Count
extern INT16U Vcc 	;								// PIC Operating Voltage


void Tx_Data(void);

#endif                                                                  /* End of file                                              */
