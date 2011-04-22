#include "stm32f10x_conf.h"
#include "RCC.h"
#include "Matrix.h"
#include "arma.h"
#include "PID.h"
#include "delay.h"
#include "Interrupt.h"
#include "DMA.h"
#include "ADC.h"
#include "Usart.h"
#include "DAC.h"
#include "ADC.h"
#include "GPIO.h"
#include "LCD.h"

#include <stdio.h>
#include <stdlib.h>
#include <cross_studio_io.h>
  
  // Stopien zidentyfikowanego obiektu
extern volatile int St;
  // Tablica odpowiedzi obiektu z chwili t:t-d (d - opoznienie)
extern fxpnt_matrix_t *odpowiedziObiektu;
  // Wartosc zadana zadana na obiekcie
extern float wartoscZadana;
  // Wyjscie z regulatora w chwili t
extern float wyjscieReg;
  // Wyjscie z regulatora w chwili t-1
extern float wyjscieRegPop;
  // Opoznienie obiektu
extern int d;
  // Regulator PID
extern volatile PIDfloat_t PID;
  // wartoœæ zadana
extern volatile int SP;
  // wartoœæ aktualna z obiektu
extern volatile int PV;
  // B³¹d regulatora
int E = 0;  
void main(void)
{

/* --------- Konfiguracja systemu -------- */
  
  RCC_Configuration();
  DMA_Configuration();
  Delay_Configutation();
  GPIO_Configuration();
  USART_Configuration();
  DAC_Configuration();
/* --------------------------------------- */

  DAC_TransmitByte(0);
/* --------- Identyfikacja wstêpna -------- */
  U = initMatrix(40,1);
  Y = initMatrix(40,1);
  TestujObiekt();
  DAC_TransmitByte(0);
  IdentyfikujObiekt(5, 0);
  SP = 1200;
//  showMatrix(teta);
  initPID(3.06, 2.69, 0.45, 100.0 , 0.01, 4095.0 , 0, &PID);
  DobierzNastawy(&Y, &U, &PID, 0, 10);
/* ---------------------------------------- */
  while(1){
    DAC_TransmitByte(3276);
    DelayMs(50);
  }

  LCD_Initialize();
  while(1){
      if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)){
        DelayMs(70);
        if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13))
          menu_event = E_UP;
      }
      if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)){
        DelayMs(70);
        if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5))
          menu_event = E_DOWN;
      }
      if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4)){
        DelayMs(100);
        if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4))
          menu_event = E_OK;
      }
      if(menu_event != E_IDLE)
        ChangeMenu();
      PV = ADC_GetValue();
      E = SP-PV;
      wyjscieReg = PIDOdpowiedz(E, &PID);
      if(wyjscieReg < 0)
        wyjscieReg = 1;
      DAC_TransmitByte(wyjscieReg);
      DelayMs(50);
  }
     
  
  uint16_t Y;
  while(1){
    while(USART_GetFlagStatus(USART1, USART_IT_RXNE) == RESET);
    Y = USART_ReceiveData(USART1);
    Y<<=8;
    while(USART_GetFlagStatus(USART1, USART_IT_RXNE) == RESET);
    Y |= USART_ReceiveData(USART1);
    printf("%f\r", PIDOdpowiedz(wartoscZadana - Y, &PID));
    DelayMs(10);
  }
  m_free(&Ld);
  m_free(&Md);
  m_free(&teta);
  m_free(&P);
  m_free(&fi);
}
