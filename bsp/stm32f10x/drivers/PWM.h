#ifndef PWM_H
#define PWM_H

#include "stm32f10x.h"

#define PWM1_PIN                    GPIO_Pin_8
#define PWM1_PORT                   GPIOA
#define PWM1_GPIO_CLK          		RCC_APB2Periph_GPIOA

#define PWM2_PIN                    GPIO_Pin_1
#define PWM2_PORT                   GPIOA
#define PWM2_GPIO_CLK          		RCC_APB2Periph_GPIOA

#define PWM3_PIN                    GPIO_Pin_0
#define PWM3_PORT                   GPIOB
#define PWM3_GPIO_CLK          		RCC_APB2Periph_GPIOB

#define PWM4_PIN                    GPIO_Pin_8
#define PWM4_PORT                   GPIOB
#define PWM4_GPIO_CLK          		RCC_APB2Periph_GPIOB

#define PWM_FREQ                    1000  //1000kHz

typedef enum
{
    FREQ = 0,
    DUTY
}PWM_Typedef;

void pwm_init(void);
unsigned char pwm_set(PWM_Typedef FR_DU,unsigned int parameter,unsigned char channel);

#endif

