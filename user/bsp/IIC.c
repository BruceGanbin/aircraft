#include "IIC.h"
#include <includes.h>


volatile u32 TimeOut=EE_TimeOut;
u8 EE_State=0x00;



/**
***************************************************
 *@Function     :IIC_WritePage(u8 *pBuffer,u8 PartAddr,u8 WriteAddr,u8 NumByteToWrite)
 *@Return_Value :Null
 *@Peremater    :pBuffer:
                 PartAddr:
                 WriteAddr:
                 NumByteToWrite:
 *@Brief        :
***************************************************/
u8* IIC_WritePage(u8 *pBuffer,u8 PartAddr,u8 WriteAddr,u8 NumByteToWrite)
{
  /*send Start condition,test on EV5 and clear it*/
  TimeOut=EE_TimeOut;
  I2C_GenerateSTART(I2C1,ENABLE);
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
    {
      if((TimeOut--)==0){EE_State=EE_ErrorWTimeOut;}    
    }

  /*send Part Address for write,test on EV6 and clear it*/
  TimeOut=EE_TimeOut;
  I2C_Send7bitAddress(I2C1,PartAddr,I2C_Direction_Transmitter);
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
      if((TimeOut--)==0){EE_State=EE_ErrorWTimeOut;}
    }

  /******writeAddress for 8bit or 16 bit**********/
  TimeOut=EE_TimeOut;

 /**send the EEPROM's internal address,test on EV8 and clear it**/
  I2C_SendData(I2C1,(u8)WriteAddr);  
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
      if((TimeOut--)==0){EE_State=EE_ErrorWTimeOut;}
    }

  /* DMA control */
  /*send the bufferData and buffer size*/
  while(NumByteToWrite--)
    {
      I2C_SendData(I2C1,*pBuffer);
      TimeOut=EE_TimeOut;
      while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
      {
    	if((TimeOut--)==0){EE_State=EE_ErrorRTimeOut;}
      }
      pBuffer++;
    }
  
  /*send STOP condition*/
  I2C_GenerateSTOP(I2C1,ENABLE);
   delayms(5);
  return pBuffer;
}


/**
*************************************************
 *@Function     : IIC_ReadPage(u8 *pBuffer,u8 PartAddr,u8 WriteAddr,u8 NumByteToWrite)
 *@Return_Value :Null
 *@Peremater    :
 *@Brief        :
***************************************************/
u8* IIC_ReadPage(u8 *pBuffer,u8 PartAddr,u8 WriteAddr,u8 NumByteToWrite)
{
//  I2C_ClearFlag(I2C1, I2C_FLAG_AF);
  I2C_AcknowledgeConfig(I2C1,ENABLE);
  /*send Start condition,test on EV5 and clear it*/
  TimeOut=EE_TimeOut;
  I2C_GenerateSTART(I2C1,ENABLE);
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
    {
      if((TimeOut--)==0){EE_State=EE_ErrorRTimeOut;}    
    }

  /*send Part Address for write,test on EV6 and clear it*/
  TimeOut=EE_TimeOut;
  I2C_Send7bitAddress(I2C1,PartAddr,I2C_Direction_Transmitter);
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
      if((TimeOut--)==0){EE_State=EE_ErrorRTimeOut;}
    }

  /*send the EEPROM's internal address,test on EV8 and clear it
  I2C_SendData(I2C1,WriteAddr);
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));  */
   /******writeAddress for 8bit or 16 bit**********/
  TimeOut=EE_TimeOut;

 /**send the EEPROM's internal address,test on EV8 and clear it**/
  I2C_SendData(I2C1,(u8)WriteAddr);
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
      if((TimeOut--)==0){EE_State=EE_ErrorRTimeOut;}
    }


  /* DMA control */
  /*send Start condition,test on EV5 and clear it*/
  TimeOut=EE_TimeOut;
  I2C_GenerateSTART(I2C1,ENABLE);
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
    {
      if((TimeOut--)==0){EE_State=EE_ErrorRTimeOut;}    
    }

  /*send Part Address for write,test on EV6 and clear it*/
  TimeOut=EE_TimeOut;
  I2C_Send7bitAddress(I2C1,PartAddr,I2C_Direction_Receiver);
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
      if((TimeOut--)==0){EE_State=EE_ErrorRTimeOut;}
    }

  while(NumByteToWrite)
    {
      if(NumByteToWrite==1)
    	{
    	  I2C_AcknowledgeConfig(I2C1,DISABLE);
    	  I2C_GenerateSTOP(I2C1,ENABLE);
    	}
      if(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED))
    	{
    	  *pBuffer=I2C_ReceiveData(I2C1);
    	  pBuffer++;
    	  NumByteToWrite--;
	     TimeOut=EE_TimeOut;
    	}
      else
    	{
    	    if((TimeOut--)==0){EE_State=EE_ErrorRTimeOut;}  
    	}
    
     }
 I2C_GenerateSTOP(I2C1,ENABLE);
 return pBuffer;
    }

