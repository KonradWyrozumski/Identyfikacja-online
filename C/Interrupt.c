#include "Interrupt.h"
#include "delay.h"
#include "GPIO.h"
#include "Usart.h"
#include "arma.h"
#include "ADC.h"
#include "DAC.h"
#include "PID.h"

#include <cross_studio_io.h>

void SysTickHandler(void) {
/*  for(int i = 0; i < d - 1; i++)
    odpowiedziObiektu->matrix[i][0] = odpowiedziObiektu->matrix[i+1][0];
  odpowiedziObiektu->matrix[d-1][0] = ADC_GetValue();
  wyjscieRegPop = wyjscieReg;
  wyjscieReg = PID(odpowiedziObiektu->matrix[d-1][0],
                              wartoscZadana,
                              &Kp, &Ti, &Td);
  DAC_TransmitByte((int)wyjscieReg);

  AktualizujM(wyjscieReg, wyjscieRegPop, odpowiedziObiektu->matrix[0][0], St);
  */
  printf("%d", ADC_GetValue());
}


void USART1_IRQHandler(void){
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    USARTBuff[0] = USART_ReceiveData(USART1);
}