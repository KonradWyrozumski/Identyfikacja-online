#include "stm32f10x_conf.h"
#ifndef DELAY_H
#define DELAY_H

#define SYSTEM_CORE_SPEED 72000000

static __IO uint32_t TimingDelay;
volatile int delayFlag;
void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);
void Delay_Configutation(void);
void DelayUs(unsigned int us);
void DelayMs(unsigned int ms);
void TIM1_CC_IRQHandler(void);
void TIM2_CC_IRQHandler(void);

#endif