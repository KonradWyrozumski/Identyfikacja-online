#include "ADC.h"
int ADC_GetValue(void)  {
  int value = 0;
  int mask = 0x800;
  CS_a_Clear;
  DelayUs(100);
  DCLOCK_Set;
  DelayUs(100);
  DCLOCK_Clear;
  DelayUs(100);
  DCLOCK_Set;
  DelayUs(100);
  DCLOCK_Clear;
  DelayUs(100);
  for(int i = 12; i > 0; i--){
    DCLOCK_Set;
    DelayUs(100);
    DCLOCK_Clear;
    DelayUs(100);
    if(GPIO_ReadInputDataBit(DOUT_P, DOUT))
      value = value | mask;
    mask >>= 1;
    DelayUs(100);
    DCLOCK_Set;
  }
  CS_a_Set;
  DCLOCK_Clear;
  return value;
}