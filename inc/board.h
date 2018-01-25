#ifndef _BOARD_H_
#define _BOARD_H_

#include "structs.h"
#include "chip.h"
#include "can.h"
#include "console.h"

#define Hertz2Ticks(freq) SystemCoreClock / freq

void Board_Chip_Init(void);

void Board_GPIO_Init(void);

void Board_BlockingDelay(uint32_t dlyTicks);

bool Board_Switch_Read(uint8_t gpio_port, uint8_t pin);

void Board_Can_Init(uint32_t baudRateHz);

void Board_UART_Init(uint32_t baudRateHz);

uint32_t Board_Print(const char *str);

uint32_t Board_Println(const char *str);

uint32_t Board_PrintNum(uint32_t num, uint8_t base);

uint32_t Board_Write(const char *str, uint32_t count);

uint32_t Board_Read(char *charBuffer, uint32_t length);

uint32_t Board_Print_BLOCKING(const char *str);

uint32_t Board_Println_BLOCKING(const char *str);

void Board_Can_ProcessInput(CSB_INPUT_T *csb_input, CSB_STATE_T* csb_state);

bool Board_Contactors_Closed(void);

void Board_GetModeRequest(const CONSOLE_OUTPUT_T *console_output, CSB_INPUT_T *csb_input);

void Board_Can_ProcessOutput(CSB_INPUT_T *csb_input, CSB_STATE_T *csb_state, CSB_OUTPUT_T *csb_output);

void Board_Contactors_Set(bool close_contactors);

void Board_Check_Faults(CSB_INPUT_T *csb_input);

#endif // _BOARD_H_
