#ifndef _FSAE_PINS_H_
#define _FSAE_PINS_H_

#include <stdbool.h>

void Board_GPIO_Init(void);

bool Low_Side_Contactor_Pin_Get(void);

bool High_Side_Contactor_Pin_Get(void);

void High_Side_Contactor_Pin_Set(bool state);

bool IMD_Pin_Get(void);

bool BMS_Pin_Get(void);

bool Interlock_Pin_Get(void);

void Button1_Pin_Set(bool state);

void Button2_Pin_Set(bool state);

void Button3_Pin_Set(bool state);

void Button4_Pin_Set(bool state);

void LCD_DB0_Pin_Set(bool state);

void LCD_DB1_Pin_Set(bool state);

void LCD_DB2_Pin_Set(bool state);

void LCD_DB3_Pin_Set(bool state);

void LCD_DB4_Pin_Set(bool state);

void LCD_DB5_Pin_Set(bool state);

void LCD_DB6_Pin_Set(bool state);

void LCD_DB7_Pin_Set(bool state);

void LCD_E_Pin_Set(bool state);

void LCD_RS_Pin_Set(bool state);

#endif //_FSAE_PINS_H_
