#ifndef __CONFIGINIT_H
#define __CONFIGINIT_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"



	/*****the time-out count  value****/
#define FLAG_TIMEOUT	 ((u16)0x4000)

	/*****usart max byte*******/
#define USART_MAX		 220


	/****ERROR  mark  to register**********/

#define ERROR_RCC		   ((u16)0x0000)
#define RIGHT_RCC			((u16)0x0001)
#define ERROR_ADCSTATE	((u16)0xFFFD)
#define RIGHT_ADCSTATE	((u16)0x0002)





	extern volatile u16 TimeCount;            //delay's count
	extern volatile u8 USART_Rbuffer[USART_MAX];
	extern volatile u8 USART_Tbuffer[USART_MAX];

	/********Status Register**********************/


	//extern  volatile u8  Step;

/*Private functions---------------------------------------------------------*/

u16  RCC_Configuration(void);
void NVIC_Configuration(void);
void TIME_Configuration(void);
void GPIO_Configuration(void);
void DMA_Configuration(void);
void IIC_Configuration(void);
void Usart_Configuration(void);
void PWM_Configuration(void);
void ADC_Confituration(void);  


	/*
void State_init(void);

void TimeCount_Init(void);
u8 delayS(u16 nCount);
void delayms(u32 nCount);
void delayus(u32 nCount);
	*/

#ifdef __cplusplus
}
#endif

#endif

