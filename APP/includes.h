/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                                 (c) Copyright 2006, Micrium, Weston, FL
*                                           All Rights Reserved
*
*                                           MASTER INCLUDE FILE
*********************************************************************************************************
*/

#ifndef     INCLUDES_H
#define     INCLUDES_H

#define 	SYSCLK 32000000

#include    "p24FJ256GA110.h"
#include    "PPS.h"

#include    <cpu.h>
#include    <ucos_ii.h>

#include    <controller.h>
#include    <load_cont.h>
#include	<app.h>
#include    <lcd.h>
#include    <KeyPad.h>
#include    <lcd_router.h>

#include    <app_cfg.h>
#include    <task_inc.h>

#include    <uart.h>
#include    <debug_router.h>
#include    <spi1.h>
#include    <i2c1.h>
#include    <eeprom_router.h>

#include    <rtc.h>
#include    <modem_router.h>
#include	<exem_router.h>
#include    <Dimmer_Data.h>
#include 	"LibraryFunctions.h"
#endif                                                                  /* End of File                                              */

