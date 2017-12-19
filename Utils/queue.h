/****************************************************************************
* $RCSfile: queue.h $

*
* Description:  This module contains the queue definitions and prototypes 
*               for Queue.c.
*
* NOTE:  The Size of the Queue must equal 2**x, where x = 2 to 31.  
****************************************************************************/

#ifndef QUEUE_H
#define QUEUE_H

#include "includes.h"

// The Size of the Queue must equal 2**x, where x = 2 to 31.  This module
// stops the queue size at 4096, but it will go as high as system
// memory will allow.
typedef enum {
    Q_SIZE_04 = 4,
    Q_SIZE_08 = 8,
    Q_SIZE_16 = 16,
    Q_SIZE_32 = 32,
    Q_SIZE_64 = 64,
    Q_SIZE_128 = 128,
    Q_SIZE_256 = 256,
    Q_SIZE_512 = 512,
    Q_SIZE_1024 = 1024,
    Q_SIZE_2048 = 2048,
    Q_SIZE_4096 = 4096
} QUEUE_SIZE;


// This is the queue structure used by Queue.c.  This queue structure is
// expandable to poCPU_INT16S to any data type.  To add a new data type, just add the
// type poCPU_INT16Ser to the Union structure definition below and then add the
// ReadQ() and WriteQ() member functions to Queue.c.  Currently this queue can
// only be used for 8 and 32 bit data types.  Another limitation of this
// queue, is the size of the QueBuffer must be of type QUEUE_SIZE (see above).
typedef struct {
    CPU_INT16S Put;        // Next On Queue Index
    CPU_INT16S Get;        // Next Off Queue Index
    CPU_INT08U * Ptr8;   // poCPU_INT16Ss to 8 bit buffer
    CPU_INT16S Mask;       // Mask is the size of the Queue-1
} QUEUE;


// prototypes for BYTE queue
CPU_BOOLEAN ReadQ8(QUEUE *Qptr, CPU_INT08U *DataPtr);
CPU_BOOLEAN WriteQ8(QUEUE *Qptr, CPU_INT08U Data);
CPU_BOOLEAN WriteBlkToQ8(QUEUE *Qptr, CPU_INT08U *pData, CPU_INT16S Size);
CPU_INT16S SpaceOnQ(QUEUE *Qptr);
void InitQ(QUEUE *Qptr);


#endif


