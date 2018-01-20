#include "pins.h"

#include "chip.h"

// Low Side Contactor
#define CTR_LOW_SWTCH 1, 2
#define IOCON_CTR_LOW_SWTCH IOCON_PIO1_2

// High Side Contactor
#define CTR_HIGH_SWTCH 1, 1
#define IOCON_CTR_HIGH_SWTCH IOCON_PIO1_1

// IMD input
#define IMD_IN 2, 4
#define IOCON_IMD_IN IOCON_PIO2_4

// BMS input
#define BMS_IN 2, 5
#define IOCON_BMS_IN IOCON_PIO2_5

// Interlock input
#define INT_IN 2, 6
#define IOCON_INT_IN IOCON_PIO2_6

// Button 1
#define BUT_1 2, 8
#define IOCON_BUT_1 IOCON_PIO2_8

// Button 2
#define BUT_2 2, 9
#define IOCON_BUT_2 IOCON_PIO2_9

// Button 3
#define BUT_3 2, 10
#define IOCON_BUT_3 IOCON_PIO2_10

// Button 4
#define BUT_4 2, 11
#define IOCON_BUT_4 IOCON_PIO2_11

// LCD DB0
#define LCD_DB0 0, 2
#define IOCON_LCD_DB0 IOCON_PIO0_2

// LCD DB1
#define LCD_DB1 0, 3
#define IOCON_LCD_DB1 IOCON_PIO0_3

// LCD DB2
#define LCD_DB2 0, 4
#define IOCON_LCD_DB2 IOCON_PIO0_4

// LCD DB3
#define LCD_DB3 0, 5
#define IOCON_LCD_DB3 IOCON_PIO0_5

// LCD DB4
#define LCD_DB4 0, 6
#define IOCON_LCD_DB4 IOCON_PIO0_6

// LCD DB5
#define LCD_DB5 0, 7
#define IOCON_LCD_DB5 IOCON_PIO0_7

// LCD DB6
#define LCD_DB6 0, 8
#define IOCON_LCD_DB6 IOCON_PIO0_8

// LCD DB7
#define LCD_DB7 0, 9
#define IOCON_LCD_DB7 IOCON_PIO0_9

// LCD E
#define LCD_E 0, 10
#define IOCON_LCD_E IOCON_PIO0_10

// LCD RS
#define LCD_RS 0, 11
#define IOCON_LCD_RS IOCON_PIO0_11

// Unused pins
#define PIN_33 1, 0
#define PIN_33_IOCON IOCON_PIO1_0
#define PIN_33_PIO_FUNC IOCON_FUNC1
#define PIN_39 1, 3
#define PIN_39_IOCON IOCON_PIO1_3
#define PIN_39_PIO_FUNC IOCON_FUNC1
#define PIN_40 1, 4
#define PIN_40_IOCON IOCON_PIO1_4
#define PIN_40_PIO_FUNC IOCON_FUNC0
#define PIN_45 1, 5
#define PIN_45_IOCON IOCON_PIO1_5
#define PIN_45_PIO_FUNC IOCON_FUNC0
#define PIN_9 1, 8
#define PIN_9_IOCON IOCON_PIO1_8
#define PIN_9_PIO_FUNC IOCON_FUNC0
#define PIN_17 1, 9
#define PIN_17_IOCON IOCON_PIO1_9
#define PIN_17_PIO_FUNC IOCON_FUNC0
#define PIN_30 1, 10
#define PIN_30_IOCON IOCON_PIO1_10
#define PIN_30_PIO_FUNC IOCON_FUNC0
#define PIN_42 1, 11
#define PIN_42_IOCON IOCON_PIO1_11
#define PIN_42_PIO_FUNC IOCON_FUNC0
#define PIN_2 2, 0
#define PIN_2_IOCON IOCON_PIO2_0
#define PIN_2_PIO_FUNC IOCON_FUNC0
#define PIN_13 2, 1
#define PIN_13_IOCON IOCON_PIO2_1
#define PIN_13_PIO_FUNC IOCON_FUNC0
#define PIN_26 2, 2
#define PIN_26_IOCON IOCON_PIO2_2
#define PIN_26_PIO_FUNC IOCON_FUNC0
#define PIN_38 2, 3
#define PIN_38_IOCON IOCON_PIO2_3
#define PIN_38_PIO_FUNC IOCON_FUNC0
#define PIN_11 2, 7
#define PIN_11_IOCON IOCON_PIO2_7
#define PIN_11_PIO_FUNC IOCON_FUNC0
#define PIN_36 3, 0
#define PIN_36_IOCON IOCON_PIO3_0
#define PIN_36_PIO_FUNC IOCON_FUNC0
#define PIN_37 3, 1
#define PIN_37_IOCON IOCON_PIO3_1
#define PIN_37_PIO_FUNC IOCON_FUNC0
#define PIN_43 3, 2
#define PIN_43_IOCON IOCON_PIO3_2
#define PIN_43_PIO_FUNC IOCON_FUNC0
#define PIN_48 3, 3
#define PIN_48_IOCON IOCON_PIO3_3
#define PIN_48_PIO_FUNC IOCON_FUNC0

void Board_GPIO_Init(void) {
  Chip_GPIO_Init(LPC_GPIO);

  // Low Side Contactor
  Chip_GPIO_SetPinDIROutput(LPC_GPIO, CTR_LOW_SWTCH);
  Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_CTR_LOW_SWTCH,
          (IOCON_FUNC1 | IOCON_MODE_INACT));

  // High Side Contactor
  Chip_GPIO_SetPinDIROutput(LPC_GPIO, CTR_HIGH_SWTCH);
  Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_CTR_HIGH_SWTCH,
          (IOCON_FUNC1 | IOCON_MODE_INACT));

  // IMD input
  Chip_GPIO_SetPinDIROutput(LPC_GPIO, IMD_IN);
  Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_IMD_IN,
          (IOCON_FUNC0 | IOCON_MODE_INACT));

  // BMS input
  Chip_GPIO_SetPinDIROutput(LPC_GPIO, BMS_IN);
  Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_BMS_IN,
          (IOCON_FUNC0 | IOCON_MODE_INACT));

  // Interlock input
  Chip_GPIO_SetPinDIROutput(LPC_GPIO, INT_IN);
  Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_INT_IN,
          (IOCON_FUNC0 | IOCON_MODE_INACT));

  // Button 1
  Chip_GPIO_SetPinDIROutput(LPC_GPIO, BUT_1);
  Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_BUT_1,
          (IOCON_FUNC0 | IOCON_MODE_INACT));

  // Button 2
  Chip_GPIO_SetPinDIROutput(LPC_GPIO, BUT_2);
  Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_BUT_2,
          (IOCON_FUNC0 | IOCON_MODE_INACT));

  // Button 3
  Chip_GPIO_SetPinDIROutput(LPC_GPIO, BUT_3);
  Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_BUT_3,
          (IOCON_FUNC0 | IOCON_MODE_INACT));

  // Button 4
  Chip_GPIO_SetPinDIROutput(LPC_GPIO, BUT_4);
  Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_BUT_4,
          (IOCON_FUNC0 | IOCON_MODE_INACT));

  // LCD DB0
  Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_DB0);
  Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_LCD_DB0,
          (IOCON_FUNC0 | IOCON_MODE_INACT));

  // LCD DB1
  Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_DB1);
  Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_LCD_DB1,
          (IOCON_FUNC0 | IOCON_MODE_INACT));

  // LCD DB2
  Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_DB2);
  Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_LCD_DB2,
          (IOCON_FUNC0 | IOCON_MODE_INACT));

  // LCD DB3
  Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_DB3);
  Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_LCD_DB3,
          (IOCON_FUNC0 | IOCON_MODE_INACT));

  // LCD DB4
  Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_DB4);
  Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_LCD_DB4,
          (IOCON_FUNC0 | IOCON_MODE_INACT));

  // LCD DB5
  Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_DB5);
  Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_LCD_DB5,
          (IOCON_FUNC0 | IOCON_MODE_INACT));

  // LCD DB6
  Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_DB6);
  Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_LCD_DB6,
          (IOCON_FUNC0 | IOCON_MODE_INACT));

  // LCD DB7
  Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_DB7);
  Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_LCD_DB7,
          (IOCON_FUNC0 | IOCON_MODE_INACT));

  // LCD E
  Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_E);
  Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_LCD_E,
          (IOCON_FUNC1 | IOCON_MODE_INACT));

  // LCD RS
  Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_RS);
  Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_LCD_RS,
          (IOCON_FUNC1 | IOCON_MODE_INACT));

  // Enable pull down resistors on unused pins
  Chip_GPIO_SetPinDIROutput(LPC_GPIO, PIN_33);
  Chip_IOCON_PinMuxSet(LPC_IOCON, PIN_33_IOCON,
      (PIN_33_PIO_FUNC | IOCON_MODE_PULLDOWN) );
  Chip_GPIO_SetPinState(LPC_GPIO, PIN_33, false);

  Chip_GPIO_SetPinDIROutput(LPC_GPIO, PIN_39);
  Chip_IOCON_PinMuxSet(LPC_IOCON, PIN_39_IOCON,
      (PIN_39_PIO_FUNC | IOCON_MODE_PULLDOWN) );
  Chip_GPIO_SetPinState(LPC_GPIO, PIN_39, false);

  Chip_GPIO_SetPinDIROutput(LPC_GPIO, PIN_40);
  Chip_IOCON_PinMuxSet(LPC_IOCON, PIN_40_IOCON,
      (PIN_40_PIO_FUNC | IOCON_MODE_PULLDOWN) );
  Chip_GPIO_SetPinState(LPC_GPIO, PIN_40, false);

  Chip_GPIO_SetPinDIROutput(LPC_GPIO, PIN_45);
  Chip_IOCON_PinMuxSet(LPC_IOCON, PIN_45_IOCON,
      (PIN_45_PIO_FUNC | IOCON_MODE_PULLDOWN) );
  Chip_GPIO_SetPinState(LPC_GPIO, PIN_45, false);

  Chip_GPIO_SetPinDIROutput(LPC_GPIO, PIN_9);
  Chip_IOCON_PinMuxSet(LPC_IOCON, PIN_9_IOCON,
      (PIN_9_PIO_FUNC | IOCON_MODE_PULLDOWN) );
  Chip_GPIO_SetPinState(LPC_GPIO, PIN_9, false);

  Chip_GPIO_SetPinDIROutput(LPC_GPIO, PIN_17);
  Chip_IOCON_PinMuxSet(LPC_IOCON, PIN_17_IOCON,
      (PIN_17_PIO_FUNC | IOCON_MODE_PULLDOWN) );
  Chip_GPIO_SetPinState(LPC_GPIO, PIN_17, false);

  Chip_GPIO_SetPinDIROutput(LPC_GPIO, PIN_30);
  Chip_IOCON_PinMuxSet(LPC_IOCON, PIN_30_IOCON,
      (PIN_30_PIO_FUNC | IOCON_MODE_PULLDOWN) );
  Chip_GPIO_SetPinState(LPC_GPIO, PIN_30, false);

  Chip_GPIO_SetPinDIROutput(LPC_GPIO, PIN_42);
  Chip_IOCON_PinMuxSet(LPC_IOCON, PIN_42_IOCON,
      (PIN_42_PIO_FUNC | IOCON_MODE_PULLDOWN) );
  Chip_GPIO_SetPinState(LPC_GPIO, PIN_42, false);

  Chip_GPIO_SetPinDIROutput(LPC_GPIO, PIN_2);
  Chip_IOCON_PinMuxSet(LPC_IOCON, PIN_2_IOCON,
      (PIN_2_PIO_FUNC | IOCON_MODE_PULLDOWN) );
  Chip_GPIO_SetPinState(LPC_GPIO, PIN_2, false);

  Chip_GPIO_SetPinDIROutput(LPC_GPIO, PIN_13);
  Chip_IOCON_PinMuxSet(LPC_IOCON, PIN_13_IOCON,
      (PIN_13_PIO_FUNC | IOCON_MODE_PULLDOWN) );
  Chip_GPIO_SetPinState(LPC_GPIO, PIN_13, false);

  Chip_GPIO_SetPinDIROutput(LPC_GPIO, PIN_26);
  Chip_IOCON_PinMuxSet(LPC_IOCON, PIN_26_IOCON,
      (PIN_26_PIO_FUNC | IOCON_MODE_PULLDOWN) );
  Chip_GPIO_SetPinState(LPC_GPIO, PIN_26, false);

  Chip_GPIO_SetPinDIROutput(LPC_GPIO, PIN_38);
  Chip_IOCON_PinMuxSet(LPC_IOCON, PIN_38_IOCON,
      (PIN_38_PIO_FUNC | IOCON_MODE_PULLDOWN) );
  Chip_GPIO_SetPinState(LPC_GPIO, PIN_38, false);

  Chip_GPIO_SetPinDIROutput(LPC_GPIO, PIN_11);
  Chip_IOCON_PinMuxSet(LPC_IOCON, PIN_11_IOCON,
      (PIN_11_PIO_FUNC | IOCON_MODE_PULLDOWN) );
  Chip_GPIO_SetPinState(LPC_GPIO, PIN_11, false);

  Chip_GPIO_SetPinDIROutput(LPC_GPIO, PIN_36);
  Chip_IOCON_PinMuxSet(LPC_IOCON, PIN_36_IOCON,
      (PIN_36_PIO_FUNC | IOCON_MODE_PULLDOWN) );
  Chip_GPIO_SetPinState(LPC_GPIO, PIN_36, false);

  Chip_GPIO_SetPinDIROutput(LPC_GPIO, PIN_37);
  Chip_IOCON_PinMuxSet(LPC_IOCON, PIN_33_IOCON,
      (PIN_37_PIO_FUNC | IOCON_MODE_PULLDOWN) );
  Chip_GPIO_SetPinState(LPC_GPIO, PIN_37, false);

  Chip_GPIO_SetPinDIROutput(LPC_GPIO, PIN_43);
  Chip_IOCON_PinMuxSet(LPC_IOCON, PIN_33_IOCON,
      (PIN_43_PIO_FUNC | IOCON_MODE_PULLDOWN) );
  Chip_GPIO_SetPinState(LPC_GPIO, PIN_43, false);

  Chip_GPIO_SetPinDIROutput(LPC_GPIO, PIN_48);
  Chip_IOCON_PinMuxSet(LPC_IOCON, PIN_48_IOCON,
      (PIN_48_PIO_FUNC | IOCON_MODE_PULLDOWN) );
  Chip_GPIO_SetPinState(LPC_GPIO, PIN_48, false);
}

bool Low_Side_Contactor_Pin_Get(void) {
  return Chip_GPIO_GetPinState(LPC_GPIO, CTR_LOW_SWTCH);
}

bool High_Side_Contactor_Pin_Get(void) {
  return Chip_GPIO_GetPinState(LPC_GPIO, CTR_HIGH_SWTCH);
}

void High_Side_Contactor_Pin_Set(bool state) {
  Chip_GPIO_SetPinState(LPC_GPIO, CTR_HIGH_SWTCH, state);
}

bool IMD_Pin_Get(void) {
  return Chip_GPIO_GetPinState(LPC_GPIO, IMD_IN);
}

bool BMS_Pin_Get(void) {
  return Chip_GPIO_GetPinState(LPC_GPIO, BMS_IN);
}

bool Interlock_Pin_Get(void) {
  return Chip_GPIO_GetPinState(LPC_GPIO, INT_IN);
}

void Button1_Pin_Set(bool state) {
  Chip_GPIO_SetPinState(LPC_GPIO, BUT_1, state);
}

void Button2_Pin_Set(bool state) {
  Chip_GPIO_SetPinState(LPC_GPIO, BUT_2, state);
}

void Button3_Pin_Set(bool state) {
  Chip_GPIO_SetPinState(LPC_GPIO, BUT_3, state);
}

void Button4_Pin_Set(bool state) {
  Chip_GPIO_SetPinState(LPC_GPIO, BUT_4, state);
}

void LCD_DB0_Pin_Set(bool state) {
  Chip_GPIO_SetPinState(LPC_GPIO, LCD_DB0, state);
}

void LCD_DB1_Pin_Set(bool state) {
  Chip_GPIO_SetPinState(LPC_GPIO, LCD_DB1, state);
}

void LCD_DB2_Pin_Set(bool state) {
  Chip_GPIO_SetPinState(LPC_GPIO, LCD_DB2, state);
}

void LCD_DB3_Pin_Set(bool state) {
  Chip_GPIO_SetPinState(LPC_GPIO, LCD_DB3, state);
}

void LCD_DB4_Pin_Set(bool state) {
  Chip_GPIO_SetPinState(LPC_GPIO, LCD_DB4, state);
}

void LCD_DB5_Pin_Set(bool state) {
  Chip_GPIO_SetPinState(LPC_GPIO, LCD_DB5, state);
}

void LCD_DB6_Pin_Set(bool state) {
  Chip_GPIO_SetPinState(LPC_GPIO, LCD_DB6, state);
}

void LCD_DB7_Pin_Set(bool state) {
  Chip_GPIO_SetPinState(LPC_GPIO, LCD_DB7, state);
}

void LCD_E_Pin_Set(bool state) {
  Chip_GPIO_SetPinState(LPC_GPIO, LCD_E, state);
}

void LCD_RS_Pin_Set(bool state) {
  Chip_GPIO_SetPinState(LPC_GPIO, LCD_RS, state);
}
