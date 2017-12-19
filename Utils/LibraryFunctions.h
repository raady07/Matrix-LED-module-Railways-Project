/****************************************************************************
* $RCSfile: LibraryFunctions.h

*
* Description:  This file contains some of the standard library function
*               declaration.
****************************************************************************/
#ifndef LIBRARYFUNCTIONS_H
#define LIBRARYFUNCTIONS_H

#define BOOTMON_EN
#define INT_BASE        10          // for base 10
#define HEX_BASE        16          // for base 16

#ifndef NULL
#define NULL             0
#endif

/****          Function Prototype          ********/

#ifdef BOOTMON_EN
//void Sprintf(CPU_INT08S *Buffer,CPU_INT08S *String,CPU_INT32U Value);
CPU_INT16S StringCmp(CPU_INT08S *String1, CPU_INT08S *String2);
void ItoA(CPU_INT32U Intvalue,CPU_INT08S *Alpvalue,CPU_INT16S base);
CPU_INT32S AtoI(CPU_INT08S *Alpvalue);

#endif

CPU_INT16S StringLength(const CPU_INT08S *string);
CPU_INT32U Power(CPU_INT32U Intvalue,CPU_INT16S Powval);
void StringCat(CPU_INT08S *String1,const CPU_INT08S *String2);
void StringCopy(CPU_INT08S *String1,const CPU_INT08S *String2);
void *Memcopy(void *m1, const void *m2, CPU_INT16U n);
void ClearBuffer(unsigned char *ptr, int length);
INT8U    BCD2Bin(INT8U data);

#endif  // LIBRARYFUNCTIONS_H

