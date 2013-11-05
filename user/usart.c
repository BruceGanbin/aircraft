#include "usart.h"
#include "stm32f10x.h"

/**
*************************************************
 @Function     :Usart_Send()
 @Return_Value :Null
 @Peremater    :
 @Brief        :
***************************************************/
void Usart_Send(u8 pBuffer,u16 SendNumber)
{ 
// 需要设置发送完成标志（未设）  
if(USART_GetFlagStatus(USART1,USART_FLAG_TC) == SET)
  {
	 DMA_Cmd(DMA1_Channel4,DISABLE); 
	 DMA_ClearFlag(DMA1_FLAG_TC4);
	 USART_ClearFlag(USART1,USART_FLAG_TC);
	 DMA1_Channel4->CNDTR =SendNumber;
	 DMA1_Channel4->CMAR=(u32)pBuffer;
	 DMA_Cmd(DMA1_Channel4,ENABLE);
  }
}


/**
*************************************************
 @Function     :Clear_UsartBuffer()
 @Return_Value :Null
 @Peremater    :Null
 @Brief        :
***************************************************/
void Clear_UsartBuffer(void)
{
  u8 i;
  i=25;
  do
	 {
    USART_Tbuffer[i]=0x00;
		USART_Rbuffer[i]=0x00;
	 }while(i--);

}

