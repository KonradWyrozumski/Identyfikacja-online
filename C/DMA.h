#ifndef DMA_H
#define DMA_H
#include "stm32f10x_conf.h"

#define ADC1_DR_Address    ((u32)0x4001244C)
vu16 ADC_ConvertedValue;
void DMA_Configuration(void);

#endif