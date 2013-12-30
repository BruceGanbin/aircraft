#include"configinit.h"


volatile  u16 TimeCount;    //delay's cout

volatile u8 Second;
volatile u8 min;
volatile u8 USART_Rbuffer[USART_MAX];
volatile u8 USART_Tbuffer[USART_MAX];



/***************************************************
 *@ Function     :RCC_Configuration()
 *@ Return Value :Null
 *@ Peremater    :Null
 *@ Brief        :Configure the RCC and enable the perpheral
                 配置时钟以及使能外设
***************************************************/
u16 RCC_Configuration(void)
{  
 volatile  u16 rcc_TimeOut;
  ErrorStatus HSEStatUpStatus;
  RCC_DeInit();
  RCC_HSEConfig(RCC_HSE_ON);
  HSEStatUpStatus=RCC_WaitForHSEStartUp();
  if(HSEStatUpStatus==SUCCESS)
    {
      RCC_HCLKConfig(RCC_SYSCLK_Div1);
      RCC_PCLK2Config(RCC_HCLK_Div1);
      RCC_PCLK1Config(RCC_HCLK_Div2);
      RCC_ADCCLKConfig(RCC_PCLK2_Div2);
 //     RCC_PLLConfig(RCC_PLLSource_PREDIV1,RCC_PLLMul_3) ;  //F100andF103 are different
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);
      RCC_PLLCmd(ENABLE);
      rcc_TimeOut=FLAG_TIMEOUT;
      while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET) 
		  {	if((rcc_TimeOut--)==0) return ERROR_RCC;	}
		
      RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);  
      rcc_TimeOut=FLAG_TIMEOUT;
      while(RCC_GetSYSCLKSource()!=0x08)
		  {	if((rcc_TimeOut--)==0) return ERROR_RCC;	}
    }
  else 
	 {	return ERROR_RCC;	} 
    
  /*******************端口时钟使能*********************/
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_AFIO|
						       RCC_APB2Periph_TIM1  |RCC_APB2Periph_ADC1|
						       RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB|
						       RCC_APB2Periph_GPIOC |RCC_APB2Periph_GPIOD|
						       RCC_APB2Periph_GPIOE
						      ,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1|RCC_APB1Periph_TIM2|
						       RCC_APB1Periph_TIM4|RCC_APB1Periph_TIM5|
						       RCC_APB1Periph_TIM3|RCC_APB1Periph_USART2,ENABLE); //  

  return RIGHT_RCC;
}


/***************************************************
 *@ Function     :Usart_Configuration()
 *@ Return Value :Null
 *@ Peremater    :Null
 *@ Brief        :Configure the usart and enable the interrupt
                 配置串口并且打开串口中断
***************************************************/
void Usart_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;
  USART_StructInit(&USART_InitStructure);
  USART_InitStructure.USART_BaudRate=9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART2,&USART_InitStructure);
  USART_Init(USART1,&USART_InitStructure);

  USART_Cmd(USART1,ENABLE);
  USART_Cmd(USART2,ENABLE);

  USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
  USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
  //  USART_ITConfig(USART1,USART_IT_TXE,ENABLE);
  USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);
  USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);

}


/***************************************************
 *@ Function     :GPIO_Configuration()
 *@ Return Value :Null
 *@ Peremater    :Null
 *@ Brief        :Configure the GPIO 
                 配置GPIO
***************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  //  GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);//Remap USART1 IO,重定义串口1   IO 
  //  GPIO_PinRemapConfig(GPIO_Remap_I2C1,ENABLE);  //Remap I2C1   IO,重定义I2C1 IO
  //  GPIO_PinRemapConfig(GPIO_FullRemap_TIM1,ENABLE);//Remap TIME1 ALL the IO 
  // GPIO_PinRemapConfig(GPIO_FullRemap_TIM2,ENABLE);
  /***Configure the GPIOB_9 for USART Transmit Data mode****/  
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_2;	  
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
  /***Configure the GPIOB_10 for USART Receive  Data mode****/  
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_3;	 
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA,&GPIO_InitStructure);

  /***Configure the GPIOB_6 for I2C1 SCL mode***************/
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_OD;
  GPIO_Init(GPIOB,&GPIO_InitStructure);					     
  /***Configure the GPIOB_7 for I2C1 SDA mode*************/  
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_OD;  
  GPIO_Init(GPIOB,&GPIO_InitStructure);		 	
  
  /***Configure the PWM TIM1*************/ 
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_14;//
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
  GPIO_Init(GPIOE,&GPIO_InitStructure);

  /***Configure the PWM TIM2*************/ 
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;//
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_10|GPIO_Pin_11;//
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB,&GPIO_InitStructure);
  /***Configure the PWM TIM3*************/ 
 
  /***Configure the PWM TIM4*************/ 

  /*****Configure the PWM Underarm*******/
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
  GPIO_Init(GPIOE,&GPIO_InitStructure);
 /*
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC,&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
  GPIO_Init(GPIOE,&GPIO_InitStructure);  **/

  /***Configure the GROUP1 ADC************
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
  GPIO_Init(GPIOA,&GPIO_InitStructure);*/ 
}


/***************************************************
 *@ Function     :NVIC_Configuration()
 *@ Return Value :Null
 *@ Peremater    :Null
 *@ Brief        :中断优先级配置
***************************************************/
void NVIC_Configuration(void) 
{
  NVIC_InitTypeDef NVIC_InitStructure;	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); 
  NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
  NVIC_Init(&NVIC_InitStructure);

/*
  NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
  NVIC_Init(&NVIC_InitStructure);
*/
  NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
  NVIC_Init(&NVIC_InitStructure);

	
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); 
  NVIC_InitStructure.NVIC_IRQChannel=DMA1_Channel7_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel=DMA1_Channel4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}


/**
*************************************************
 @Function     :
 @Return_Value :Null
 @Peremater    :Null
 @Brief        :
*************************************************/
void DMA_Configuration(void)
{
  DMA_InitTypeDef DMA_InitStructure;
	u8 tempBuffer=0;
//  DMA_DeInit(DMA1_Channel4);  
//  DMA form USART2_TX
  DMA_InitStructure.DMA_PeripheralBaseAddr=0x40004404;//USART2_TX 
  DMA_InitStructure.DMA_MemoryBaseAddr=(u32)tempBuffer;
  DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize=1;
  DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority=DMA_Priority_High;
  DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;
  DMA_Init(DMA1_Channel7,&DMA_InitStructure);
//  DMA form USART2_TX  
  DMA_InitStructure.DMA_PeripheralBaseAddr=0x40013804;//USART1_TX 
  DMA_Init(DMA1_Channel4,&DMA_InitStructure);

  //   DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);
  //   DMA_ITConfig(DMA1_Channel7,DMA_IT_TC,ENABLE);
}




/**
*************************************************
 @Function     :IIC_ConfigControl(void)
 @Return Value :Null
 @Peremater    :Null
 @Brief        :Configure the IIC
***************************************************/
void IIC_Configuration(void)
{
  I2C_InitTypeDef  I2C_InitStructure;
  I2C_StructInit(&I2C_InitStructure);
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; //Tlow/Thigh=2
  I2C_InitStructure.I2C_OwnAddress1 = 0x00; 
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;//使能I2C的应答
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;  
  I2C_InitStructure.I2C_ClockSpeed=200000;
  I2C_Init(I2C1,&I2C_InitStructure);
  I2C_Cmd(I2C1,ENABLE);
}
 



/**
*************************************************
@Function     :TIME_Configuration()
@Return_Value :Null
@Peremater    :Null
@Brief        :
***************************************************/
void TIME_Configuration(void)
{
  TIM_TimeBaseInitTypeDef TimeBaseStructure;

//  TIM_TimeBaseInit(TIM2,&TimeBaseStructure);
//  TIM_TimeBaseInit(TIM3,&TimeBaseStructure);
  //  TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);

  TimeBaseStructure.TIM_Prescaler=7199;
  TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
  TimeBaseStructure.TIM_Period=99;
  TIM_TimeBaseInit(TIM4,&TimeBaseStructure);

  TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
  TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
  TIM_Cmd(TIM4,DISABLE);

}                                                     



/**
*************************************************
 @Function     :PWM_Confituration(void)
 @Return_Value :Null
 @Peremater    :Null
 @Brief        :
***************************************************/
void PWM_Configuration(void)
{
  TIM_OCInitTypeDef TIMOC_InitStructure;


  TIMOC_InitStructure.TIM_OCMode=TIM_OCMode_PWM1;
  TIMOC_InitStructure.TIM_OutputState=TIM_OutputState_Enable;
  TIMOC_InitStructure.TIM_OutputNState=TIM_OutputState_Disable;
  TIMOC_InitStructure.TIM_Pulse=10;
  TIMOC_InitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
  TIMOC_InitStructure.TIM_OCNPolarity=TIM_OCPolarity_Low;
  TIMOC_InitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;
  TIMOC_InitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset;
  TIM_OC1Init(TIM1,&TIMOC_InitStructure);
  TIM_OC2Init(TIM1,&TIMOC_InitStructure);
  TIM_OC3Init(TIM1,&TIMOC_InitStructure);
  TIM_OC4Init(TIM1,&TIMOC_InitStructure); 
//  TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
//  TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);
//  TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);
//  TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable); 
//  TIM_Cmd(TIM1,ENABLE);
//  TIM_CtrlPWMOutputs(TIM1,ENABLE);

//  TIM_OC1Init(TIM2,&TIMOC_InitStructure);
//  TIM_OC2Init(TIM2,&TIMOC_InitStructure);
//  TIM_OC3Init(TIM2,&TIMOC_InitStructure);


}


void Group1_SetPWM(u8 compare)
{
  compare=100-compare;
  TIM_SetCompare1(TIM1,compare);
  TIM_SetCompare2(TIM1,compare);
  TIM_SetCompare3(TIM1,compare);
}



/**
*************************************************
 @Function     :ADC_Confituration(void)
 @Return_Value :Null
 @Peremater    :Null
 @Brief        :
***************************************************/
void ADC_Confituration(void)
{
  ADC_InitTypeDef  ADC_InitStructure;
  
  ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode=ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
  ADC_InitStructure. ADC_DataAlign=ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel=3;
  ADC_Init(ADC1,&ADC_InitStructure);

  ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_1Cycles5);
  ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_1Cycles5);
  ADC_RegularChannelConfig(ADC1,ADC_Channel_2,3,ADC_SampleTime_1Cycles5);
//  ADC_RegularChannelConfig(ADC1,ADC_Channel_3,4,ADC_SampleTime_1Cycles5);
                        
  ADC_DMACmd(ADC1,ENABLE);
  ADC_Cmd(ADC1,ENABLE);
  

}




/**
*************************************************
 *@Function     :ADC_StartCmd(FunctionalState NewState)
 *@Return_Value :Null
 *@Peremater    :
 *@Brief        :
***************************************************/
u16  ADC_StartCmd(FunctionalState NewState)
{
  volatile u16 adcState_TimeOut;
  if(NewState != DISABLE)
	 {
  //		DMA_Configuration();

		ADC_ResetCalibration(ADC1);
		adcState_TimeOut=FLAG_TIMEOUT;
		while(ADC_GetResetCalibrationStatus(ADC1))
		  {	if((adcState_TimeOut--)==0)     return ERROR_ADCSTATE;	}

		ADC_StartCalibration(ADC1);
		adcState_TimeOut=FLAG_TIMEOUT;
		while(ADC_GetCalibrationStatus(ADC1))
		  {	if((adcState_TimeOut--)==0)     return ERROR_ADCSTATE;	}
        
		DMA_Cmd(DMA1_Channel1,ENABLE);
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	 }
  else
	 {  
		DMA_Cmd(DMA1_Channel1,DISABLE);
		ADC_SoftwareStartConvCmd(ADC1,DISABLE);// 				*可能需要删除*
	 }
  return RIGHT_ADCSTATE;
}





/***************************************************
 *@ Function     :delayus(u32)
 *@ Return Value :Null
 *@ Peremater    :uint32
 *@ Brief        :
 ***********************************************/
void delayus(u32 nCount)
{
 if (SysTick_Config (SystemCoreClock / 1000000)) 
  {  while (1); }
  TimeCount=nCount;
  while(TimeCount);
  SysTick->CTRL=0x00; 
  SysTick->VAL =0X00; 
}

void delayms(u32 nCount)
{
  if (SysTick_Config (SystemCoreClock / 1000)) 
  {  while (1){}; }
  TimeCount=nCount;
  while(TimeCount)
	{};
  SysTick->CTRL=0x00;
  SysTick->VAL =0X00; 
}


/*
void delayus(u32 nCount)
{
  while(nCount--)
	 {
		__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();
	 }
}
*/
