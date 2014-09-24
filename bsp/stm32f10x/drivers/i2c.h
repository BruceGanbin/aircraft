#ifndef I2C_H
#define I2C_H

#include "stm32f10x.h"



#define I2C1_SDA_PIN                  GPIO_Pin_7
#define I2C1_SCL_PIN                  GPIO_Pin_6
#define I2C1_GPIO_PORT          	  GPIOB 
#define I2C1_GPIO_CLK          		  RCC_APB2Periph_GPIOB
#define I2C1_CLK                      RCC_APB1Periph_I2C1

#define I2C2_SDA_PIN                  GPIO_Pin_11
#define I2C2_SCL_PIN                  GPIO_Pin_10
#define I2C2_GPIO_PORT          	  GPIOB
#define I2C2_GPIO_CLK          		  RCC_APB2Periph_GPIOB
#define I2C2_CLK                      RCC_APB1Periph_I2C2

#define I2C_MEM_1Byte			      1
#define I2C_MEM_2Bytes				  2

#define I2C_TimeOut                   10000

#define I2C_WTimeOut                  0x08
#define I2C_RTimeOut                  0x0F
#define I2C_NOTimeout                 0x00

#define I2C1_DMA_ADDR                 ((uint32_t)0x40005410)
#define I2C1_DMA_CLK                  RCC_AHBPeriph_DMA1
#define I2C1_DMA_TX_CHANNEL           DMA1_Channel6
#define I2C1_DMA_RX_CHANNEL           DMA1_Channel7

typedef enum
{
    DMA_TX = 0,
    DMA_RX = 1
}IIC_RT_Typedef;

void I2C_init(void);
unsigned char IIC_Write(u8 *pBuffer,u8 PartAddr,u8 WriteAddr,u16 NumByteToRead);
unsigned char IIC_Read(u8 *pBuffer,u8 PartAddr,u8 WriteAddr,u16 NumByteToRead);

#endif

