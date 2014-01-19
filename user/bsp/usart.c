#include "usart.h"
#include "stm32f10x.h"




char Usart1_RxBuffer[500];
char Usart1_TxBuffer[500];
char Usart2_RxBuffer[500];
char Usart2_TxBuffer[500];
u8   Usart1_RxCount=0;
u8   Usart2_RxCount=0;
u8   Usart_Sta=(USART1R_OVER|USART2R_OVER);
/****   , , , , , , ,Uart1_TimeOver      ***/

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


char *Usart1_Read(u16 *Num)
{
//  u16 TimeOver=0;
//  u16 i=0;
//  for(i=0;i<Usart1_RxCount;i++)
//	 Usart1_RxBuffer[i]=0;

  USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
  Usart_Sta |= USART1R_OPEN;	
  Usart_Sta &= USART1R_CLOVER;
  while(!(Usart_Sta & USART1R_OVER)){};
  *Num=Usart1_RxCount;
  return Usart1_RxBuffer;
}



char *Usart2_Read(u16 *Num,u8 swait)
{
  u16 i=0;
  u16 zero=0;

  if(!Usart_Sta&USART2R_OPEN){
	 //clean receive buffer 
	 for(i=0;i<Usart2_RxCount;i++)
		Usart2_RxBuffer[i]=0;
	 //clean flay
	 Usart2_RxCount = 0;
	 Usart_Sta |= USART2R_OPEN;
	 Usart_Sta &= USART2R_CLOVER;

	 Usart2_Ropen();
  }

  if(swait){
	 while(!(Usart_Sta & USART2R_OVER)){};
	 *Num=Usart2_RxCount;
  }
  else{
	 *Num=&zero;
  }


  return Usart2_RxBuffer;
}


void Usart1_Ropen(void)
{
	Usart1_RxCount=0;
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
//	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
//  TIM_Cmd(TIM4,ENABLE);
}


void Usart2_Ropen(void)
{
	Usart2_RxCount=0;
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
//	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
//  TIM_Cmd(TIM4,ENABLE);
}
