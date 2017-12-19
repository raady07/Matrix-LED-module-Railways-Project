/****************************************************************************
* $RCSfile: Queue.c $
*
* Description:  This module contains reentrant functions that maintain a 
*               queue defined by the QUEUE typedef in queue.h.
*
* The following routines are Global functions   
*     InitQ() - This routine sets the queue index's to 0.
*     ReadQ8() - This routine reads an element from the queue.
*     WriteQ8() - This routine writes an element to the queue.
*     WriteBlkToQ8() - This routine writes a blk of data to the queue.
*
* NOTE:  The Size of the Queue must equal 2**x, where x = 2 to 31.

****************************************************************************/
#include "queue.h"
#include "LibraryFunctions.h"

#ifdef UNIT_TEST
    void IncGetIndex(QUEUE *Qptr, CPU_INT16S Get);
    void IncPutIndex(QUEUE *Qptr, CPU_INT16S Put);
#else
    inline void IncGetIndex(QUEUE *Qptr, CPU_INT16S Get);
    inline void IncPutIndex(QUEUE *Qptr, CPU_INT16S Put);
#endif



///////////////////////////////////////////////////////////////////////////////
// The following 3 routines service a queue of Bytes
///////////////////////////////////////////////////////////////////////////////


/****************************************************************************
* TITLE:        ReadQ8
*
* DESCRIPTION:  This function gets an element from the queue and updates the 
*               get index.  
*
* INPUTS:       *Qptr - Pointer to a queue as defined in queue.h
*               DataPtr - Points to the location to store the data
*
* OUTPUTS:      CPU_BOOLEAN - TRUE if data was read from the queue
*                         FALSE if the queue was empty
*               *DataPtr - The data read from the queue is passed in the
*                          argument.
*
* LIMITATIONS:  
*
****************************************************************************/
CPU_BOOLEAN ReadQ8(QUEUE *Qptr, CPU_INT08U *DataPtr)
{
    CPU_INT16S Get = Qptr->Get;
    CPU_BOOLEAN DataAvailable = FALSE;

    if(Qptr->Put != Get)
    {
        // Get next word off the Queue
        *DataPtr = Qptr->Ptr8[Get];

        // Increment Get Index
        IncGetIndex(Qptr, Get);

        DataAvailable = TRUE;
    }
    return DataAvailable;
}


/****************************************************************************
* TITLE:        WriteQ8
*
* DESCRIPTION:  This is a reentrant function that writes a single byte to 
*               the queue
*
* INPUTS:       *Qptr - Pointer to a queue as defined in queue.h
*               Data - Data element to write to the queue
*
* OUTPUTS:      CPU_BOOLEAN - TRUE if data was placed on the queue
*                         FALSE if the queue is FULL
*
* LIMITATIONS:  None
*
****************************************************************************/
CPU_BOOLEAN WriteQ8(QUEUE *Qptr, CPU_INT08U Data)
{
    CPU_INT16S Put;
    CPU_BOOLEAN WordLoaded = FALSE;
    
    if( SpaceOnQ(Qptr) )
    {    
        // Get Next location on the Queue
        Put = Qptr->Put;
        // Copy Data to Queue
        Qptr->Ptr8[Put] = Data;

        // Increment Put Index
        IncPutIndex(Qptr, Put);

        WordLoaded = TRUE;
    }
    return WordLoaded;
}

/****************************************************************************
* TITLE:        WriteBlkToQ8
*
* DESCRIPTION:  This is a reentrant function that writes 1 or more bytes 
*               to the queue
*
* INPUTS:       *Qptr - Pointer to a queue as defined in queue.h
*               pData - Data element to write to the queue
*
* OUTPUTS:      CPU_BOOLEAN - TRUE if data was placed on the queue
*                         FALSE if the queue is FULL
*
* LIMITATIONS:  None
*
****************************************************************************/
CPU_BOOLEAN WriteBlkToQ8(QUEUE *Qptr, CPU_INT08U *pData, CPU_INT16S Size)
{
    CPU_INT16S Put;
    CPU_BOOLEAN DataLoaded = FALSE;
    CPU_INT16S SpaceToEndOfBuffer;
    CPU_INT16S SizeOfWrapAround;
    
    if( SpaceOnQ(Qptr) >= Size )
    {    
        // Get Next location on the Queue
        Put = Qptr->Put;
        // Calc number of elements to the end of the buffer.
        SpaceToEndOfBuffer = Qptr->Mask - Put + 1;

        // Check to see if buffer will wrap around.
        if( SpaceToEndOfBuffer >= Size )
        {
            // Buffer does not wrap so copy data to queue
            Memcopy(&Qptr->Ptr8[Put], pData, Size);
            // Increment Put Index
            Put += Size;
        }
        else
        {
            // Size of data wraps around the buffer.
            SizeOfWrapAround = Size - SpaceToEndOfBuffer;
            Memcopy(&Qptr->Ptr8[Put], pData, SpaceToEndOfBuffer);
            Memcopy(&Qptr->Ptr8[0], pData+SpaceToEndOfBuffer,
                    SizeOfWrapAround);
            Put = SizeOfWrapAround;
        }
        Put &= Qptr->Mask;
        Qptr->Put = Put;
        DataLoaded = TRUE;
    }
    return DataLoaded;
}


///////////////////////////////////////////////////////////////////////////////
//  The following routines service the Queues above
///////////////////////////////////////////////////////////////////////////////

/****************************************************************************
* TITLE:        SpaceOnQ
*
* DESCRIPTION:  This is a reentrant function that calculates the remaining
*               space on the queue.
*
* INPUTS:       *Qptr - Pointer to a queue as defined in queue.h
*
* OUTPUTS:      int - Number of available elements left on the queue.
*
* LIMITATIONS:  None
*
****************************************************************************/
CPU_INT16S SpaceOnQ(QUEUE *Qptr)
{
    CPU_INT16S RemainingSpace;
    CPU_INT16S Get = Qptr->Get;
    CPU_INT16S Put = Qptr->Put;
    
    if(Get > Put)
    {
        RemainingSpace = Get - Put - 1;
    }
    else
    {
        RemainingSpace = (Qptr->Mask - Put) + Get;
    }
    
    return RemainingSpace;
}


/****************************************************************************
* TITLE:        InitQ
*
* DESCRIPTION:  This is a reentrant function that sets the queue index's to 
*               zero.
*
* INPUTS:       *Qptr - Pointer to a queue as defined in queue.h
*
* OUTPUTS:      None
*
* LIMITATIONS:  
*
****************************************************************************/
void InitQ(QUEUE *Qptr)
{
    Qptr->Put = Qptr->Get = 0;
}


/****************************************************************************
* TITLE:        IncPutIndex
*
* DESCRIPTION:  This is a reentrant function increments the Put Index
*
* INPUTS:       *Qptr - Pointer to a queue as defined in queue.h
*               Put - Put index for the queue specified by Qptr.
*
* OUTPUTS:      
*
* LIMITATIONS:  None
*
****************************************************************************/
#ifdef UNIT_TEST
void IncPutIndex(QUEUE *Qptr, CPU_INT16S Put)
#else
inline void IncPutIndex(QUEUE *Qptr, CPU_INT16S Put)
#endif
{
    Put++;
    Put &= Qptr->Mask;
    Qptr->Put = Put;
}
/****************************************************************************
* TITLE:        IncGetIndex
*
* DESCRIPTION:  This is a reentrant function increments the Get Index
*
* INPUTS:       *Qptr - Pointer to a queue as defined in queue.h
*               Get - Get index for the queue specified by Qptr.
*
* OUTPUTS:      
*
* LIMITATIONS:  None
*
****************************************************************************/
#ifdef UNIT_TEST
void IncGetIndex(QUEUE *Qptr, CPU_INT16S Get)
#else
inline void IncGetIndex(QUEUE *Qptr, CPU_INT16S Get)
#endif
{
    Get++;
    Get &= Qptr->Mask;
    Qptr->Get = Get;
}

/****************************************************************************
*                                END OF FILE                                *
****************************************************************************/
