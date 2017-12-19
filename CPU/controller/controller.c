/*
*********************************************************************************************************
*                                             Microchip dsPIC33FJ
*                                            Board Support Package
*
*                                                *********************************************************************************************************
*/

#include <includes.h>

/*
*********************************************************************************************************
*                                         MPLAB CONFIGURATION MACROS
*********************************************************************************************************
*/

    //_CONFIG1(JTAGEN_OFF & FWDTEN_OFF & WINDIS_OFF);                     /* Disable JTAG and watchdog timer. (Enables full use   ... */
                                                                        /* ... port A pins and prevents watchdog timer resets.      */
    /* Disable JTAG and Enable Watchdog timer. FWPSA_PR128 (Prescalar/128) with Time out Period 16.384 Secs (Ratio 1:4096)    */
   _CONFIG1(JTAGEN_OFF & FWDTEN_ON & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS4096); 
 

    _CONFIG2(FNOSC_PRIPLL & POSCMOD_XT);                                /* Select the primary (XT, HS, EC) Oscillator with PLL      */     


/*
*********************************************************************************************************
*                                              VARIABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                              PROTOTYPES
*********************************************************************************************************
*/


static  void  Tmr_TickInit(void);



/*
*********************************************************************************************************
*                                         BSP INITIALIZATION
*
* Description : This function should be called by your application code before you make use of any of the
*               functions found in this module.
*
* Arguments   : none
*********************************************************************************************************
*/

void  BSP_Init (void)
{
    RCON    &= ~SWDTEN;                                                 /* Ensure Watchdog disabled via IDE CONFIG bits and SW.     */
    Tmr_TickInit();                                                     /* Initialize the uC/OS-II tick interrupt                   */
}

/*
*********************************************************************************************************
*                                      BSP_CPU_ClkFrq()

* Description : This function determines the CPU clock frequency (Fcy)
* Returns     : The CPU frequency in (HZ)
*********************************************************************************************************
*/

CPU_INT32U  BSP_CPU_ClkFrq (void)
{
    CPU_INT08U  Clk_Selected;
    CPU_INT16U  FRC_Div;
    CPU_INT32U  CPU_Clk_Frq;


    FRC_Div       = ((CLKDIV & FRCDIV_MASK) >> 8);                      /* Get the FRC Oscillator Divider register value            */
    FRC_Div       = ((1 << FRC_Div) * 2);                               /* Compute the FRC Divider value                            */

    Clk_Selected  =  (OSCCON & COSC_MASK) >> 12;                        /* Determine which clock source is currently selected       */

    switch (Clk_Selected) {
        case 0:                                                         /* Fast Oscillator (FRC) Selected                           */
             CPU_Clk_Frq   =   CPU_FRC_OSC_FRQ;                         /* Return the frequency of the internal fast oscillator     */
             break;

        case 1:                                                         /* Fast Oscillator (FRC) with PLL Selected                  */
             CPU_Clk_Frq   =  (CPU_FRC_OSC_FRQ  * 4);                   /* Compute the PLL output frequency  = (FRC * 4)            */
            break;

        case 2:                                                         /* Primary External Oscillator Selected                     */
             CPU_Clk_Frq   =   CPU_PRIMARY_OSC_FRQ;                     /* Return the frequency of the primary external oscillator  */
             break;

        case 3:                                                         /* Primary External Oscillator with PLL Selected            */
             CPU_Clk_Frq   =  (CPU_PRIMARY_OSC_FRQ * 4);                /* Compute the PLL output frq as (CPU_PRIMARY_OSC_FRQ * 4)  */
             break;

        case 4:                                                         /* Secondary Oscillator Selected (SOCS)                     */
             CPU_Clk_Frq   =   CPU_SECONDARY_OSC_FRQ;                   /* Return the frq of the external secondary oscillator      */
             break;

        case 5:                                                         /* Low Power Oscillator (LPOSC) Selected                    */
             CPU_Clk_Frq   =   CPU_LOW_POWER_OSC_FRQ;                   /* Return the frq of the Low Power Oscillator               */
             break;

        case 6:
             CPU_Clk_Frq = 0;                                           /* Return 0 for the Reserved clock setting                  */
             break;

        case 7:                                                         /* Fast Oscillator (FRC) with FRCDIV Selected               */
             CPU_Clk_Frq   =   CPU_FRC_OSC_FRQ / FRC_Div;               /* Return the clock frequency of FRC / FRC_Div              */
             break;

        default:
             CPU_Clk_Frq = 0;                                           /* Return 0 if the clock source cannot be determined        */
             break;
    }
    
    CPU_Clk_Frq   /=  2;                                                /* Divide the final frq by 2, get the actual CPU Frq (Fcy)  */

    return (CPU_Clk_Frq);                                               /* Return the operating frequency                           */
}

/*
*********************************************************************************************************
*                                     DISABLE ALL INTERRUPTS
*
* Description : This function disables all interrupts from the interrupt controller.
*
* Arguments   : none
*********************************************************************************************************
*/

void  BSP_IntDisAll (void)
{
}

/*
*********************************************************************************************************
*                                         LED I/O INITIALIZATION
*
* Description : This function initializes the I/O Pins used by the onboard LEDs
*
* Arguments   : none
*
* Notes       : 1) Jumper JP2 on the Explorer 16 board must be connected to enable the onboard LEDs
*               2) JTAG must be DISABLED in order to utilize all of PORTA I/O Lines for LEDs
*********************************************************************************************************
*/

void  LED_Init (void)
{
 //   TRISA     =  0;                                                     /* Set all PORTA pins to output                             */
 //   LED_Off(0);                                                         /* Shut off all LEDs                                        */
}

/*
*********************************************************************************************************
*                                             LED ON
*
* Description : This function is used to control any or all the LEDs on the board.
*
* Arguments   : led    is the number of the LED to control
*                      0    indicates that you want ALL the LEDs to be ON
*                      1    turns ON LED1
*                      2    turns ON LED2
*                      ...
*                      8    turns ON LED8
*
* Notes       : 1) Jumper JP2 on the Explorer 16 board must be connected to enable the onboard LEDs
*********************************************************************************************************
*/

void  LED_On (CPU_INT08U led)
{
	return;						// Temp disable
  /*
	    if (led == 0) {
        PORTA |=  0xFF;                                                 // Turn on all of the LEDs if a 0 is passed in              
		return;
		
    }

    if ((led >= 1) && (led <= 8)) {
		led--;                                                          // Convert the LED number to a pin number by subtracting 1	
        PORTA |= (1 << led);                                            // Turn on the chosen LED                                   
    }
   */ 
}

/*
*********************************************************************************************************
*                                             LED OFF
*
* Description : This function is used to control any or all the LEDs on the board.
*
* Arguments   : led    is the number of the LED to turn OFF
*                      0    indicates that you want ALL the LEDs to be OFF
*                      1    turns OFF LED1
*                      2    turns OFF LED2
*                      .
*                      8    turns OFF LED8
*
* Notes       : 1) Jumper JP2 on the Explorer 16 board must be connected to enable the onboard LEDs
*********************************************************************************************************
*/

void  LED_Off (CPU_INT08U led)
{
		return;						// Temp disable
    /*
	    if (led == 0) {
        PORTA &= ~0xFF;                                                 // Turn on all of the LEDs if a 0 is passed in              
		return;
    }

    if ((led >= 1) && (led <= 8)) {
		led--;                                                          // Convert the LED number to a pin number by subtracting 1	
        PORTA &= ~(1 << led);                                           // Turn on the chosen LED                                   
    }
    */
}

/*
*********************************************************************************************************
*                                             LED TOGGLE
*
* Description : This function is used to toggle any or all the LEDs on the board.
*
* Arguments   : led    is the number of the LED to control
*                      0    indicates that you want to toggle ALL the LEDs
*                      1    toggles LED1
*                      2    toggles LED2
*                      .
*                      8    toggles LED8
*
* Notes       : 1) Jumper JP2 on the Explorer 16 board must be connected to enable the onboard LEDs
*********************************************************************************************************
*/

void  LED_Toggle (CPU_INT08U led)
{
	return;
  /* 
	    if (led == 0) {
        PORTA ^=  0xFF;                                                 // Turn on all of the LEDs if a 0 is passed in              
		return;
    }

    if ((led >= 1) && (led <= 8)) {
		led--;                                                          // Convert the LED number to a pin number by subtracting 1	
        PORTA ^= (1 << led);                                            // Turn on the chosen LED                                   
    }
    */
}

/*
*********************************************************************************************************
*                                   OSProbe_TmrInit()
*
* Description : This function is called to by uC/Probe Plug-In for uC/OS-II to initialize the
*               free running timer that is used to make time measurements.
*
* Arguments   : none
*
* Returns     : none
*
* Note(s)     : 1) This timer is shared with the uC/OS-II time tick and is initialized
*                  from Tmr_TickInit().
*********************************************************************************************************
*/

#if (uC_PROBE_OS_PLUGIN > 0) && (OS_PROBE_HOOKS_EN == 1)
void  OSProbe_TmrInit (void)
{
#if OS_PROBE_TIMER_SEL == 3
    T3CON  =   0;                                                       /* Use Internal Osc (Fosc / 4), 16 bit mode, prescaler = 1  */
    TMR3   =   0;                                                       /* Start counting from 0 and clear the prescaler count      */
    PR3    =   0xFFFF;                                                  /* Set the period register to its maximum value             */
    T3CON |=   TON;                                                     /* Start the timer                                          */
#endif

#if OS_PROBE_TIMER_SEL == 5
    T5CON  =   0;                                                       /* Use Internal Osc (Fosc / 4), 16 bit mode, prescaler = 1  */
    TMR5   =   0;                                                       /* Start counting from 0 and clear the prescaler count      */
    PR5    =   0xFFFF;                                                  /* Set the period register to its maximum value             */
    T5CON |=   TON;                                                     /* Start the timer                                          */
#endif
}
#endif

/*
*********************************************************************************************************
*                                   OSProbe_TmrRd()
*
* Description : This function is called to read the current counts of a 16 bit free running timer.
*
* Arguments   : none
*
* Returns     ; The 16 bit count (in a 32 bit variable) of the timer assuming the timer is an UP counter.
*********************************************************************************************************
*/

#if (uC_PROBE_OS_PLUGIN > 0) && (OS_PROBE_HOOKS_EN == 1)
CPU_INT32U  OSProbe_TmrRd (void)
{
#if OS_PROBE_TIMER_SEL == 3
    return ((CPU_INT32U)TMR3);                                          /* Return the value of timer 3 if selected                  */
#endif

#if OS_PROBE_TIMER_SEL == 5
    return ((CPU_INT32U)TMR5);                                          /* Return the value of timer 5 if selected                  */
#endif
}
#endif

/*
*********************************************************************************************************
*                                       TICKER INITIALIZATION
*
* Description : This function is called to initialize uC/OS-II's tick source (typically a timer generating
*               interrupts every 1 to 100 mS).
*
* Arguments   : none
*
* Note(s)     : 1) The timer operates at a frequency of Fosc / 4
*               2) The timer resets to 0 after period register match interrupt is generated
*********************************************************************************************************
*/

static  void  Tmr_TickInit (void)
{
    INT32U  tmr_frq;
    INT32U  cnts;


    tmr_frq   =   BSP_CPU_ClkFrq();                                     /* Get the CPU Clock Frequency (Hz) (Fcy)                   */
    cnts      =   (tmr_frq / OS_TICKS_PER_SEC) - 1;                     /* Calaculate the number of timer ticks between interrupts  */

#if BSP_OS_TMR_SEL == 2
    T2CON     =   0;                                                    /* Use Internal Osc (Fcy), 16 bit mode, prescaler = 1  		*/
    T2CONbits.T32 = 1;
    TMR2      =   0;                                                    /* Start counting from 0 and clear the prescaler count      */
    TMR3      =   0;
    PR3       =   (cnts>>16); 
    PR2       =   cnts&0xFFFFF;                                         /* Set the period register                                  */
    IPC2bits.T3IP     =  0;                                            /* Clear all timer 2 interrupt priority bits                */
    IPC2bits.T3IP     =  TIMER_INT_PRIO;                                /* Set timer 2 to operate with an interrupt priority of 4   */
    IFS0bits.T3IF     =  0;                                                 /* Clear the interrupt for timer 2                          */
    IEC0bits.T3IE     = 1  ;                                                 /* Enable interrupts for timer 2                            */
    T2CON    |=   TON;                                                  /* Start the timer                                          */
#endif

#if BSP_OS_TMR_SEL == 4
    T4CON     =   0;                                                    /* Use Internal Osc (Fcy), 16 bit mode, prescaler = 1  		*/
    TMR4      =   0;                                                    /* Start counting from 0 and clear the prescaler count      */
    PR4       =   cnts;                                                 /* Set the period register                                  */
    IPC6     &=  ~T4IP_MASK;                                            /* Clear all timer 4 interrupt priority bits                */
    IPC6     |=  (TIMER_INT_PRIO << 12);                                /* Set timer 4 to operate with an interrupt priority of 4   */
    IFS1     &=  ~T4IF;                                                 /* Clear the interrupt for timer 4                          */
    IEC1     |=   T4IE;                                                 /* Enable interrupts for timer 4                            */
    T4CON    |=   TON;                                                  /* Start the timer                                          */
#endif
}

/*
*********************************************************************************************************
*                                     OS TICK INTERRUPT SERVICE ROUTINE
*
* Description : This function handles the timer interrupt that is used to generate TICKs for uC/OS-II.
*********************************************************************************************************
*/

void OS_Tick_ISR_Handler (void)
{
//	static unsigned int AppTastRouter = 0;
	
	#if  BSP_OS_TMR_SEL == 2
	//IFS0 &= ~T2IF;
	IFS0bits.T3IF     =  0; 
	#endif
	
	#if  BSP_OS_TMR_SEL == 4
	IFS1 &= ~T41F;
	#endif
	
	SendMsgToRouter();	
	OSTimeTick(); 
}

