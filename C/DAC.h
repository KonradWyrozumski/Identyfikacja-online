#ifndef DAC_H
#define DAC_H

#include "stm32f10x_conf.h"

#define LD  GPIO_Pin_10
#define CS  GPIO_Pin_11
#define CLK GPIO_Pin_8
#define SDI GPIO_Pin_2
#define CLR GPIO_Pin_10

#define LD_P GPIOE
#define CS_P  GPIOD
#define CLK_P GPIOE
#define SDI_P GPIOB
#define CLR_P GPIOE

#define LD_Set    GPIO_WriteBit(LD_P, LD, (BitAction)0x01);
#define CS_Set    GPIO_WriteBit(CS_P, CS, (BitAction)0x01);
#define CLK_Set   GPIO_WriteBit(CLK_P, CLK, (BitAction)0x01);
#define SDI_Set   GPIO_WriteBit(SDI_P, SDI, (BitAction)0x01);
#define CLR_Set   GPIO_WriteBit(CLR_P, CLR, (BitAction)0x01);

#define LD_Clear    GPIO_WriteBit(LD_P, LD, (BitAction)0x00);
#define CS_Clear    GPIO_WriteBit(CS_P, CS, (BitAction)0x00);
#define CLK_Clear   GPIO_WriteBit(CLK_P, CLK, (BitAction)0x00);
#define SDI_Clear   GPIO_WriteBit(SDI_P, SDI, (BitAction)0x00);
#define CLR_Clear   GPIO_WriteBit(CLR_P, CLR, (BitAction)0x00);

void DAC_Configuration(void);
bool DAC_TransmitByte(int);
void DAC_ClearOutput(void);

#endif