#ifndef ADC_H
#define ADC_H
#include "stm32f10x_conf.h"
#include "delay.h"

#define DCLOCK  GPIO_Pin_0
#define DOUT    GPIO_Pin_8
#define CS_a      GPIO_Pin_6

#define DCLOCK_P  GPIOE
#define DOUT_P    GPIOB
#define CS_a_P      GPIOB

#define DCLOCK_Set    GPIO_WriteBit(DCLOCK_P, DCLOCK, (BitAction)0x01);
#define DCLOCK_Clear  GPIO_WriteBit(DCLOCK_P, DCLOCK, (BitAction)0x00);

#define CS_a_Set    GPIO_WriteBit(CS_a_P, CS_a, (BitAction)0x01);
#define CS_a_Clear  GPIO_WriteBit(CS_a_P, CS_a, (BitAction)0x00);

int AD_value;

int ADC_GetValue(void);

#endif