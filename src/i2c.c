#include "main.h"


//------------------------------------------------------------------------------
void i2c_ee_init(void){

  GPIO_InitTypeDef GPIO_InitStructure;
  
  #ifdef  pcbASDR_V1
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);
  //CODEC_I2C SCL and SDA pins configuration
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;//GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
  #else
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);
  //CODEC_I2C SCL and SDA pins configuration
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;//GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);
  #endif
  
  
  I2C_InitTypeDef I2C_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

  /* CODEC_I2C peripheral configuration */
  I2C_DeInit(I2C1);
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0x33;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
  /* Enable the I2C peripheral */
  I2C_Cmd(I2C1, ENABLE);
  I2C_Init(I2C1, &I2C_InitStructure);
  
}
//------------------------------------------------------------------------------
/*******************************************************************************
* Function Name  : I2C_EE_ByteWrite
* Description    : Writes one byte to the I2C EEPROM.
* Input          :
*                  - WriteAddr : EEPROM's internal address to write to.
                   - pBuffer : pointer to the buffer  containing the data to be
*                    written to the EEPROM.
* Output         : None
* Return         : None
*******************************************************************************/
int I2C_EE_BufferWrite(unsigned short WriteAddr,  unsigned char * pBuffer, int leght)
{
  unsigned long time;
#define t_out 100

  I2C_GenerateSTART(I2C1, ENABLE);
  time = RTC_Get();
  while((I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)==0) && ( Interval_RTC_Get(time)< t_out));
  if( Interval_RTC_Get(time)>= t_out)
    return 1;



  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);
  time = RTC_Get();
  while( (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==0) && ( Interval_RTC_Get(time)< t_out) );
  if( Interval_RTC_Get(time)>= t_out)
    return 2;



  I2C_SendData(I2C1, *(((unsigned char*)&WriteAddr)+1) );//MSB
  time = RTC_Get();
  while( (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)==0) && ( Interval_RTC_Get(time)< t_out));
  if( Interval_RTC_Get(time)>= t_out)
    return 3;
  I2C_SendData(I2C1, *(((unsigned char*)&WriteAddr)+0) );//LSB
  time = RTC_Get();
  while( (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)==0) && ( Interval_RTC_Get(time)< t_out));
  if( Interval_RTC_Get(time)>= t_out)
    return 4;



  for( int i=0; i< leght; ++i ){

    I2C_SendData(I2C1, *pBuffer++);
    time = RTC_Get();
    while( (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)==0) && ( Interval_RTC_Get(time)< t_out));
    if( Interval_RTC_Get(time)>= t_out)
      return 5+i;
  }

  I2C_GenerateSTOP(I2C1, ENABLE);
  
  //delay_ms( 1 );
  time = RTC_Get();
  while(I2C_CheckEvent(I2C1, I2C_FLAG_BUSY)&&(Interval_RTC_Get(time)< t_out));
  if( Interval_RTC_Get(time)>= t_out)
    return 1;

  return 0;
}
//------------------------------------------------------------------------------

/*******************************************************************************
* Function Name  : I2C_EE_BufferRead
* Description    : Reads a block of data from the EEPROM.
* Input          : - pBuffer : pointer to the buffer that receives the data read
*                    from the EEPROM.
*                  - ReadAddr : EEPROM's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the EEPROM.
* Output         : None
* Return         : None
*******************************************************************************/
int I2C_EE_BufferRead(unsigned short ReadAddr, unsigned char* pBuffer,  u16 NumByteToRead)
{


  unsigned long time;
  unsigned long rtime = t_out*NumByteToRead;


  I2C_GenerateSTART(I2C1, ENABLE);
  time = RTC_Get();
  while((I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)== 0)&&(Interval_RTC_Get(time)< t_out));
  if( Interval_RTC_Get(time)>= t_out)
    return 1;

  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);
  time = RTC_Get();
  while((I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)== 0)&&(Interval_RTC_Get(time)< t_out));
  if( Interval_RTC_Get(time)>= t_out)
    return 2;

  I2C_Cmd(I2C1, ENABLE);
  I2C_SendData(I2C1, *(((unsigned char*)&ReadAddr)+1));
  time = RTC_Get();
  while((I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)== 0)&&(Interval_RTC_Get(time)< t_out));
  if( Interval_RTC_Get(time)>= t_out)
    return 3;
  I2C_SendData(I2C1, *(((unsigned char*)&ReadAddr)+0));
  time = RTC_Get();
  while((I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)== 0)&&(Interval_RTC_Get(time)< t_out));
  if( Interval_RTC_Get(time)>= t_out)
    return 4;




  I2C_GenerateSTART(I2C1, ENABLE);
  time = RTC_Get();
  while((I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)== 0)&&(Interval_RTC_Get(time)< t_out));
  if( Interval_RTC_Get(time)>= t_out)
    return 5;

  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Receiver);
  time = RTC_Get();
  while((I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)== 0)&&(Interval_RTC_Get(time)< t_out));
  if( Interval_RTC_Get(time)>= t_out)
    return 6;


  time = RTC_Get();
  //while( (NumByteToRead > 0)&&(get_time_interval(time)>= t_out))
  while( NumByteToRead )
  {
    if(NumByteToRead == 1)
    {
      while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
      I2C_AcknowledgeConfig(I2C1, DISABLE);
      I2C_GenerateSTOP(I2C1, ENABLE);
    }

    if( I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
    {
      *pBuffer++ = I2C_ReceiveData(I2C1);
      NumByteToRead--;
    }

  if( Interval_RTC_Get(time)>= rtime )
    return 7+NumByteToRead;
  }
 
  if( Interval_RTC_Get(time)>= rtime )
    return 7+NumByteToRead;




 /* I2C_GenerateSTART(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  I2C_Cmd(I2C1, ENABLE);
  I2C_SendData(I2C1, *(((unsigned char*)&ReadAddr)+1));
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  I2C_SendData(I2C1, *(((unsigned char*)&ReadAddr)+0));
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));


  I2C_GenerateSTART(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
 
  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Receiver);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));


  while(NumByteToRead)
  {
    if(NumByteToRead == 1)
    {
      while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
      I2C_AcknowledgeConfig(I2C1, DISABLE);
      I2C_GenerateSTOP(I2C1, ENABLE);
    }

    if( I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
    {
      *pBuffer++ = I2C_ReceiveData(I2C1);
      NumByteToRead--;
    }
  }


*/

  I2C_AcknowledgeConfig(I2C1, ENABLE);
  
  //delay_ms( 1 );
  time = RTC_Get();
  while(I2C_CheckEvent(I2C1, I2C_FLAG_BUSY)&&(Interval_RTC_Get(time)< t_out));
  if( Interval_RTC_Get(time)>= t_out)
    return 1;

  return 0;
}
