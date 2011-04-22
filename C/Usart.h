#ifndef USART_H
#define USART_H
#include "stm32f10x_conf.h"
#include <stdio.h>
USART_InitTypeDef USART_InitStructure;

float USARTBuff[1];
void USART_Configuration(void);
void __putchar(int ch);

#endif