#if defined (HSE_Value)
  #define HSE_VALUE HSE_Value
#endif

#ifndef HSE_VALUE
  #error W projekcie nie zdefiniowano czestotliwosci kwarcu.
#endif

#if HSE_VALUE == 25000000
  #define RCC_PREDIV2_DivX RCC_PREDIV2_Div5
#elif HSE_VALUE == 20000000
  #define RCC_PREDIV2_DivX RCC_PREDIV2_Div4
#elif HSE_VALUE == 15000000
  #define RCC_PREDIV2_DivX RCC_PREDIV2_Div3
#elif HSE_VALUE == 10000000
  #define RCC_PREDIV2_DivX RCC_PREDIV2_Div2
#elif HSE_VALUE == 5000000
  #define RCC_PREDIV2_DivX RCC_PREDIV2_Div1
#else
  #error Bledna wartosc HSE_VALUE
#endif

#include "check.h"
#include "clock.h"
#include "stm32f10x_conf.h"


/* Konfiguruj wszystkie wyprowadzenia w analogowym trybie
   wejściowym (ang. analog input mode, trigger off), co
   redukuje zużycie energii i zwiększa odporność na
   zakłócenia elektromagnetyczne. */
void AllPinsDisable() {
  GPIO_InitTypeDef GPIO_InitStruct;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                         RCC_APB2Periph_GPIOB |
                         RCC_APB2Periph_GPIOC |
                         RCC_APB2Periph_GPIOD |
                         RCC_APB2Periph_GPIOE,
                         ENABLE);
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStruct.GPIO_Speed = 0;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
  GPIO_Init(GPIOB, &GPIO_InitStruct);
  GPIO_Init(GPIOC, &GPIO_InitStruct);
  GPIO_Init(GPIOD, &GPIO_InitStruct);
  GPIO_Init(GPIOE, &GPIO_InitStruct);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                         RCC_APB2Periph_GPIOB |
                         RCC_APB2Periph_GPIOC |
                         RCC_APB2Periph_GPIOD |
                         RCC_APB2Periph_GPIOE,
                         DISABLE);
}

int ClockConfigure(unsigned freqMHz) {
  static const int maxAttempts = 1000000;

  RCC_DeInit();
  RCC_HSEConfig(RCC_HSE_ON);
  /* Wykonuje maksymalnie HSEStartUp_TimeOut = 1280 prób. */
  if (RCC_WaitForHSEStartUp() == ERROR)
    return -1;

  FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
  if (freqMHz > 48)
    FLASH_SetLatency(FLASH_Latency_2);
  else if (freqMHz > 14) /* Dokumentacja mówi o 24 MHz. */
    FLASH_SetLatency(FLASH_Latency_1);
  else
    FLASH_SetLatency(FLASH_Latency_0);

  if (freqMHz >= 48)
    /* preskaler AHB, HCLK = SYSCLK = freqMHz */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
  else if (freqMHz >= 24)
    /* preskaler AHB, HCLK = SYSCLK / 2 = freqMHz */
    RCC_HCLKConfig(RCC_SYSCLK_Div2);
  else
    /* preskaler AHB, HCLK = SYSCLK / 4 = freqMHz */
    RCC_HCLKConfig(RCC_SYSCLK_Div4);

  /* preskaler APB1, PCLK1 = HCLK / 2 = freqMHz / 2 */
  RCC_PCLK1Config(RCC_HCLK_Div2);

  /* preskaler APB2, PCLK2 = HCLK = freqMHz */
  RCC_PCLK2Config(RCC_HCLK_Div1);

  /* PLL2: PLL2CLK = HSE / RCC_PREDIV2_DivX * 8 = 40 MHz */
  RCC_PREDIV2Config(RCC_PREDIV2_DivX);
  RCC_PLL2Config(RCC_PLL2Mul_8);
  RCC_PLL2Cmd(ENABLE);
  active_check(RCC_GetFlagStatus(RCC_FLAG_PLL2RDY), maxAttempts);

  /* PLL1: PLLCLK = (PLL2 / 5) * RCC_PLLMul */
  RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div5);
  if (freqMHz == 48 || freqMHz == 24 || freqMHz == 12)
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_6);
  else if (freqMHz == 56 || freqMHz == 28 || freqMHz == 14)
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_7);
  else if (freqMHz == 64 || freqMHz == 32 || freqMHz == 16)
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_8);
  else if (freqMHz == 72 || freqMHz == 36 || freqMHz == 18)
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);
  else
    return -1;
  RCC_PLLCmd(ENABLE);
  active_check(RCC_GetFlagStatus(RCC_FLAG_PLLRDY), maxAttempts);

  /* Ustaw SYSCLK = PLLCLK i czekaj aż PLL zostanie ustawiony
     jako zegar systemowy. */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  active_check(RCC_GetSYSCLKSource() == 0x08, maxAttempts);

  return 0;
}
