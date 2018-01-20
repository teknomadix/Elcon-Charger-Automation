#include <string.h>
#include <stdlib.h>
#include "structs.h"
#include "config.h"
#include "ssm.h"
#include "board.h"

volatile uint32_t msTicks;

// memory allocation for CBS_OUTPUT_T
static bool balance_waitingoff[MAX_NUM_MODULES*MAX_CELLS_PER_MODULE];
static uint32_t balance_timeon[MAX_NUM_MODULES*MAX_CELLS_PER_MODULE];
static CSB_OUTPUT_T csb_output;

// memory allocation for BMS_INPUT_T
static BMS_PACK_STATUS_T pack_status;
static CSB_INPUT_T csb_input;

// memory allocation for BMS_STATE_T
static PACK_CONFIG_T pack_config;
static CSB_STATE_T csb_state;

void Init_Structs(void) {
  csb_output.voltage_req_mV = 0;
  csb_output.current_req_mA = 0;
  csb_output.send_bms_config = false;
  csb_output.close_contactors = false;
  csb_output.charger_on = false;

  csb_state.pack_config = &pack_config;
  csb_state.curr_mode = CSB_SSM_MODE_INIT;
  csb_state.init_state = CSB_INIT_OFF;
  csb_state.charge_state = CSB_CHARGE_OFF;
  csb_state.idle_state = CSB_IDLE_OFF;
  csb_state.balance_waitingoff = balance_waitingoff;
  memset(balance_waitingoff, 0, sizeof(balance_waitingoff[0])*MAX_NUM_MODULES*MAX_CELLS_PER_MODULE);
  csb_state.balance_timeon = balance_timeon;
  memset(balance_timeon, 0, sizeof(balance_timeon[0])*MAX_NUM_MODULES*MAX_CELLS_PER_MODULE);

  pack_config.module_cell_count = 0;
  pack_config.cell_min_mV = 0;
  pack_config.cell_max_mV = 0;
  pack_config.cell_capacity_cAh = 0;
  pack_config.num_modules = 0;
  pack_config.cell_charge_c_rating_cC = 0;
  pack_config.bal_on_thresh_mV = 0;
  pack_config.bal_off_thresh_mV = 0;
  pack_config.pack_cells_p = 0;
  pack_config.cv_min_current_mA = 0;
  pack_config.cv_min_current_ms = 0;
  pack_config.cc_cell_voltage_mV = 0;

  //assign bms_inputs
  csb_input.mode_request = CSB_SSM_MODE_IDLE;
  csb_input.balance_mV = 0; // console request balance to mV
  csb_input.msTicks = msTicks;
  csb_input.elcon_output_voltage = 0;
  csb_input.elcon_output_current = 0;
  csb_input.elcon_has_hardware_failure = false;
  csb_input.elcon_over_temp_protection_on = false;
  csb_input.elcon_is_input_voltage_wrong = false;
  csb_input.elcon_battery_voltage_not_detected = false;
  csb_input.pack_status = &pack_status;
  csb_input.balance_req = false;
  csb_input.contactors_closed = false;
  csb_input.receive_bms_config = false;
  csb_input.charger_on = false;

  pack_status.pack_cell_max_mV = 0;
  pack_status.pack_current_mA = 0;
  pack_status.pack_voltage_mV = 0;
}

void Process_Output(CSB_INPUT_T* csb_input, CSB_OUTPUT_T* csb_output, CSB_STATE_T* csb_state) {
  Board_Contactors_Set(csb_output->close_contactors);
  Board_Can_ProcessOutput(csb_input, csb_state, csb_output);
}

void Process_Input(CSB_INPUT_T* csb_input, CSB_STATE_T* csb_state) {
  Board_Can_ProcessInput(csb_input, csb_state);
  Board_GetModeRequest();
  csb_input->msTicks = msTicks;
  csb_input->contactors_closed = Board_Contactors_Closed();
}

int main(void) {
  Init_Structs();
  Board_Can_Init(BMS_CAN_BAUD);
  Board_UART_Init(UART_BAUD);
  Board_Chip_Init();
  Board_GPIO_Init();

  SSM_Init(&csb_input, &csb_state, &csb_output);

  while(1) {
    Process_Input(&csb_input, &csb_state);
    SSM_Step(&csb_input, &csb_state, &csb_output);
    Process_Output(&csb_input, &csb_output, &csb_state);
  }
}
