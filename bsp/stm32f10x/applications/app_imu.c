#include "rtthread.h"
#include "app_imu.h"
#include "stm32f10x.h"


unsigned int T_count=0;


void TIM5_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM5,TIM_IT_Update)==SET)
    {
        TIM_ClearITPendingBit(TIM5,TIM_FLAG_Update);
        ++T_count;
    }
}

void Tim_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);

    TIM_TimeBaseStructure.TIM_Prescaler = 72-1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 1000-1;
    TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
    TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM5,DISABLE);
}

void TIME5_NVIC_Configuration(void)
{  
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void imu_thread(void *parameter)
{

    while(1)
    {
        rt_thread_delay(10);
    }
}

rt_time_t print_timer;
void imupr_timer(void* parameter)
{
    static unsigned long pcount = 0;
    ++pcount;
    rt_kprintf("count:%d",pcount);
    //    rt_kprintf("roll:%d.%d      pitch:%d.%d      yaw:%d.%d\n",);
 }

void imu_application_init(void)
{
    rt_thread_t imu_tid;

    Tim_Config();
    TIME5_NVIC_Configuration();

    imu_tid = rt_thread_create("imu_app",
                                  imu_thread, RT_NULL,
                                  2048, 5, 1);
    if(imu_tid != RT_NULL)
    {
        rt_thread_startup(imu_tid);
    }
    
    print_timer = rt_timer_create("print_imu",
                                 imupr_timer,
                                 RT_NULL,
                                 100,
                                 RT_TIMER_FLAG_PERIODIC);
}


void imu_test(void)
{
    static unsigned char imu_ap_start = 0;

    if(imu_ap_start)
    {
        imu_ap_start = 0;
        TIM_Cmd(TIM5,DISABLE);
        rt_timer_stop(print_timer);
    }
    else
    {
        imu_ap_start = 1;
        TIM_Cmd(TIM5,ENABLE);
        rt_timer_start(print_timer);
    }
}
#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(imu_test,start or stop imu appliction.)
#endif
