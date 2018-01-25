#include "ssm.h"
#include "config.h"
#include "charge.h"
#include "board.h"

volatile uint32_t msTicks;

static uint32_t last_init_wait_start_time;
static uint32_t last_init_switch_start_time;

void SSM_Init(CSB_STATE_T *state, PACK_CONFIG_T *pack_config) {
  // Initialize BMS state variables
  state->curr_mode = CSB_SSM_MODE_INIT;
  state->init_state = CSB_INIT_OFF;
  state->charge_state = CSB_CHARGE_OFF;
  state->idle_state = CSB_IDLE_OFF;

  Charge_Init(state, pack_config);
}

void Init_Step(CSB_INPUT_T *input, CSB_STATE_T *state, CSB_OUTPUT_T *output) {
  switch(state->init_state) {
      case CSB_INIT_OFF:
          output->send_bms_config = false;
          state->init_state = CSB_INIT_SWITCH_500;
          state->curr_baud_rate = BMS_CAN_BAUD;
          input->receive_bms_config = false;
          break;
      case CSB_INIT_SWITCH_500:
          output->send_bms_config = true;
          state->init_state = CSB_INIT_SEND_500;
          state->curr_baud_rate = BMS_CAN_BAUD;
          input->receive_bms_config = false;
          last_init_switch_start_time = msTicks;
          break;
      case CSB_INIT_SEND_500:
          if ( (msTicks - last_init_switch_start_time) > INIT_SEND_TIME_MAX) {
            output->send_bms_config = false;
            state->init_state = CSB_INIT_SWITCH_250;
            state->curr_baud_rate = CSB_CAN_BAUD;
            input->receive_bms_config = false;
          }
          break;
      case CSB_INIT_SWITCH_250:
          output->send_bms_config = false;
          state->init_state = CSB_INIT_WAIT_250;
          state->curr_baud_rate = CSB_CAN_BAUD;
          input->receive_bms_config = false;
          last_init_wait_start_time = msTicks;
          break;
      case CSB_INIT_WAIT_250:
          if(input->receive_bms_config) {
              output->send_bms_config = false;
              state->init_state = CSB_INIT_DONE;
              state->curr_baud_rate = CSB_CAN_BAUD;
              input->receive_bms_config = false;
          } else if ( (msTicks - last_init_wait_start_time) > INIT_WAIT_TIME_MAX) {
              output->send_bms_config = false;
              state->init_state = CSB_INIT_SWITCH_500;
              state->curr_baud_rate = BMS_CAN_BAUD;
              input->receive_bms_config = false;
          }
          break;
      case(CSB_INIT_DONE):
          state->curr_mode = CSB_SSM_MODE_IDLE;
          state->init_state = CSB_INIT_OFF;
          state->curr_baud_rate = CSB_CAN_BAUD;
          break;
  }
}

bool Is_Valid_Jump(CSB_SSM_MODE_T mode1, CSB_SSM_MODE_T mode2) {

    // no requested
    if(mode2 == CSB_SSM_MODE_NULL) {
      return false;
    }

    // idle switch
    if(mode1 == CSB_SSM_MODE_IDLE && mode2 == CSB_SSM_MODE_CHARGE) {
        return true;
    } else if(mode1 == CSB_SSM_MODE_IDLE && mode2 == CSB_SSM_MODE_BALANCE) {
        return true;

    } else if(mode1 == CSB_SSM_MODE_CHARGE && mode2 == CSB_SSM_MODE_IDLE) {
        return true;
    } else if(mode1 == CSB_SSM_MODE_BALANCE && mode2 == CSB_SSM_MODE_IDLE) {
        return true;

    // charge and balance switch
    } else if(mode1 == CSB_SSM_MODE_BALANCE && mode2 == CSB_SSM_MODE_CHARGE) {
        return true;
    } else if(mode1 == CSB_SSM_MODE_CHARGE && mode2 == CSB_SSM_MODE_BALANCE) {
        return true;
    }
    return false;
}

bool Is_State_Done(CSB_STATE_T *state) {
    switch(state->curr_mode) {
        case CSB_SSM_MODE_CHARGE:
        case CSB_SSM_MODE_BALANCE:
            return state->charge_state == CSB_CHARGE_OFF;
        case CSB_SSM_MODE_INIT:
            return state->init_state == CSB_INIT_DONE;
        case CSB_SSM_MODE_IDLE:
            return true;
    }
    return false;
}

void SSM_Step(CSB_INPUT_T *input, CSB_STATE_T *state, CSB_OUTPUT_T *output) {
    // OUTLINE:
    // If change state request made and possible, change state
    // Check if in idle:
    //   if in idle:
    //        if mode request change valid, switch over
    //   else dispatch step to appropriate SM step

    if(Is_Valid_Jump(state->curr_mode, input->mode_request) && Is_State_Done(state)) {
        state->curr_mode = input->mode_request;
    }

    switch(state->curr_mode) {
        case CSB_SSM_MODE_IDLE:
            break;
        case CSB_SSM_MODE_INIT:
            Init_Step(input, state, output);
            break;
        case CSB_SSM_MODE_CHARGE:
            Charge_Step(input, state, output);
            break;
        case CSB_SSM_MODE_BALANCE:
            Charge_Step(input, state, output);
            break;
    }
}
