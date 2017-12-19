
#include <includes.h>

void OpenI2C1(unsigned int config1,unsigned int config2)
{
    I2C1BRG = config2;
    I2C1CON = config1;
}

void StartI2C1(void)
{
     I2C1CONbits.SEN = 1;   /* initiate Start on SDA and SCL pins */
}

void RestartI2C1(void)
{ 
    I2C1CONbits.RSEN = 1;   /* initiate restart on SDA and SCL pins */
}

void StopI2C1(void)
{
     I2C1CONbits.PEN = 1;   /* initiate Stop on SDA and SCL pins */
}


void IdleI2C1(void)
{
    /* Wait until I2C Bus is Inactive */
    while(I2C1CONbits.SEN || I2C1CONbits.RSEN || I2C1CONbits.PEN || I2C1CONbits.RCEN ||
          I2C1CONbits.ACKEN || I2C1STATbits.TRSTAT);  
}

unsigned char MasterReadI2C1(void)
{
    I2C1CONbits.RCEN = 1;
    while(I2C1CONbits.RCEN);
    I2C1STATbits.I2COV = 0;
    return(I2C1RCV);
}

char MasterWriteI2C1(unsigned char data_out)
{
    I2C1TRN = data_out;

    if(I2C1STATbits.IWCOL)        /* If write collision occurs,return -1 */
        return -1;
    else
    {
        return 0;
    }
}

void NotAckI2C1(void)
{
    I2C1CONbits.ACKDT = 1;
    I2C1CONbits.ACKEN = 1;
}

void AckI2C1(void)
{
    I2C1CONbits.ACKDT = 0;
    I2C1CONbits.ACKEN = 1;
}


void WaitI2C1(unsigned int cnt)
{
	while(--cnt)
	{
		asm( "nop" );
		asm( "nop" );
	}
}
