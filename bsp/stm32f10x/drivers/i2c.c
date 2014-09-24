#include "i2c.h"
#include "stm32f10x.h"
#include <rtthread.h>



#define I2C_TRACE(...)x


static unsigned int TimeOut=I2C_TimeOut;
static unsigned char I2C_State=0x00;


//static void dump_i2c_register(I2C_TypeDef* I2Cx)
//
//	 if(I2Cx == I2C1 )
//		 I2C_TRACE("======I2C1======\n");
//	 else
//		 I2C_TRACE("======I2C2======\n");
//	 I2C_TRACE("CR1: 0x%x\tCR2: 0x%x\n", I2Cx->CR1, I2Cx->CR2);
//	 I2C_TRACE("SR1: 0x%x\tSR2: 0x%x\n", I2Cx->SR1, I2Cx->SR2);
//
//


void I2C_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    I2C_InitTypeDef   I2C_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
    
    RCC_APB2PeriphClockCmd(I2C1_GPIO_CLK,ENABLE);
    RCC_APB1PeriphClockCmd(I2C1_CLK,ENABLE);

    GPIO_InitStructure.GPIO_Pin=I2C1_SDA_PIN | I2C1_SCL_PIN;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB,&GPIO_InitStructure);

    /* I2C init */
    I2C_StructInit(&I2C_InitStructure);
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00; 
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;  
    I2C_InitStructure.I2C_ClockSpeed=100000;
    I2C_Init(I2C1,&I2C_InitStructure);
    I2C_Cmd(I2C1,ENABLE);

    I2C_DMACmd(I2C1, ENABLE);
}

static void I2C_Dma_Config(IIC_RT_Typedef Dir,u8 *pbuffer,u16 NumData)
{
    DMA_InitTypeDef DMA_InitStructure;
  
    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_DMA1, ENABLE);
  
    /* Initialize the DMA_PeripheralBaseAddr member */
    DMA_InitStructure.DMA_PeripheralBaseAddr = I2C1_DMA_ADDR;//I2C1->DR;
    /* Initialize the DMA_MemoryBaseAddr member */
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)pbuffer;
    /* Initialize the DMA_PeripheralInc member */
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    /* Initialize the DMA_emoryInc member */
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    /* Initialize the DMA_PeripheralDataSize member */
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    /* Initialize the DMA_MemoryDataSize member */
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    /* Initialize the DMA_Mode member */
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    /* Initialize the DMA_Priority member */
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    /* Initialize the DMA_M2M member */
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  
    /* If using DMA for Reception */
    if (Dir == DMA_RX)
        {
            /* Initialize the DMA_DIR member */
            DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    
            /* Initialize the DMA_BufferSize member */
            DMA_InitStructure.DMA_BufferSize = NumData;
    
            DMA_DeInit(I2C1_DMA_RX_CHANNEL);
    
            DMA_Init(I2C1_DMA_RX_CHANNEL, &DMA_InitStructure);
        }
    /* If using DMA for Transmission */
    else if (Dir == DMA_TX)
        { 
            /* Initialize the DMA_DIR member */
            DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    
            /* Initialize the DMA_BufferSize member */
            DMA_InitStructure.DMA_BufferSize = NumData;
    
            DMA_DeInit(I2C1_DMA_TX_CHANNEL);
    
            DMA_Init(I2C1_DMA_TX_CHANNEL, &DMA_InitStructure);
        }

}    

unsigned char IIC_Write(u8 *pBuffer,u8 PartAddr,u8 WriteAddr,u16 NumByteToWrite)
{
    /*send Start condition,test on EV5 and clear it*/
    TimeOut=I2C_TimeOut;
    I2C_GenerateSTART(I2C1,ENABLE);
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
        {
            if((TimeOut--)==0){I2C_State=I2C_WTimeOut; return I2C_WTimeOut;} 
        }

        /* Configure DMA Peripheral */
    if(NumByteToWrite>1)
        I2C_Dma_Config(DMA_TX,pBuffer,NumByteToWrite);
    /*send Part Address for write,test on EV6 and clear it*/
    TimeOut=I2C_TimeOut;
    I2C_Send7bitAddress(I2C1,PartAddr,I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        {
            if((TimeOut--)==0){I2C_State=I2C_WTimeOut; return I2C_WTimeOut;} 
        }

    
    TimeOut=I2C_TimeOut;
    I2C_SendData(I2C1,(u8)WriteAddr);  
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        {
            if((TimeOut--)==0){I2C_State=I2C_WTimeOut; return I2C_WTimeOut;} 
        }

    if(NumByteToWrite>1)
        {
            I2C_DMACmd(I2C1,ENABLE);
            DMA_Cmd(I2C1_DMA_TX_CHANNEL,ENABLE);
 
            TimeOut=I2C_TimeOut;
            while (!DMA_GetFlagStatus(DMA1_FLAG_TC6))
                {
                    if((TimeOut--)==0){I2C_State=I2C_RTimeOut; return I2C_WTimeOut;} 
                }
            I2C_AcknowledgeConfig(I2C1,DISABLE); 
            I2C_GenerateSTOP(I2C1,ENABLE);
 
            DMA_Cmd(I2C1_DMA_TX_CHANNEL,DISABLE);
            I2C_DMACmd(I2C1,DISABLE);
            DMA_ClearFlag(DMA1_FLAG_TC6);
            I2C_GenerateSTOP(I2C1,ENABLE);
        }
    else
        {
            while(NumByteToWrite--)
                {
                    I2C_SendData(I2C1,*pBuffer);
                    TimeOut=I2C_TimeOut;
                    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
                        {
                            if((TimeOut--)==0){I2C_State=I2C_RTimeOut; return I2C_WTimeOut;} 
                        }
                    pBuffer++;
                }
  
            /*send STOP condition*/
            I2C_GenerateSTOP(I2C1,ENABLE);
        }
    return I2C_NOTimeout;
}

unsigned char IIC_Read(u8 *pBuffer,u8 PartAddr,u8 WriteAddr,u16 NumByteToRead)
{
    I2C_AcknowledgeConfig(I2C1,ENABLE);
    /*send Start condition,test on EV5 and clear it*/
    TimeOut=I2C_TimeOut;
    I2C_GenerateSTART(I2C1,ENABLE);
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
        {
            if((TimeOut--)==0){I2C_State=I2C_RTimeOut; return I2C_RTimeOut;}
        }

    /* Configure DMA Peripheral */
    if(NumByteToRead>1)
        I2C_Dma_Config(DMA_RX,pBuffer,NumByteToRead);
    
    /*send Part Address for write,test on EV6 and clear it*/
    TimeOut=I2C_TimeOut;
    I2C_Send7bitAddress(I2C1,PartAddr,I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        {
            if((TimeOut--)==0){I2C_State=I2C_RTimeOut; return I2C_RTimeOut;}
        }


    TimeOut=I2C_TimeOut;
    /**send the  internal address,test on EV8 and clear it**/
    I2C_SendData(I2C1,(u8)WriteAddr);
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        {
            if((TimeOut--)==0){I2C_State=I2C_RTimeOut; return I2C_RTimeOut;}
        }

    /*send Start condition,test on EV5 and clear it*/
    TimeOut=I2C_TimeOut;
    I2C_GenerateSTART(I2C1,ENABLE);
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
        {
            if((TimeOut--)==0){I2C_State=I2C_RTimeOut; return I2C_RTimeOut;}
        }

    TimeOut=I2C_TimeOut;
    I2C_Send7bitAddress(I2C1,PartAddr,I2C_Direction_Receiver);
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
        {
            if((TimeOut--)==0){I2C_State=I2C_RTimeOut; return I2C_RTimeOut;}
        }

    /* read data*/
    if(NumByteToRead>1)
        {
            I2C_DMACmd(I2C1,ENABLE);
            DMA_Cmd(I2C1_DMA_RX_CHANNEL,ENABLE);
 
            TimeOut=I2C_TimeOut;
            while (!DMA_GetFlagStatus(DMA1_FLAG_TC7))
                {
                    if((TimeOut--)==0){I2C_State=I2C_RTimeOut; return I2C_RTimeOut;} 
                }
            I2C_AcknowledgeConfig(I2C1,DISABLE);
            I2C_GenerateSTOP(I2C1,ENABLE);
 
            DMA_Cmd(I2C1_DMA_RX_CHANNEL,DISABLE);
            I2C_DMACmd(I2C1,DISABLE);
            DMA_ClearFlag(DMA1_FLAG_TC7);
            I2C_GenerateSTOP(I2C1,ENABLE);
        }
    else
        {
            while(NumByteToRead)
                {
                    if(NumByteToRead==1)
                        {
                            I2C_AcknowledgeConfig(I2C1,DISABLE);
                            I2C_GenerateSTOP(I2C1,ENABLE);
                        }
                    if(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED))
                        {
                            *pBuffer=I2C_ReceiveData(I2C1);
                            pBuffer++;
                            NumByteToRead--;
                            TimeOut=I2C_TimeOut;
                        }
                    else
                        {
                            if((TimeOut--)==0){I2C_State=I2C_RTimeOut; return I2C_RTimeOut;}
                        }
    
                }
            I2C_GenerateSTOP(I2C1,ENABLE);            
        }

    return I2C_NOTimeout;
}

