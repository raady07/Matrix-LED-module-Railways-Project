/****************************************************************************
* $RCSfile: LibraryFunctions.c

*
* Description:  This file contains some of the standard library functions .
*
***************************************************************************/
#include <includes.h>
#include "LibraryFunctions.h"

#define MAX_STR_LEN   256

/****************************************************************************
* TITLE:        StringLength
*
* DESCRIPTION:  This routine is to find the length of a null terminated
*               string. If the string is not null terminated, the string
*               size is fixed to 256.
*               It replaces standard library function strlen().
*
* INPUTS:       string - String whose length is to be found
*
* OUTPUTS:      length of the string
*
* LIMITATIONS:  None
*
****************************************************************************/
CPU_INT16S StringLength(const CPU_INT08S *string)
{
    CPU_INT16S len = 0;

    // until null string or 256 bytes is reached.
    while (string[len] != '\0' && len < MAX_STR_LEN)  
    {
       len++;
    }
    return len;
}

/****************************************************************************
* TITLE:        Power
*
* DESCRIPTION:  This routine is to perform power() operation on long 
*               CPU_INT16Seger values.
*               It replaces standard library function pow().
*
* INPUTS:       Intvalue - value on which power operation is to be done
*               Powval - value to which the CPU_INT16Seger should be raised 
*                           (ie ) Intvalue^Powval
*
* OUTPUTS:      resulting power raised value
*
* LIMITATIONS:  None
*
****************************************************************************/
CPU_INT32U Power(CPU_INT32U Intvalue, CPU_INT16S Powval)
{
    CPU_INT32U Result = 1;
    CPU_INT16S i;
    for (i=0; i<Powval; i++)
    {
        Result = Result * Intvalue ;
    }
    return Result;
}   


#ifdef BOOTMON_EN
/****************************************************************************
* TITLE:        ItoA
*
* DESCRIPTION:  This routine is to convert CPU_INT16Seger to alphanumeric value. 
*               It replaces standard library function itoa().
*
* INPUTS:       Intvalue - CPU_INT16Seger which is to be converted
*               Alpvalue - resulting alphanumeric value
*               Base - whether decimal or hexa 
*
* OUTPUTS:      None
*
* LIMITATIONS:  None
*
****************************************************************************/
void ItoA(CPU_INT32U Intvalue,CPU_INT08S *Alpvalue,CPU_INT16S Base)
{
    CPU_INT32U OneDigit;
    CPU_INT08S temp[10];
    CPU_INT16S i = 0;
    CPU_INT16S j,k;
    
    if (Intvalue != 0)
    {  
        while (Intvalue != 0)
        {
            OneDigit = Intvalue % Base;
            if (OneDigit > 9)
            {            // for hexa values greater than 9 , where 96 = '`'
                temp[i] = (CPU_INT08S)OneDigit -9 + 96;    
            }                                  
            else
            {
                temp[i] = (CPU_INT08S)OneDigit+48;  // where 48 = '0'
            }
            Intvalue = Intvalue / Base;
            i++;
        }
        k = 0;
    
        for(j = i-1; j>=0; j--) // elements in temp are in reverse order
        {
            Alpvalue[k] = temp[j];
            k++;
        }
        Alpvalue[k] = '\0';
    }
    else
    {
        Alpvalue[i] = (CPU_INT08S)48 ;         // if value is zero
        i++;
        Alpvalue[i] = '\0';
    }

}
/****************************************************************************
* TITLE:        AtoI
*
* DESCRIPTION:  This routine is to convert alphanumeric  to CPU_INT16Seger value.
*               It replaces standard library function atoi().
*
* INPUTS:       Alpvalue - alphanumeric value to be converted
*
* OUTPUTS:      converted CPU_INT16Seger value
*
* LIMITATIONS:  None
*
****************************************************************************/
CPU_INT32S AtoI(CPU_INT08S *value)
{
    CPU_INT16S len = 0;
    CPU_INT32S Result = 0;
    CPU_INT16S i = 1, j = 1;    
    len = StringLength(value);  // find the length
    while(j < len)
	{
		j++;
		i = i * 10;
	}
	while(len--)
	{
		Result = Result + (i * (*value++ - 48) );
		i = i/10;
	}	
    return Result;
}
   
 /**************************************************************************
 
 unsigned long int ConvertSTRING_LongInt(unsigned char *ptr)
{
	unsigned long int c = 0;
	unsigned int i = 1;
	unsigned char len = 0,j=1;
	len = stringlength(ptr);
	
	while(j < len)
	{
		j++;
		i = i*10;
	}
	while(len--)
	{
		c = c + i*(*ptr++ - 48);
		i = i/10;
	}
	return c;
}
 **************************************************************************/  
    
/****************************************************************************
* TITLE:        Sprintf
*
* DESCRIPTION:  This routine formats the variable of type decimal and 
*               hexadecimal to string type. It replaces standard 
*               library function sprCPU_INT16Sf().
*
* INPUTS:       Buffer - poCPU_INT16Ser to resulting string type variable
*               String - string with which formatting has to be done
*               Value  - decimal or hexadecimal value to be converted
*
* OUTPUTS:      None
*
* LIMITATIONS:  None
*
****************************************************************************/
/*
void Sprintf(CPU_INT08S *Buffer,CPU_INT08S *String,CPU_INT32U Value)
{
    CPU_INT16S i, j, k;
    CPU_INT16S Format, ItoAlength;
    CPU_INT16S Fillvalue = 48;                     // Zero
    
    CPU_INT08S ItoaStr[100] = {0};
    CPU_INT08S Formatarr[10] = {0};  
    
    i = 0;  j = 0; k =0;                // Initialise to zero
   
    while (String[i] != '%')            // loop until '%'
    {
        *Buffer = String[i];
        *Buffer++;
        i++;
    }
    i++;
   
    if (String[i] == 46)      // if format needs zero fill (ex) %.4d or %04d
    {                      
        i++;
    }
        
    while (String[i] < 58)        // if format needs  allignment (ex) %08d
    {
        Formatarr[k] = String[i];
        k++;
        i++;
    }
       
    if(String[i] == 'd')                        //  for decimal types       
    {
        ItoA(Value,ItoaStr,INT_BASE);
    }
    else if (String[i] == 'x')
    {
        ItoA(Value,ItoaStr,HEX_BASE);           // for hexa decimal types   
    }
    else
    {
        StringCopy(ItoaStr,(const CPU_INT08S*)Value);
        Fillvalue = 32;                         // Blank fill
    }
    i++;
    
    Format = AtoI(Formatarr);
    ItoAlength = StringLength(ItoaStr);
   
    while((Format - ItoAlength) > 0)        
    {
        *Buffer = Fillvalue;                     // fill with value above choosen
        *Buffer++;
        Format--;
    }                               
                // if format length is less than string 
                
    if ((Format != 0) && (ItoAlength > Format)) 
    {
        ItoAlength = Format;
    }
    
    while (j < ItoAlength)          // Copy the ItoA converted string
    {
        *Buffer = ItoaStr[j];
        *Buffer++;
        j++;
    }

    while (String[i] != '\0')      // copy the rest in the formatting string
    {
        *Buffer = String[i];
        *Buffer++;
        i++;
    }
       
        *Buffer = '\0';               // append null character at end.
        *Buffer++;
        
}
*/
/****************************************************************************
* TITLE:        StringCmp
*
* DESCRIPTION:  This routine is to compare two strings.It replaces standard
*               library function strcmp().
*
* INPUTS:       String1,String2 - strings which are to be compared
*
* OUTPUTS:      True if strings are equal and FALSE if not
*
* LIMITATIONS:  None
*
****************************************************************************/
CPU_INT16S StringCmp(CPU_INT08S *String1, CPU_INT08S *String2)
{
    CPU_INT16S i,Strlen1,Strlen2;
    
    Strlen1 = StringLength(String1);
    Strlen2 = StringLength(String2);
    if (Strlen1 == Strlen2)
    {
        i = 0;
        while (i < Strlen1)
        {
            if (String1[i] != String2[i])
            return 0;
        
            i++;
        }
        return 1;
    }
    return 0;
}
#endif

/****************************************************************************
* TITLE:        StringCat
*
* DESCRIPTION:  This routine is to concatenate two strings. It replaces 
*               standard library function strcat().
*
* INPUTS:       String1 - Destination string 
*               String2 - source string to be concatenated
* OUTPUTS:      None
*
* LIMITATIONS:  None
*
****************************************************************************/
void StringCat(CPU_INT08S *String1,const CPU_INT08S *String2)
{
    CPU_INT16S len = 0;
    CPU_INT16S strlen = 0;
    CPU_INT16S i;

    len = StringLength(String2);        // length of string2
    
    // go until end of the string
    while ( *String1 != '\0' && strlen < MAX_STR_LEN) 
    {
        *String1++;
         strlen++;
    }

    i = 0;

    while (len > 0)
    {
        *String1 = String2[i];      // add string2 at end of string1
        *String1++;
        i++;
        len--;
    }

    *String1 = '\0';
    *String1++;
}

/****************************************************************************
* TITLE:        StringCopy
*
* DESCRIPTION:  This routine is to copy a string to another string. It 
*               replaces standard library function strcpy().
*
* INPUTS:       String1 - Destination string 
*               String2 - source string to be copied
*
* OUTPUTS:      None
*
* LIMITATIONS:  None
*
****************************************************************************/
void StringCopy(CPU_INT08S *String1,const CPU_INT08S *String2)
{
    CPU_INT16S len = 0 ;
    CPU_INT16S i;
    len = StringLength(String2);
    
    i = 0;
    while (len > 0)        // while length of string2 is greater than 0
    {
        *String1 = String2[i];
        *String1++;
        i++;
        len--;
    }
    *String1 = '\0';
    *String1++;
    
}

/****************************************************************************
* TITLE:        Memcopy
*
* DESCRIPTION:  This routine is to copy data from one memory location to 
*               another. It replaces standard library function memcpy().
*
* INPUTS:       m1 - destination address where m2 content should be copied
*               m2 - source address from where content to be copied
*               n  - number of bytes to be copied
*
* OUTPUTS:      Returns start of the destination address.
*
* LIMITATIONS:  None
*
****************************************************************************/
void *Memcopy(void *m1, const void *m2, CPU_INT16U n)
{
    CPU_INT08S *memloc1 = m1;
    const CPU_INT08S *memloc2 = m2;
    
    while (n > 0)
    {
        *memloc1 = *memloc2;
        *memloc1++;
        *memloc2++;
        n--;
    }
    return m1;
}

INT8U    BCD2Bin(INT8U data)
{
	return ((((data & 0xF0 )>>4) * 10) + (data&0xF));	  
} 

INT8U	D2BCD(INT8U data)
{
	return (((data/10)<< 4) | (data%10));
}

/****************************************************************************
* TITLE:        ClearBuffer
*
* DESCRIPTION:  This routine is to Clear the Buffer. 
*
* INPUTS:       *ptr 	- Buffer to be Cleared
*               length 	- Lenth of the Buffer
* OUTPUTS:      None
*
* LIMITATIONS:  None
*
****************************************************************************/
void ClearBuffer(unsigned char *ptr, int length)
{
	length--;
	while(length>=0)
	{
		ptr[length] = '\0';
		length--;
	}	
}
/****************************************************************************
*                        E N D   OF   F I L E
****************************************************************************/

