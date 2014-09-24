#include "pwm.h"
#include "stm32f10x.h"

static GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //    RCC_APB2PeriphClockCmd(PWM1_GPIO_CLK|PWM2_GPIO_CLK|PWM3_GPIO_CLK|PWM4_GPIO_CLK|RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);
                                                   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = PWM1_PIN | PWM2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = PWM3_PIN | PWM4_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

static TIM_Config(u32 sys_freq)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
    
    TIM_TimeBaseStructure.TIM_Prescaler = sys_freq/1000000-1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = PWM_FREQ-1;
    // timer 1
    TIM_DeInit(TIM1);
    TIM_InternalClockConfig(TIM1);
    TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
    TIM_Cmd(TIM1, ENABLE);
 
    //timer 2
    TIM_DeInit(TIM2);
    TIM_InternalClockConfig(TIM2);
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
    TIM_Cmd(TIM2, ENABLE);
 
    //timer 3
    TIM_DeInit(TIM3);
    TIM_InternalClockConfig(TIM3);
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
    TIM_Cmd(TIM3, ENABLE);

    //timer 4

    TIM_DeInit(TIM4);
    TIM_InternalClockConfig(TIM4);
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
    TIM_Cmd(TIM4, ENABLE);
}

static PWM_Config(void)
{
    TIM_OCInitTypeDef TimOCInitStructure;
    
    TIM_OCStructInit(&TimOCInitStructure);
    TimOCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TimOCInitStructure.TIM_Pulse = 500-1;
    TimOCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TimOCInitStructure.TIM_OutputState = TIM_OutputState_Enable;

    TIM_OC1Init(TIM1, &TimOCInitStructure);
    TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
    TIM_CtrlPWMOutputs(TIM1,ENABLE);

    TIM_OC2Init(TIM2, &TimOCInitStructure);
    TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable); 
    
    TIM_OC3Init(TIM3, &TimOCInitStructure);
    TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable); 

    TIM_OC3Init(TIM4, &TimOCInitStructure);
    TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable); 
}

void pwm_init(void)
{
    RCC_ClocksTypeDef ClockFreq;

    RCC_GetClocksFreq(&ClockFreq);
    rt_kprintf("SYSCLK : %d \n",ClockFreq.SYSCLK_Frequency);
    rt_kprintf("HCLK : %d \n",ClockFreq.HCLK_Frequency);
    rt_kprintf("PCLK1 : %d \n",ClockFreq.PCLK1_Frequency);
    rt_kprintf("PCLK2 : %d \n",ClockFreq.PCLK2_Frequency);
    rt_kprintf("ADCCLK : %d \n",ClockFreq.ADCCLK_Frequency);

    GPIO_Config();
    TIM_Config(ClockFreq.SYSCLK_Frequency);
    PWM_Config();
}

unsigned char pwm_set(PWM_Typedef FR_DU,unsigned int parameter,unsigned char channel)
{
    if(FR_DU == FREQ)
    {
        switch(channel)
        {
        case 1:
            TIM_PrescalerConfig(TIM1,parameter,TIM_PSCReloadMode_Immediate);
            //            TIM_SetAutoreload(TIM1,parameter);
            break;
        case 2:
            TIM_PrescalerConfig(TIM2,parameter,TIM_PSCReloadMode_Immediate);
            //            TIM_SetAutoreload(TIM2,parameter);
            break;
        case 3:
            TIM_PrescalerConfig(TIM3,parameter,TIM_PSCReloadMode_Immediate);
            //            TIM_SetAutoreload(TIM3,parameter);
            break;
        case 4:
            TIM_PrescalerConfig(TIM4,parameter,TIM_PSCReloadMode_Immediate);
            //            TIM_SetAutoreload(TIM4,parameter);
            break;
        default:
            break;
        }
        rt_kprintf("set Freq OK \n");
    }
    else if(FR_DU == DUTY)
    {
        switch(channel)
        {
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        default:
            break;
        }
        rt_kprintf("set Duty OK \n");
    }
}
#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(pwm_set, set pwm (1)[FREQ or DUTY] (2) parameter (3) channel .)
#endif
