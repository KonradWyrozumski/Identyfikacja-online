#include "LCD.h"
#include "PID.h"
#include "ADC.h"
#include "stm32f10x_conf.h"
#include <stdio.h>

GPIO_InitTypeDef GPIO_InitStructure;

volatile uint8_t	menu_event = E_IDLE;
volatile uint8_t	current_menu = 0;
extern volatile PIDfloat_t PID;
extern volatile int SP;
extern volatile int PV;

const menu_item menu[] = {
	// U, D, O, E, I
	{{ 1, 1, 1, 0}, NULL, "IDLE"},
	{{ 2, 2, 4, 1}, LCD_ShowPID, "Nastawy reg."},
	{{ 1, 1, 11, 2}, LCD_ShowSPPV, "SP/PV"},
	{{ 2, 1, 3, 3}, NULL, ""},
	{{ 6, 7, 8, 4}, NULL, "OK by edyt. Kp"},
	{{ 5, 5, 2, 5}, NULL, ""},
        {{ 7, 4, 9, 6}, NULL, "OK by edyt. Ti"},
        {{ 4, 6, 10, 7}, NULL, "OK by edyt. Td"},
        {{ 8, 8, 1, 8}, EditKp, "Kp = "},
        {{ 9, 9, 1, 9}, EditTi, "Ti = "},
        {{ 10, 10, 1, 10}, EditTd, "Td = "},
        {{ 11, 11, 2, 11}, EditSP, "SP = "}
};

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void LCD_WriteNibble(u8 nibbleToWrite)
{
	GPIO_WriteBit(LCD_GPIO, LCD_EN, Bit_SET);
	GPIO_WriteBit(LCD_GPIO, LCD_D4, (nibbleToWrite & 0x01));
	GPIO_WriteBit(LCD_GPIO, LCD_D5, (nibbleToWrite & 0x02));
	GPIO_WriteBit(LCD_GPIO, LCD_D6, (nibbleToWrite & 0x04));
	GPIO_WriteBit(LCD_GPIO, LCD_D7, (nibbleToWrite & 0x08));
	GPIO_WriteBit(LCD_GPIO, LCD_EN, Bit_RESET);
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
u8 LCD_ReadNibble(void)
{
	u8 tmp = 0;
	GPIO_WriteBit(LCD_GPIO, LCD_EN, Bit_SET);
	tmp |= (GPIO_ReadInputDataBit(LCD_GPIO, LCD_D4) << 0);
	tmp |= (GPIO_ReadInputDataBit(LCD_GPIO, LCD_D5) << 1);
	tmp |= (GPIO_ReadInputDataBit(LCD_GPIO, LCD_D6) << 2);
	tmp |= (GPIO_ReadInputDataBit(LCD_GPIO, LCD_D7) << 3);
	GPIO_WriteBit(LCD_GPIO, LCD_EN, Bit_RESET);
	return tmp;
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
u8 LCD_ReadStatus(void)
{
	u8 status = 0;
	
	GPIO_InitStructure.GPIO_Pin   =  LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7;
	GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_IPU;
	GPIO_Init(LCD_GPIO, &GPIO_InitStructure);
	
	GPIO_WriteBit(LCD_GPIO, LCD_RW, Bit_SET);
	GPIO_WriteBit(LCD_GPIO, LCD_RS, Bit_RESET);
	
	status |= (LCD_ReadNibble() << 4);
	status |= LCD_ReadNibble();
	
	GPIO_InitStructure.GPIO_Pin   =  LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7;
	GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_Out_PP;
	GPIO_Init(LCD_GPIO, &GPIO_InitStructure);
	
	return status;
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void LCD_WriteData(u8 dataToWrite)
{
	GPIO_WriteBit(LCD_GPIO, LCD_RW, Bit_RESET);
	GPIO_WriteBit(LCD_GPIO, LCD_RS, Bit_SET);
	
	LCD_WriteNibble(dataToWrite >> 4);
	LCD_WriteNibble(dataToWrite & 0x0F);
	
	while(LCD_ReadStatus() & 0x80);
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void LCD_WriteCommand(u8 commandToWrite)
{
	GPIO_WriteBit(LCD_GPIO, LCD_RW | LCD_RS, Bit_RESET);
	LCD_WriteNibble(commandToWrite >> 4);
	LCD_WriteNibble(commandToWrite & 0x0F);
	
	while(LCD_ReadStatus() & 0x80);
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void LCD_WriteText(const char * text)
{
	while(*text)
	  LCD_WriteData(*text++);
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void LCD_GoTo(unsigned char x, unsigned char y)
{
	LCD_WriteCommand(HD44780_DDRAM_SET | (x + (0x40 * y)));
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void LCD_WriteTextXY(const char * text, u8 x, u8 y)
{
	LCD_GoTo(x,y);
	while(*text)
	  LCD_WriteData(*text++);
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void LCD_WriteBinary(u32 var, u8 bitCount)
{
	s8 i;
	
	for(i = (bitCount - 1); i >= 0; i--)
	{
		LCD_WriteData((var & (1 << i))?'1':'0');
	}
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void LCD_ShiftLeft(void)
{
	LCD_WriteCommand(HD44780_DISPLAY_CURSOR_SHIFT | HD44780_SHIFT_LEFT | HD44780_SHIFT_DISPLAY);
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void LCD_ShiftRight(void)
{
	LCD_WriteCommand(HD44780_DISPLAY_CURSOR_SHIFT | HD44780_SHIFT_RIGHT | HD44780_SHIFT_DISPLAY);
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void LCD_Initialize(void)
{
	vu8 i = 0;
	vu32 delayCnt = 0;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin   =  LCD_D4|LCD_D5|LCD_D6|LCD_D7|LCD_RS|LCD_RW|LCD_EN;
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_Out_PP;
	

	GPIO_Init(LCD_GPIO, &GPIO_InitStructure);
        
        GPIO_ResetBits(LCD_GPIO, LCD_RS | LCD_EN | LCD_RW);
	
	for(delayCnt = 0; delayCnt < 300000; delayCnt++);
	
	for(i = 0; i < 3; i++)              
	{
		LCD_WriteNibble(0x03);            
		for(delayCnt = 0;delayCnt < 30000; delayCnt++);
	}
	
	LCD_WriteNibble(0x02);             
	
	for(delayCnt = 0;delayCnt < 6000; delayCnt++);
	   
	LCD_WriteCommand(HD44780_FUNCTION_SET | 
	                 HD44780_FONT5x7 | 
	                 HD44780_TWO_LINE | 
	                 HD44780_4_BIT);
	
	LCD_WriteCommand(HD44780_DISPLAY_ONOFF | 
	                 HD44780_DISPLAY_OFF); 
	
	LCD_WriteCommand(HD44780_CLEAR);
	
	LCD_WriteCommand(HD44780_ENTRY_MODE | 
	                 HD44780_EM_SHIFT_CURSOR | 
	                 HD44780_EM_INCREMENT);
	
	LCD_WriteCommand(HD44780_DISPLAY_ONOFF | 
	                 HD44780_DISPLAY_ON |
	                 HD44780_CURSOR_OFF | 
	                 HD44780_CURSOR_NOBLINK);
}

void LCD_Clear(void)
{
  LCD_WriteCommand(HD44780_CLEAR);
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

/* Obs³uga menu */

void ChangeMenu(void)
{
  //przejdz do nastepnego stanu
  current_menu = menu[current_menu].next_state[menu_event];
  LCD_Clear();
  LCD_WriteTextXY(menu[current_menu].first_line, 0, 0);

  //wywolaj funkcje zwrotna
  if (menu[current_menu].callback)
    menu[current_menu].callback(menu_event);
  //wyswietl komunikat 
  //skasuj zdarzenie
  menu_event = E_IDLE;								
  TIM_Cmd(TIM2, DISABLE);  // Wylacz przerwania od Tim2

}

void LCD_ShowPID(uint8_t event){
  char buff [16];
  sprintf(buff, "P=%.1fI=%.1fD=%.1f", PID._Kp, PID._Ti, PID._Td);
  LCD_WriteTextXY(buff, 0,1);
  TIM_Cmd(TIM3, DISABLE);  // Wy³acz TIM3
}
/* Koniec obs³uga menu */

void EditKp(uint8_t event){
  char buff[16];
  if(menu_event == E_UP)
    PID._Kp += 0.1;
  if(menu_event == E_DOWN)
    PID._Kp -= 0.1;
  sprintf(buff, "%.2f", PID._Kp);
  LCD_WriteTextXY(buff, 0,1);
}
void EditTi(uint8_t event){
  char buff[16];
  if(menu_event == E_UP)
    PID._Ti += 0.1;
  if(menu_event == E_DOWN)
    PID._Ti -= 0.1;
  sprintf(buff, "%.2f", PID._Ti);
  LCD_WriteTextXY(buff, 0,1);
}
void EditTd(uint8_t event){
  char buff[16];
  if(menu_event == E_UP)
    PID._Td += 0.1;
  if(menu_event == E_DOWN)
    PID._Td -= 0.1;
  sprintf(buff, "%.2f", PID._Td);
  LCD_WriteTextXY(buff, 0,1);
}

void EditSP(uint8_t event){
  TIM_Cmd(TIM3, DISABLE);  // Wylacz Tim2
  char buff[16];
  if(menu_event == E_UP){
    if(SP < 4095)
      SP += 100;
  }
  if(menu_event == E_DOWN){
    if(SP > 0)
      SP-=100;
  }
  sprintf(buff, "%d", SP);
  LCD_WriteTextXY(buff, 0,1);
}

void LCD_ShowSPPV(uint8_t event){
  char buff[16];
  sprintf(buff, "SP: %d", SP);
  LCD_WriteTextXY(buff, 0, 0);
  sprintf(buff, "PV: %d", PV);
  LCD_WriteTextXY(buff, 0, 1);
  TIM_Cmd(TIM3, ENABLE);  // Wlacz Tim2
}

void TIM3_IRQHandler(void){
  char buff[16];
  sprintf(buff, "SP: %d", SP);
  LCD_WriteTextXY(buff, 0, 0);
  sprintf(buff, "PV: %d", PV);
  LCD_WriteTextXY(buff, 0, 1);
  TIM_ClearITPendingBit(TIM3,TIM_IT_CC3); // Clear interruption bit

}