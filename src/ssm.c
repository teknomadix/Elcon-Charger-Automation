#include "ssm.h"

volatile uint32_t msTicks;

void SSM_Init(CSB_INPUT_T *input, CSB_STATE_T *state, CSB_OUTPUT_T *output) {
  // Initialize BMS state variables
  state->curr_mode = CSB_SSM_MODE_INIT;
  state->init_state = CSB_INIT_OFF;

  input->pack_status->max_cell_temp_dC = 0;

  Charge_Init(state);
}

void Init_Step(CSB_INPUT_T *input, CSB_STATE_T *state, CSB_OUTPUT_T *output) {
  switch(state->init_state) {
      case(CSB_INIT_OFF):
          output->send_bms_config = true;
          state->init_state = CSB_INIT_BMS_CONFIG;
          input->receive_bms_config = false;
          break;
      case(CSB_INIT_BMS_CONFIG):
          if(input->receive_bms_config) {
              output->send_bms_config = false;
              state->init_state = CSB_INIT_DONE;
              state->curr_mode = CSB_SSM_MODE_IDLE; //but why?
              input->receive_bms_config = false;
          }
          break;
      case(CSB_INIT_DONE):
          state->curr_mode = CSB_SSM_MODE_IDLE;
          break;
  }
}

bool Is_Valid_Jump(CSB_SSM_MODE_T mode1, CSB_SSM_MODE_T mode2) {

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

static void Check_Error(CSB_INPUT_T *input, CSB_STATE_T *state, CSB_OUTPUT_T *output) {
    (void)(output);

}


void SSM_Step(CSB_INPUT_T *input, CSB_STATE_T *state, CSB_OUTPUT_T *output) {
    // OUTLINE:
    // If change state request made and possible, change state
    // Check if in idle:
    //   if in idle:
    //        if mode request change valid, switch over
    //   else dispatch step to appropriate SM step
    Check_Error(input, state, output);

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
