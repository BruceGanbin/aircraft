
#include <stm32f10x_bus.h>
#include <includes.h>

/**********************************
	  IIC
***********************************/
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





/**************************************************
        USART
 **************************************************/

char Usart1_RxBuffer[500];
char Usart1_TxBuffer[500];
char Usart2_RxBuffer[500];
char Usart2_TxBuffer[500];
u8   Usart1_RxCount=0;
u8   Usart2_RxCount=0;
u8   Usart_Sta=(USART1R_OVER|USART2R_OVER);
/****  ,Uart1_TimeOver      ***/

/**
*************************************************
 @Function     :Usart_Send()
 @Return_Value :Null
 @Peremater    :
 @Brief        :
***************************************************/
u8 Usart1_Send(char *pBuffer,u32 SendNumber)
{ 
// 
if(USART_GetFlagStatus(USART1,USART_FLAG_TC) == SET)
  {
	 DMA_Cmd(DMA1_Channel4,DISABLE); 
	 DMA_ClearFlag(DMA1_FLAG_TC4);
	 USART_ClearFlag(USART1,USART_FLAG_TC);
	 DMA1_Channel4->CNDTR =SendNumber;
	 DMA1_Channel4->CMAR=(u32)pBuffer;
	 DMA_Cmd(DMA1_Channel4,ENABLE);
	 
	 return 1;
  }
 return 0;
}

u8 Usart2_Send(char *pBuffer,u32 SendNumber)
{ 
// 
if(USART_GetFlagStatus(USART2,USART_FLAG_TC) == SET)
  {
	 DMA_Cmd(DMA1_Channel7,DISABLE); 
	 DMA_ClearFlag(DMA1_FLAG_TC4);
	 USART_ClearFlag(USART2,USART_FLAG_TC);
	 DMA1_Channel7->CNDTR = SendNumber;
	 DMA1_Channel7->CMAR  =(u32)pBuffer;
	 DMA_Cmd(DMA1_Channel7,ENABLE);
	 return 1;
  }
 return 0;
}


char *Usart1_Read(u16 *Num,u8 swait){
  u16 i=0;
  //
  if((Usart_Sta & (USART1R_OPEN | USART1R_OVER))==(USART1R_OPEN | USART1R_OVER)){
	 //clean receive buffer 
	 for(i=0;i<Usart1_RxCount;i++)
		Usart1_RxBuffer[i]=0;
	 //clean flag
	 Usart1_Ropen();
  }

  if(swait){
	 while(!(Usart_Sta & USART1R_OVER)){};
	 *Num=Usart1_RxCount;
		Usart_Sta |= USART1R_OPEN;
  }
  else{
		if(Usart_Sta & USART1R_OVER){
			*Num=Usart1_RxCount;
			Usart_Sta |= USART1R_OPEN;
		}
		else  *Num=0;
  }
	
  return Usart1_RxBuffer;
}



char *Usart2_Read(u16 *Num,u8 swait){
  u16 i=0;
  
  if((Usart_Sta & (USART2R_OPEN | USART2R_OVER))==(USART2R_OPEN | USART2R_OVER)){
	 //clean receive buffer 
	 for(i=0;i<Usart2_RxCount;i++)
		Usart2_RxBuffer[i]=0;
	 //clean flag
	 Usart2_Ropen();
  }

  if(swait){
	 while(!(Usart_Sta & USART2R_OVER)){};
	 *Num=Usart2_RxCount;
		Usart_Sta |= USART2R_OPEN;
  }
  else{
		if(Usart_Sta & USART2R_OVER){
			*Num=Usart2_RxCount;
			Usart_Sta |= USART2R_OPEN;
		}
		else  *Num=0;
  }
	
  return Usart2_RxBuffer;
}
//---------------------------------------

void Usart1_Ropen(void)
{
	Usart1_RxCount=0;
	Usart_Sta |= USART1R_OPEN;
	Usart_Sta &= USART1R_CLOVER;
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
//	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
//  TIM_Cmd(TIM4,ENABLE);
}


void Usart2_Ropen(void)
{
	Usart2_RxCount = 0;
	Usart_Sta |= USART2R_OPEN;
	Usart_Sta &= USART2R_CLOVER;
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
//	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
//  TIM_Cmd(TIM4,ENABLE);
}
