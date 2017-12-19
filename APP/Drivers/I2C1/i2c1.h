
#ifndef  I2C1_H
#define  I2C1_H

void OpenI2C1(unsigned int config1,unsigned int config2);
void StartI2C1(void);
void IdleI2C1(void);
unsigned char MasterReadI2C1(void);
char MasterWriteI2C1(unsigned char data_out);
void NotAckI2C1(void);
void RestartI2C1(void);
void StopI2C1(void);
void AckI2C1(void);
void WaitI2C1(unsigned int cnt);
VOID Enable_Read_I2C1();

#define I2C_BRG	  157//((32000000/2/100000)-1)
#define I2C_ON                      0b1000000100111111   //0xFFFF /*I2C module enabled */
#define I2C_OFF                     0x7FFF /*I2C module disabled */

#endif //I2C1_H
