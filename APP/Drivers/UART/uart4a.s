;
;********************************************************************************************************
;                                               uC/OS-II
;                                         The Real-Time Kernel
;
;                         (c) Copyright 2002, Jean J. Labrosse, Weston, FL
;                                          All Rights Reserved
;
;
;                                   PIC24 MPLab and C30 uC/Probe Port
;
;
;********************************************************************************************************
;

;
;********************************************************************************************************
;                                                CONSTANTS
;********************************************************************************************************
;
    .equ     __24FJ256GA110, 1                                          ; Inform the p24FJ256GA110 header file that we are using a p24FJ256GA110

;
;********************************************************************************************************
;                                                INCLUDES
;********************************************************************************************************
;

    .include "p24FJ256GA110.inc"                                        ; Include assembly equates for various CPU registers and bit masks
    .include "os_cpu_util_a.s"                                          ; Include an assembly utility files with macros for saving and restoring the CPU registers

;
;********************************************************************************************************
;                                             LINKER SPECIFICS
;********************************************************************************************************
;

    .text                                                               ; Locate this file in the text region of the build

;
;********************************************************************************************************
;                                          GLOBALS AND EXTERNALS
;********************************************************************************************************
;

    .global __U4RXInterrupt
    .global __U4TXInterrupt

;
;********************************************************************************************************
;                                            U4RXInterrupt
;
; Description : This function is the UART4 Rx Interrupt Service Routine
;
; Notes       : All user interrupts should be defined as shown below.
;********************************************************************************************************
;

__U4RXInterrupt:
    OS_REGS_SAVE                                                        ; 1) Save processor registers

    mov   #_OSIntNesting, w1
    inc.b [w1], [w1]                                                    ; 2) Call OSIntEnter() or increment OSIntNesting

    dec.b _OSIntNesting, wreg                                           ; 3) Check OSIntNesting. if OSIntNesting == 1, then save the stack pointer, otherwise jump to T2_Cont
    bra nz, U4RXCont
    mov _OSTCBCur, w0
    mov w15, [w0]

U4RXCont:
    call _UART4_RxISRHandler                                       ; 4) Call YOUR ISR Handler (May be a C function). In this case, the OSView Rx ISR Handler
    call _OSIntExit                                                     ; 5) Call OSIntExit() or decrement 1 from OSIntNesting

    OS_REGS_RESTORE                                                     ; 6) Restore registers

    retfie                                                              ; 7) Return from interrupt

;
;********************************************************************************************************
;                                            U4TXInterrupt
;
; Description : This function is the UART4 Tx Interrupt Service Routine
;
; Notes       : All user interrupts should be defined as shown below.
;********************************************************************************************************
;

__U4TXInterrupt:
    OS_REGS_SAVE                                                        ; 1) Save processor registers

    mov   #_OSIntNesting, w1
    inc.b [w1], [w1]                                                    ; 2) Call OSIntEnter() or increment OSIntNesting

    dec.b _OSIntNesting, wreg                                           ; 3) Check OSIntNesting. if OSIntNesting == 1, then save the stack pointer, otherwise jump to T2_Cont
    bra nz, U4TXCont
    mov _OSTCBCur, w0
    mov w15, [w0]

U4TXCont:
    call _UART4_TxISRHandler                                       ; 4) Call YOUR ISR Handler (May be a C function). In this case, the OSView Tx ISR Handler
    call _OSIntExit                                                     ; 5) Call OSIntExit() or decrement 1 from OSIntNesting

    OS_REGS_RESTORE                                                     ; 6) Restore registers

    retfie                                                              ; 7) Return from interrupt



