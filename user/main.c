#include "includes.h"
#include "string.h"






/***************************************************************************************************
**************************************************************************************************
                                          main
**************************************************************************************************
***************************************************************************************************/

int main(void)
{
  u16 Init_error=0;
  BT_InitTypedef BT_configure;
  // config
  Init_error=RCC_Configuration();
  while(!Init_error){}
  NVIC_Configuration();
  GPIO_Configuration();
  Usart_Configuration();
  TIME_Configuration();
  IIC_Configuration();
  DMA_Configuration();
  //
//  USART_SendData(USART1,'O');
//	delayms(500);	
//  Usart1_Send("1234567",7);
//	delayms(500);
  Usart1_Ropen();	
  BT_configure.BT_ReadData=Usart1_Read;
  BT_configure.BT_SendData=Usart1_Send;
  BT_configure.BT_Name="Bruce";
  BT_configure.BT_Auth=AUTH0;
  BT_configure.BT_Chk=CHK_ODD;
  BT_configure.BT_StopBit=STOPBIT1;
  BT_configure.BT_PIN="88888";


  BT_Init(&BT_configure);
  delayms(500);	
  Init_error=1;
  while(1)
  {
	 

  }
	
//  return 0;
} 

/***************************************************************************************************
**************************************************************************************************
                                          main
**************************************************************************************************
***************************************************************************************************/


