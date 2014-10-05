/*
 * File      : led.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */
#include <rtthread.h>
#include <stm32f10x.h>
#include <led.h>

// led define
#define LEDB1                       0x01
#define LEDB2                       0x02
#define LEDB3                       0x04
#define LEDB4                       0x08

#define led1_rcc                    RCC_APB2Periph_GPIOC
#define led1_gpio                   GPIOC
#define led1_pin                    (GPIO_Pin_9)

#define led2_rcc                    RCC_APB2Periph_GPIOC
#define led2_gpio                   GPIOC
#define led2_pin                    (GPIO_Pin_8)

#define led3_rcc                    RCC_APB2Periph_GPIOC
#define led3_gpio                   GPIOC
#define led3_pin                    (GPIO_Pin_7)

#define led4_rcc                    RCC_APB2Periph_GPIOC
#define led4_gpio                   GPIOC
#define led4_pin                    (GPIO_Pin_6)

static struct rt_timer timer1;
//static struct rt_timer timer2;
//static struct rt_timer timer3;
//static struct rt_timer timer4;

// sorft timer interrupt
static rt_uint8_t ledstart = 0;
static void timeout_led1(void* parameter)
{
    if(ledstart & LEDB1)
    {
        rt_hw_led_on(0);
        ledstart &= ~LEDB1;
    }
    else
    {
        rt_hw_led_off(0);
        ledstart |= LEDB1;
    }
}


static void timeout_led2(void* parameter)
{
    if(ledstart & LEDB2)
    {
        rt_hw_led_on(1);
        ledstart &= ~LEDB2;
    }
    else
    {
        rt_hw_led_off(1);
        ledstart |= LEDB2;
    }
}

static void timeout_led3(void* parameter)
{
    if(ledstart & LEDB3)
    {
        rt_hw_led_on(2);
        ledstart &= ~LEDB3;
    }
    else
    {
        rt_hw_led_off(2);
        ledstart |= LEDB3;
    }
}

static void timeout_led4(void* parameter)
{
    if(ledstart & LEDB4)
    {
        rt_hw_led_on(3);
        ledstart &= ~LEDB4;
    }
    else
    {
        rt_hw_led_off(3);
        ledstart |= LEDB4;
    }
}


void rt_hw_led_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(led1_rcc|led2_rcc|led3_rcc|led4_rcc,ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin   = led1_pin|led2_pin|led3_pin|led4_pin;
    GPIO_Init(led1_gpio, &GPIO_InitStructure);

    rt_timer_init(&timer1,"timer1",timeout_led1,RT_NULL,100,RT_TIMER_FLAG_PERIODIC);
    //    rt_timer_init(&timer2,"timer2",timeout_led2,RT_NULL,100,RT_TIMER_FLAG_PERIODIC);
    //    rt_timer_init(&timer3,"timer3",timeout_led3,RT_NULL,100,RT_TIMER_FLAG_PERIODIC);
    //    rt_timer_init(&timer4,"timer4",timeout_led4,RT_NULL,100,RT_TIMER_FLAG_PERIODIC);

    //turn off  all the led
    rt_hw_led_off(99);

}

void rt_hw_led_off(rt_uint32_t n)
{
    switch (n)
    {
    case 0:
        GPIO_SetBits(led1_gpio, led1_pin);
        break;
    case 1:
        GPIO_SetBits(led2_gpio, led2_pin);
        break;
    case 2:
        GPIO_SetBits(led3_gpio, led3_pin);
        break;
    case 3:
        GPIO_SetBits(led4_gpio, led4_pin);
        break;
    default:
        GPIO_SetBits(led1_gpio, led1_pin);
       GPIO_SetBits(led2_gpio, led2_pin);
       GPIO_SetBits(led3_gpio, led3_pin);
       GPIO_SetBits(led4_gpio, led4_pin);
       break;
    }
}

void rt_hw_led_on(rt_uint32_t n)
{
    switch (n)
    {
    case 0:
        GPIO_ResetBits(led1_gpio, led1_pin);
        break;
    case 1:
        GPIO_ResetBits(led2_gpio, led2_pin);
        break;
    case 2:
        GPIO_ResetBits(led3_gpio, led3_pin);
        break;
    case 3:
        GPIO_ResetBits(led4_gpio, led4_pin);
        break;
    default:
        GPIO_ResetBits(led1_gpio, led1_pin);
        GPIO_ResetBits(led2_gpio, led2_pin);
        GPIO_ResetBits(led3_gpio, led3_pin);
        GPIO_ResetBits(led4_gpio, led4_pin);
        break;
    }
}

#ifdef RT_USING_FINSH
#include <finsh.h>
//static rt_uint8_t led_inited = 0;
void led(rt_uint32_t led, rt_uint32_t value)
{
    /* init led configuration if it's not inited. */
//    if (!led_inited)
//    {
//        rt_hw_led_init();
//        led_inited = 1;
//    }

    
    if ( led == 0 )
    {
        /* set led status */
        switch (value)
        {
        case 0:
            rt_hw_led_off(0);
            break;
        case 1:
            rt_hw_led_on(0);
            break;
        default:
            break;
        }
    }

    if ( led == 1 )
    {
        /* set led status */
        switch (value)
        {
        case 0:
            rt_hw_led_off(1);
            break;
        case 1:
            rt_hw_led_on(1);
            break;
        default:
            break;
        }
    }

    if ( led == 2 )
    {
        /* set led status */
        switch (value)
        {
        case 0:
            rt_hw_led_off(2);
            break;
        case 1:
            rt_hw_led_on(2);
            break;
        default:
            break;
        }
    }

    if ( led == 3 )
    {
        /* set led status */
        switch (value)
        {
        case 0:
            rt_hw_led_off(3);
            break;
        case 1:
            rt_hw_led_on(3);
            break;
        default:
            break;
        }
    }
}
FINSH_FUNCTION_EXPORT(led, set led[0 - 3] on[1] or off[0].)
#endif

void led_flash(rt_uint32_t led,rt_uint32_t freq)
{

    rt_timer_t set_timer;

    switch(led)
        {
        case 0:
            set_timer = &timer1;
            break;

//        case 1:
//            set_timer = &timer2;
//            break;
// 
//        case 2:
//            set_timer = &timer3;
//            break;
// 
//        case 3:
//            set_timer = &timer4;
//            break;
// 
//        default:
//            rt_timer_stop(&timer1);
//            rt_timer_stop(&timer2);
//            rt_timer_stop(&timer3);
//            rt_timer_stop(&timer4);
//            return;
        }


    if(freq == 0)
    {
        rt_timer_stop(set_timer);
        return;
    }

    freq/=10;
    rt_timer_control(set_timer,RT_TIMER_CTRL_SET_TIME,(void*)&freq);
    rt_timer_start(set_timer);
}

#ifdef RT_USING_FINSH
FINSH_FUNCTION_EXPORT(led_flash, set flash[0 - 4] time[10ms - 100s] 1=1ms.)
#endif
