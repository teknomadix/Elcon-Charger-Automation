#include "board.h"
#include "CAN_Library.h"
#include "config.h"
#include "charge.h"
#include "pins.h"

#include <stdlib.h>

const uint32_t OscRateIn = 0;

static uint32_t last_csb_elcon_command_time = 0;
static uint32_t last_csb_bms_switch_time = 0;
static uint32_t last_csb_bms_mode_time = 0;

static RINGBUFF_T uart_rx_ring;
static uint8_t _uart_rx_ring[UART_BUFFER_SIZE];
static RINGBUFF_T uart_tx_ring;
static uint8_t _uart_tx_ring[UART_BUFFER_SIZE];

static char str[10];

static bool isResetting = false;

void handle_can_error(Can_ErrorID_T err, uint32_t baud_rate);
void Send_Elcon_Command(CSB_OUTPUT_T *csb_output);
void Send_Bms_Switch(void);
void Send_Bms_Mode(CSB_STATE_T *csb_state);

void UART_IRQHandler(void) {
  Chip_UART_IRQRBHandler(LPC_USART, &uart_rx_ring, &uart_tx_ring);
}

void SysTick_Handler(void) {
  msTicks++;
}

void Board_Chip_Init(void) {
  SysTick_Config(Hertz2Ticks(1000));
}

uint32_t Board_Print(const char *str) {
  return Chip_UART_SendRB(LPC_USART, &uart_tx_ring, str, strlen(str));
}

void Board_BlockingDelay(uint32_t dlyTicks) {
  uint32_t curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
}

uint32_t Board_Println(const char *str) {
  uint32_t count = Board_Print(str);
  return count + Board_Print("\r\n");
}

uint32_t Board_PrintNum(uint32_t a, uint8_t base) {
  itoa(a, str, base);
  return Board_Print(str);
}

uint32_t Board_Write(const char *str, uint32_t count) {
  return Chip_UART_SendRB(LPC_USART, &uart_tx_ring, str, count);
}

uint32_t Board_Read(char *charBuffer, uint32_t length) {
  return Chip_UART_ReadRB(LPC_USART, &uart_rx_ring, charBuffer, length);
}

// USE THESE SPARINGLY. ONLY WHEN A PRINT WOULD RESULT IN A BUFFER OVERFLOW
uint32_t Board_Print_BLOCKING(const char *str) {
  return Chip_UART_SendBlocking(LPC_USART, str, strlen(str));
}

uint32_t Board_Println_BLOCKING(const char *str) {
  uint32_t count = Board_Print_BLOCKING(str);
  return count + Board_Print_BLOCKING("\r\n");
}

void Board_Can_Init(uint32_t baud_rate) {
  Can0_Csb_Init(baud_rate);
}

void Board_UART_Init(uint32_t baudRateHz) {
  // Initialize UART Buffers
  RingBuffer_Init(&uart_rx_ring, _uart_rx_ring, sizeof(uint8_t), UART_BUFFER_SIZE);
  RingBuffer_Flush(&uart_rx_ring);
  RingBuffer_Init(&uart_tx_ring, _uart_tx_ring, sizeof(uint8_t), UART_BUFFER_SIZE);
  RingBuffer_Flush(&uart_tx_ring);

  Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_6, (IOCON_FUNC1 | IOCON_MODE_INACT));/* RXD */
  Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_7, (IOCON_FUNC1 | IOCON_MODE_INACT));/* TXD */

  Chip_UART_Init(LPC_USART);
  Chip_UART_SetBaudFDR(LPC_USART, baudRateHz);
  Chip_UART_ConfigData(LPC_USART, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT | UART_LCR_PARITY_DIS));
  Chip_UART_SetupFIFOS(LPC_USART, (UART_FCR_FIFO_EN | UART_FCR_TRG_LEV2));
  Chip_UART_TXEnable(LPC_USART);

  Chip_UART_IntEnable(LPC_USART, UART_IER_RBRINT);
  NVIC_ClearPendingIRQ(UART0_IRQn);
  NVIC_EnableIRQ(UART0_IRQn);
}

void Board_Can_ProcessInput(CSB_INPUT_T *csb_input, CSB_STATE_T *csb_state){
  Can_MsgID_T msgType = Can_MsgType();

  if (msgType == Can_No_Msg) {
      // No message, so do nothing this round
      return;
  } else if (msgType == Can_Error_Msg) {
      Can_ErrorID_T err = Can_Error_Read();
      handle_can_error(err, csb_state->curr_baud_rate);
      return;
  }

  // We had a successful reception, so finish reset (if it's still happening)
  isResetting = false;

  if (msgType == Can_Unknown_Msg) {
      Frame f;
      Can_Unknown_Read(&f);
  } else if (msgType == Can_ElconHeartbeat_Msg) {
      Can_ElconHeartbeat_T msg;
      Can_ElconHeartbeat_Read(&msg);
      csb_input->elcon_output_voltage = msg.output_voltage;
      csb_input->elcon_output_current = msg.output_current;
      csb_input->elcon_has_hardware_failure = msg.has_hardware_failure;
      csb_input->elcon_over_temp_protection_on = msg.over_temp_protection_on;
      csb_input->elcon_is_input_voltage_wrong = msg.is_input_voltage_wrong;
      csb_input->elcon_battery_voltage_not_detected = msg.battery_voltage_not_detected;
      csb_input->elcon_is_comms_bad = msg.elcon_is_comms_bad;
      //check this, maybe some logic from above signals
      csb_input->charger_on = true;
  } else if (msgType == Can_BmsPackStatus_Msg) {
      Can_BmsPackStatus_T msg;
      Can_BmsPackStatus_Read(&msg);
      csb_input->pack_status->pack_cell_max_mV = msg.max_cell_voltage;
      csb_input->pack_status->pack_current_mA = msg.pack_current;
      csb_input->pack_status->pack_voltage_mV= msg.pack_voltage;
      csb_input->balance_req = msg.balancing_needed;
      //check this maybe gotta check if the above read correctly from CAN 2
      csb_input->receive_bms_config = true;
  } else {
      // note other errors
  }
}

void Board_Can_ProcessOutput(CSB_INPUT_T *csb_input, CSB_STATE_T *csb_state, CSB_OUTPUT_T *csb_output) {
  uint32_t msTicks = csb_input->msTicks;
  if ((csb_state->curr_mode == CSB_SSM_MODE_INIT) && (csb_state->init_state == CSB_INIT_SWITCH_500)){
      Board_Can_Init(BMS_CAN_BAUD);
  }
  if ((csb_state->curr_mode == CSB_SSM_MODE_INIT) && (csb_state->init_state == CSB_INIT_SWITCH_250)){
      Board_Can_Init(CSB_CAN_BAUD);
  }
  if ( ((msTicks - last_csb_elcon_command_time) > CSB_ELCON_COMMAND_PERIOD) && (csb_state->curr_mode != CSB_SSM_MODE_INIT)){
      last_csb_elcon_command_time = msTicks;
      Send_Elcon_Command(csb_output);
  }
  if ( ((msTicks - last_csb_bms_switch_time) > CSB_BMS_CSB_SWITCH_PERIOD) && (csb_output->send_bms_config == true)) {
      last_csb_bms_switch_time = msTicks;
      Send_Bms_Switch();
  }
  if ( (msTicks - last_csb_bms_mode_time) > CSB_BMS_MODE_PERIOD) {
      last_csb_bms_mode_time = msTicks;
      Send_Bms_Mode(csb_state);
  }
}

void Send_Elcon_Command(CSB_OUTPUT_T *csb_output) {
  Can_ElconCommand_T elcon_command;
  elcon_command.max_terminal_voltage = csb_output->voltage_req_mV;
  elcon_command.max_current = csb_output->current_req_mA;
  elcon_command.control = csb_output->charger_on;

  handle_can_error(Can_ElconCommand_Write(&elcon_command), CSB_CAN_BAUD);
}

void Send_Bms_Switch(void) {
  Can_BMS_CSB_Switch_T switch_msg;
  switch_msg.always_true = true;

  handle_can_error(Can_BMS_CSB_Switch_Write(&switch_msg), BMS_CAN_BAUD);
}

void Send_Bms_Mode(CSB_STATE_T *csb_state) {
  Can_BMS_Mode_T mode_msg;

  switch (csb_state->curr_mode) {
    case CSB_SSM_MODE_CHARGE:
      mode_msg.requested_mode = ____BMS_MODE__REQUESTED_MODE__CHARGE;
      break;
    case CSB_SSM_MODE_BALANCE:
      mode_msg.requested_mode = ____BMS_MODE__REQUESTED_MODE__BALANCE;
      break;
    case CSB_SSM_MODE_INIT:
    case CSB_SSM_MODE_IDLE:
    default:
      mode_msg.requested_mode = ____BMS_MODE__REQUESTED_MODE__IDLE;
      break;
  }

  handle_can_error(Can_BMS_Mode_Write(&mode_msg), CSB_CAN_BAUD);
}

bool Board_Contactors_Closed(void) {
  return Low_Side_Contactor_Pin_Get();
}

void Board_GetModeRequest(void) {
  //balance_mV, mode_request for csb_inputs
}

void Board_Contactors_Set(bool close_contactors) {
  High_Side_Contactor_Pin_Set(close_contactors);
}

void Board_Check_Faults(CSB_INPUT_T *csb_input) {
  csb_input->imd_fault = IMD_Pin_Get();
  csb_input->int_fault = Interlock_Pin_Get();
  csb_input->bms_fault = BMS_Pin_Get();
  csb_input->low_side_cntr_fault = Low_Side_Contactor_Pin_Get();
}

void handle_can_error(Can_ErrorID_T err, uint32_t baud_rate) {
    if (err == Can_Error_NONE || err == Can_Error_NO_RX) {
        // Neither of these are real errors
        isResetting = false;
        return;
    }
    if (!isResetting) {
        // We have an error, and should start a reset.
        // TODO change behavior depending on error type.
        isResetting = true;
        CAN_ResetPeripheral();
        Board_Can_Init(baud_rate);
        UNUSED(err);

    }
}
