#include "DAC.h"
#include "delay.h"

void DAC_Configuration(void)  {
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = LD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(LD_P, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = CS;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(CS_P, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = CLK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(CLK_P, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = SDI;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(SDI_P, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = CLR;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(CLR_P, &GPIO_InitStructure);

  CLR_Set;
}

bool DAC_TransmitByte(int data)  { 
  LD_Set;
  int i; 
  if (data > 4096) 
    return 0; 
  for(i = 12; i > 0; i--){ 
    if (data & 0x800) { 
      SDI_Set;    // When 1 is transfered SDI is high 
    } 
    else  { 
      SDI_Clear;  // else is low 
    } 
    data <<= 1; 
    DelayUs(1); 
    CLK_Clear; 
    DelayUs(1); 
    CLK_Set; 
    DelayUs(1); 
  }
  CS_Set;   // Deselect chip 
  DelayUs(10); 
  LD_Clear; // End of transfer, shift temp register to output 
  DelayUs(10); 
  LD_Set; // End of transfer, shift temp register to output 
  return 1;
}

void DAC_ClearOutput(void)  {
  CS_Clear;   // Select chip
  CLR_Clear;  // Clear output
  DelayUs(100);
  CLR_Set;  // Set CLR
  CS_Set;  // Deselect chip
}