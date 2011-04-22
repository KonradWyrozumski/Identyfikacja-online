#include "delay.h"
#include "stm32f10x_conf.h"

void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

/* Timer 1 counting delay */

void Delay_Configutation(void)  {
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 
   TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 

   TIM_TimeBaseStructure.TIM_ClockDivision=0; 
   TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
   TIM_TimeBaseStructure.TIM_Period=72; 
   TIM_TimeBaseStructure.TIM_Prescaler=1; 

   TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure); 

   TIM_OCInitTypeDef TIM_OCInitStructure; 

   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing; 
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
   TIM_OCInitStructure.TIM_Pulse = 72;   // T= 1us! 
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 

   TIM_OC1Init(TIM1, &TIM_OCInitStructure); 

   TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable); 
   TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE); 

   NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the TIM1 gloabal Interrupt */
   NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

   NVIC_Init(&NVIC_InitStructure);

  /* Tim3 do odœwie¿ania LCD */
   int SystemCoreClock = 7200000;
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

   TIM_TimeBaseStructure.TIM_ClockDivision=0; 
   TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
   TIM_TimeBaseStructure.TIM_Period=65535;
   TIM_TimeBaseStructure.TIM_Prescaler=(SystemCoreClock / 6000)-1; // 1kHz

   TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);

   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
   TIM_OCInitStructure.TIM_Pulse = 3000;
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 

   TIM_OC1Init(TIM3, &TIM_OCInitStructure); 

   TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable); 
   TIM_ITConfig(TIM3, TIM_IT_CC3, ENABLE); 

  /* Enable the TIM2 gloabal Interrupt */
   NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

   NVIC_Init(&NVIC_InitStructure);
}

void DelayUs(unsigned int us) { 
   TIM1->CNT=0;           // Clear timer count
   TIM1->PSC=us;          // Set prescaler
   TIM_Cmd(TIM1, ENABLE); // Start counter
   while(delayFlag!=0);   // Wait for end of delay
   delayFlag=1;
} 

void DelayMs(unsigned int ms) { 
   int i; 
   for(i=0; i<=(ms-1);++i) 
      DelayUs(1000);
}

void TIM1_CC_IRQHandler(void) { 
   delayFlag=0;                 // Clear flag
   TIM_Cmd(TIM1, DISABLE);      // Turn off timer
   TIM_ClearITPendingBit(TIM1,TIM_IT_CC1); // Clear interruption bit
}

