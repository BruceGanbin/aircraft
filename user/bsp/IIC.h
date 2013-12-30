#ifndef __IIC_H
#define __IIC_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <includes.h>




#define WriteAddress_EEPROM    0xA0
#define ReadAddress_EEPROM     0xA1

#define EE_TimeOut             10000


#define EE_ErrorWTimeOut       0x01
#define EE_ErrorRTimeOut       0x10

extern unsigned char EE_State;

/**********************************************************************
------------------------define the EEPROM Address --------------------
**********************************************************************/





/***********************************************************************
-------------------------function define   ----------------------------
***********************************************************************/


//   unsigned char EEPROM_Write(unsigned char* pBuffer,unsigned int WriteAddress,unsigned char NumbyteToWrite);
//  unsigned char EEPROM_Read(unsigned char* pBuffer,unsigned int ReadAddress,unsigned char NumbyteToWrite);
//   void IIC_ReadBuffer(u8*,u16,u8);

//void IIC_ReadBuffer(unsigned char* pBuffer,unsigned int ReadAddress,unsigned char NumbyteToWrite);
//void IIC_WriteBuffer(unsigned char* pBuffer,unsigned int WriteAddress,unsigned char NumByteToWrite);
u8 *IIC_ReadPage(u8 *,u8,u8,u8);
u8 *IIC_WritePage(u8 *,u8,u8,u8);





#ifdef __cplusplus
}
#endif

#endif
