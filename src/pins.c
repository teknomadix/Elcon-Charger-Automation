#include "pins.h"

#include "chip.h"

// Low Side Contactor/Precharge
#define CTR_LOW_SWTCH 2,1
#define IOCON_CTR_LOW_SWTCH IOCON_PIO2_1

// Contactors Closed
#define CONTACTORS_CLOSED 0, 3 
#define IOCON_CTR_HIGH_SWTCH IOCON_PIO0_3

// IMD input
#define IMD_IN 2, 6
#define IOCON_IMD_IN IOCON_PIO2_6

// BMS input
#define BMS_IN 2, 8
#define IOCON_BMS_IN IOCON_PIO2_8

// Interlock input
#define INT_IN 0, 2
#define IOCON_INT_IN IOCON_PIO0_2

//Charge Enable (from BMS)
#define CHARGE_ENABLE 1,8
#define IOCON_CHARGE_ENABLE_IN IOCON_PIO1_8

//LED 1
#define LED1 0,3
#define IOCON_LED1_OUT IOCON_PIO0_3

//LED 2
#define LED2 1, 11
#define IOCON_LED2_OUT IOCONPIO1_11

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

  //Charge Enable Input 
  Chip_GPIO_SetPinDIROutput(LPC_GPIO,CHARGE_ENABLE);
  Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_CHARGE_ENABLE_IN, (IOCON_FUN0|IOCON_MODE_INACT));

  //LED1 
  Chip_GPIO_SetPinDIROutput(LPC_GPIO,LED1);
  Chip_IOCON_PinMuxSet(LPC_IOCON,IOCON_LED1_OUT,(IOCON_FUNC1 | IOCON_MODE_INACT));

  //LED 2
  Chip_GPIO_SetPinDIROutput(LPC_GPIO,LED2);
  Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_LED2_OUT, (IOCON_FUN1 | IOCON_MODE_INACE)); 

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

bool Contactors_Closed_Pin_Get(void) {
  return Chip_GPIO_GetPinState(LPC_GPIO,CONTACTORS_CLOSED);
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

bool Charge_Enable_Get(void){
       return Chip_GPIO_GetPinState(LPC_GPIO,CHARGE_ENABLE);
}

void LED1_Set (bool state){
	Chip_GPIO_SetPinState(LPC_GPIO,LED1,state);
}

void LED2_Set (bool state){
	Chip_GPIO_SetPinState(LPC_GPIO,LED2,state);
}

