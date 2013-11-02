#include "includes.h"







/***************************************************************************************************
**************************************************************************************************
                                          main
**************************************************************************************************
***************************************************************************************************/

int main(void)
{
  u16 Init_error=0x00;

  Init_error=RCC_Configuration();
  while(!Init_error)
  {}
  NVIC_Configuration();
  GPIO_Configuration();
  Usart_Configuration();
  TIME_Configuration();
  IIC_Configuration();

  while(1)
  {


  }
  return (0);
} 

/***************************************************************************************************
**************************************************************************************************
                                          main
**************************************************************************************************
***************************************************************************************************/

void App_TaskStart(void *p_arg)
{
  
  while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET){};
  USART_SendData(USART2,'A');
  while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET){};
  USART_SendData(USART2,'T');

  while(1)
  {
	 USART_SendData(USART2,0x55);

  }
}
