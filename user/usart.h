#ifndef __USART_H
#define __USART_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"

#define USART1R_OVER      0x01
#define USART1R_CLOVER    0xFE
#define USART1R_OPEN      0x08
#define USART2R_OVER      0x10
#define USART2R_CLOVER    0xEF
#define USART2R_OPEN      0x80
	 

extern char Usart1_RxBuffer[500];
extern char Usart1_TxBuffer[500];
extern char Usart2_RxBuffer[500];
extern char Usart2_TxBuffer[500];

extern u8   Usart1_RxCount;
extern u8   Usart2_RxCount;
extern u8   Usart_Sta;

extern u16  Usart_Start(u8 Set);

extern u8 Usart1_Send(char *pBuffer,u32 SendNumber);
extern u8 Usart2_Send(char *pBuffer,u32 SendNumber);
	//	void Clear_UsartBuffer(void);
   
extern char *Usart1_Read(u16 *Num);
extern char *Usart2_Read(u16 *Num);

extern void Usart1_Ropen(void);
















#ifdef __cplusplus
}
#endif

#endif
