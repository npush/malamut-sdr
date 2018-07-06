#ifndef __I2C_H
#define __I2C_H


#define I2C_Speed              350000
#define EEPROM_ADDRESS         0xA0

void i2c_ee_init(void);
int I2C_EE_BufferWrite(unsigned short WriteAddr,  unsigned char * pBuffer, int leght);
int I2C_EE_BufferRead(unsigned short ReadAddr, unsigned char* pBuffer,  u16 NumByteToRead);


#endif