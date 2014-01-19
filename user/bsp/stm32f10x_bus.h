#ifndef __IIC_H
#define __IIC_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <includes.h>

	/***
		 IIC 
	 ***/
#define WriteAddress_EEPROM    0xA0
#define ReadAddress_EEPROM     0xA1

#define EE_TimeOut             10000

#define EE_ErrorWTimeOut       0x01
#define EE_ErrorRTimeOut       0x10

extern unsigned char EE_State;


/***********************************************************************
----------------------IIC function define   ----------------------------
***********************************************************************/
//   unsigned char EEPROM_Write(unsigned char* pBuffer,unsigned int WriteAddress,unsigned char NumbyteToWrite);
//  unsigned char EEPROM_Read(unsigned char* pBuffer,unsigned int ReadAddress,unsigned char NumbyteToWrite);
//   void IIC_ReadBuffer(u8*,u16,u8);

//void IIC_ReadBuffer(unsigned char* pBuffer,unsigned int ReadAddress,unsigned char NumbyteToWrite);
//void IIC_WriteBuffer(unsigned char* pBuffer,unsigned int WriteAddress,unsigned char NumByteToWrite);
u8 *IIC_ReadPage(u8 *,u8,u8,u8);
u8 *IIC_WritePage(u8 *,u8,u8,u8);



/***********************************************************************
----------------------USART function define   -------------------------
***********************************************************************/
#define USART1R_OVER      0x01
#define USART1R_CLOVER    0xFE
#define USART1R_OPEN      0x08
#define USART2R_OVER      0x10
#define USART2R_CLOVER    0xEF
#define USART2R_OPEN      0x80
	 
#define USART_RHOLD       1
#define USART_RNHOLD      0

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
   
extern char *Usart1_Read(u16 *Num,u8 hold);
extern char *Usart2_Read(u16 *Num,u8 hold);

extern void Usart1_Ropen(void);
extern void Usart2_Ropen(void);








#ifdef __cplusplus
}
#endif

#endif
