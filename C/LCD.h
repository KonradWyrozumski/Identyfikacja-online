#ifndef LCD_H
#define LCD_H

#include "stm32f10x_conf.h"
#include "Interrupt.h"
#include <stdlib.h>

#define LCD_GPIO GPIOD

#define LCD_D4 GPIO_Pin_7
#define LCD_D5 GPIO_Pin_5
#define LCD_D6 GPIO_Pin_3
#define LCD_D7 GPIO_Pin_1

#define LCD_RS GPIO_Pin_8
#define LCD_RW GPIO_Pin_10
#define LCD_EN GPIO_Pin_12

#define HD44780_CLEAR                      0x01
#define HD44780_HOME                       0x02

#define HD44780_ENTRY_MODE                 0x04
#define HD44780_EM_SHIFT_CURSOR            0
#define HD44780_EM_SHIFT_DISPLAY	   1
#define HD44780_EM_DECREMENT		   0
#define HD44780_EM_INCREMENT		   2

#define HD44780_DISPLAY_ONOFF		   0x08
#define HD44780_DISPLAY_OFF		   0
#define HD44780_DISPLAY_ON		   4
#define HD44780_CURSOR_OFF		   0
#define HD44780_CURSOR_ON		   2
#define HD44780_CURSOR_NOBLINK             0
#define HD44780_CURSOR_BLINK		   1

#define HD44780_DISPLAY_CURSOR_SHIFT       0x10
#define HD44780_SHIFT_CURSOR		   0
#define HD44780_SHIFT_DISPLAY		   8
#define HD44780_SHIFT_LEFT		   0
#define HD44780_SHIFT_RIGHT		   4
#define HD44780_FUNCTION_SET		   0x20
#define HD44780_FONT5x7			   0
#define HD44780_FONT5x10		   4
#define HD44780_ONE_LINE		   0
#define HD44780_TWO_LINE		   8
#define HD44780_4_BIT			   0
#define HD44780_8_BIT			   16

#define HD44780_CGRAM_SET		   0x40

#define HD44780_DDRAM_SET		   0x80

/* Obs³uga menu */

#define E_UP    0
#define E_DOWN  1
#define E_OK    2
#define E_IDLE  3

extern volatile uint8_t	menu_event;
extern volatile uint8_t	current_menu;

typedef struct
{
	uint8_t next_state[4];            //przejœcia do nastêpnych stanów
	void (*callback)(uint8_t event);  //funkcja zwrotna
	const char* first_line;           //tekst dla 1. linii LCD
 
} menu_item;



void ChangeMenu(void);

/* Koniec obs³uga menu */

void LCD_Initialize(void);
void LCD_WriteData(u8);
void LCD_WriteText(const char *);
void LCD_WriteTextXY(const char * , u8 , u8);
void LCD_GoTo(unsigned char, unsigned char);
void LCD_WriteBinary(u32 var, u8 bitCount);
void LCD_Clear(void);
void LCD_ShowPID(uint8_t event);
void EditKp(uint8_t event);
void EditTi(uint8_t event);
void EditTd(uint8_t event);
void EditSP(uint8_t event);
void LCD_ShowSPPV(uint8_t event);

void TIM3_IRQHandler(void);

#endif