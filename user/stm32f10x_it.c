/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    10/15/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "usart.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/



/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
 
void PendSV_Handler(void)
{
} */

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
*/
void SysTick_Handler(void)
{															 
TimeCount--;
}  

/**
   * @brief  This function handles TIM2 global interrupt request.
   * @param  None
   * @retval None
   */
void TIM2_IRQHandler(void)
{


}

/**
   * @brief  This function handles TIM2 global interrupt request.
   * @param  None
   * @retval None
   */
void TIM5_IRQHandler(void)
{

} 

/**
   * @brief  This function handles TIM7 global interrupt request.
   * @param  None
   * @retval None
	***/   
static u8 Usart1_TimeOut=0;
static u8 Usart2_TimeOut=0;
static u8 Uart_TimeEN=0;
void TIM4_IRQHandler(void)
{
//  TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE);
  if(TIM_GetITStatus(TIM4, TIM_IT_Update))
  {
	 if(Usart_Sta & USART1R_OPEN)
	 {
		if( Usart1_TimeOut>=50)
		{
		//		TIM_Cmd(TIM4,DISABLE);
		USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);  
		Usart_Sta|=USART1R_OVER;
		}
		else
			Usart1_TimeOut++;
	 }
	 
	 if(Usart_Sta & USART2R_OPEN)
	 {
		 if(Usart2_TimeOut>=50 )
		 {
		//		TIM_Cmd(TIM4,DISABLE);
		USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);  
		Usart_Sta|=USART2R_OVER;
		 }
		 else
			 Usart2_TimeOut++;
	 }
		
	 if((Usart_Sta & (USART1R_OVER|USART2R_OVER)) == (USART1R_OVER|USART2R_OVER))
	 {
		 TIM_Cmd(TIM4,DISABLE);	Uart_TimeEN=0;
		 Usart_Sta &= (~(USART1R_OPEN|USART2R_OPEN));
	 }


  }
  TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
  //if(  )TIM_Cmd(TIM4,DISABLE);
//  TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);

}    

/**
  * @brief  This function handles USART1_IRQHandler.
  * @param  None
  * @retval None
  */

void USART1_IRQHandler(void)
{
  if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
  {
	 USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);			   
	 if(Uart_TimeEN==0)
		{
		  TIM_Cmd(TIM4,ENABLE);
			Uart_TimeEN=1;
      Usart1_RxCount=0;			
		  Usart_Sta &= USART1R_CLOVER;
		}

	 Usart1_RxBuffer[Usart1_RxCount]=USART_ReceiveData(USART1);
	 Usart1_RxCount++;
	 Usart1_TimeOut=0;
	 USART_ClearITPendingBit(USART1,USART_IT_RXNE); 
  
	 USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);  
  }    
}

void USART2_IRQHandler(void)
{
  if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
  {
	 USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);			   
	 if(Uart_TimeEN==0)
		{
		  TIM_Cmd(TIM4,ENABLE);
			Uart_TimeEN=1;
      Usart2_RxCount=0;			
		  Usart_Sta &= USART2R_CLOVER;
		}
	 Usart2_RxBuffer[Usart2_RxCount]=USART_ReceiveData(USART2);
	 Usart2_RxCount++;
	 Usart2_TimeOut=0;
	 USART_ClearITPendingBit(USART2,USART_IT_RXNE); 
  
	 USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);  	 
  }    
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
