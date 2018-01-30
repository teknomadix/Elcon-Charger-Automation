#include "charge.h"
#include "config.h"

static uint32_t last_time_above_cv_min_curr;
static uint32_t last_init_wait_start_time;
static uint32_t last_init_switch_start_time;

void _set_output(bool close_contactors, bool charger_on, uint32_t charge_voltage_mV, uint32_t charge_current_mA, CSB_OUTPUT_T *output);

void Charge_Init(CSB_STATE_T *state) {
    state->charge_state = CSB_CHARGE_OFF;
    last_time_above_cv_min_curr = 0;
    Charge_Config(state);
}

void Charge_Config(CSB_STATE_T *state) {
    //TODO:power limit stuff?
}

void Charge_Step(CSB_INPUT_T *input, CSB_STATE_T *state, CSB_OUTPUT_T *output) {

    switch (input->mode_request) {
        case CSB_SSM_MODE_CHARGE:
            if (state->charge_state == CSB_CHARGE_OFF
                    || state->charge_state == CSB_CHARGE_BAL) {
                switch (state->pack_config->bms_comm) {
                  case BMS_NO_COMM:
                      state->charge_state = CSB_CHARGE_CLOSE_CNTR;
                      break;
                  case BMS_YES_COMM:
                      state->charge_state = CSB_CHARGE_START_SWITCH;
                      break;
                }
            }
            break;

        case CSB_SSM_MODE_BALANCE:
            if (state->charge_state == CSB_CHARGE_OFF
                    || state->charge_state == CSB_CHARGE_CC
                    || state->charge_state == CSB_CHARGE_CV) {
                switch (state->pack_config->bms_comm) {
                  case BMS_NO_COMM:
                      state->charge_state = CSB_CHARGE_CLOSE_CNTR;
                      break;
                  case BMS_YES_COMM:
                      state->charge_state = CSB_CHARGE_START_SWITCH;
                      break;
                }
            }
            break;

        case CSB_SSM_MODE_IDLE:
            if(state->charge_state == CSB_CHARGE_OFF) {
                state->charge_state = CSB_CHARGE_OFF;
            } else {
                state->charge_state = CSB_CHARGE_DONE;
            }
            break;

        default:
            // no request do nothing
            break;
    }

    switch (state->charge_state) {
        case CSB_CHARGE_OFF:
            _set_output(false, false, 0, 0, output);
            break;
        case CSB_CHARGE_START_SWITCH:
            output->send_bms_config = false;
            state->charge_state = CSB_CHARGE_SWITCH_500;
            state->curr_baud_rate = BMS_CAN_BAUD;
            input->receive_bms_config = false;
            break;
        case CSB_CHARGE_SWITCH_500:
            output->send_bms_config = true;
            state->charge_state = CSB_CHARGE_SEND_500;
            state->curr_baud_rate = BMS_CAN_BAUD;
            input->receive_bms_config = false;
            last_init_switch_start_time = msTicks;
            break;
        case CSB_CHARGE_SEND_500:
            if ( (msTicks - last_init_switch_start_time) > INIT_SEND_TIME_MAX) {
              output->send_bms_config = false;
              state->charge_state = CSB_CHARGE_SWITCH_250;
              state->curr_baud_rate = CSB_CAN_BAUD;
              input->receive_bms_config = false;
            }
            break;
        case CSB_CHARGE_SWITCH_250:
            output->send_bms_config = false;
            state->charge_state = CSB_CHARGE_WAIT_250;
            state->curr_baud_rate = CSB_CAN_BAUD;
            input->receive_bms_config = false;
            last_init_wait_start_time = msTicks;
            break;
        case CSB_CHARGE_WAIT_250:
            if(input->receive_bms_config) {
                output->send_bms_config = false;
                state->charge_state = CSB_CHARGE_CLOSE_CNTR;
                state->curr_baud_rate = CSB_CAN_BAUD;
                input->receive_bms_config = false;
            } else if ( (msTicks - last_init_wait_start_time) > INIT_WAIT_TIME_MAX) {
                output->send_bms_config = false;
                state->charge_state = CSB_CHARGE_SWITCH_500;
                state->curr_baud_rate = BMS_CAN_BAUD;
                input->receive_bms_config = false;
            }
            break;
        case CSB_CHARGE_CLOSE_CNTR:
            if (input->mode_request == CSB_SSM_MODE_CHARGE && input->low_side_cntr_fault == false) { //if in charge mode and there is no fault
                _set_output(true, false, 0, 0, output); //close contactors

                if (input->contactors_closed == true) { //if contactors are closed
                    state->charge_state =
                        (input->pack_status->pack_cell_max_mV < state->pack_config->cell_max_mV) ? CSB_CHARGE_CC : CSB_CHARGE_CV;
                }
            } else if (input->mode_request == CSB_SSM_MODE_BALANCE) {
                _set_output(false, false, 0, 0, output);
                state->charge_state = CSB_CHARGE_BAL;
            }

            if (input->low_side_cntr_fault && !input->elcon_status->elcon_on) {
                _set_output(false, false, 0, 0, output);
                state->charge_state = CSB_CHARGE_FAULT;
            }
            break;
        case CSB_CHARGE_CC:
            if (input->pack_status->pack_cell_max_mV >= state->pack_config->cell_max_mV) {
                state->charge_state = CSB_CHARGE_CV; // Need to go to CV Mode
                _set_output(true, true, state->pack_config->cv_charge_voltage_mV, state->pack_config->cv_charge_current_mA, output);
            } else {
                // Charge in CC Mode
                _set_output(true, true, state->pack_config->cc_charge_voltage_mV, state->pack_config->cc_charge_current_mA, output); //know that this is not immediate
            }

            if (input->low_side_cntr_fault && !input->elcon_status->elcon_on) {
                _set_output(false, false, 0, 0, output);
                state->charge_state = CSB_CHARGE_FAULT;
            } else if (!input->contactors_closed || !input->elcon_status->elcon_charging) {
                _set_output(true, false, 0, 0, output);
                state->charge_state = CSB_CHARGE_CLOSE_CNTR;
            }
            break;
        case CSB_CHARGE_CV:

            if (input->pack_status->pack_cell_max_mV < state->pack_config->cell_max_mV) {
                // Need to go back to CC Mode
                state->charge_state = CSB_CHARGE_CC;
                _set_output(true, true, state->pack_config->cc_charge_voltage_mV, state->pack_config->cc_charge_current_mA, output);
            } else {
                _set_output(true, true, state->pack_config->cv_charge_voltage_mV, state->pack_config->cv_charge_current_mA, output);

                if (input->pack_status->pack_current_mA < state->pack_config->cv_min_current_mA*state->pack_config->pack_cells_p) {
                    //so after your under this threshold for some time you are allowed to be done
                    if ((input->msTicks - last_time_above_cv_min_curr) >= state->pack_config->cv_min_current_ms) {
                        _set_output(false, false, 0, 0, output);
                        state->charge_state = CSB_CHARGE_DONE;
                        break;
                    }
                } else {
                    last_time_above_cv_min_curr = input->msTicks;
                }
            }

            if(!input->contactors_closed) {
                _set_output(true, false, 0, 0, output);
                state->charge_state = CSB_CHARGE_CLOSE_CNTR;
            } else if (input->low_side_cntr_fault && !input->elcon_status->elcon_on) {
                _set_output(false, false, 0, 0, output);
                state->charge_state = CSB_CHARGE_FAULT;
            }
            break;
        case CSB_CHARGE_BAL:
            _set_output(false, false, 0, 0, output);

            // Done balancing
            if (!input->balance_req) {
                state->charge_state = CSB_CHARGE_DONE;
            }

            if(input->contactors_closed) {
                _set_output(false, false, 0, 0, output);
                state->charge_state = CSB_CHARGE_CLOSE_CNTR;
            }

            break;
        case CSB_CHARGE_DONE:
            _set_output(false, false, 0, 0, output);

            // if mode request is idle, that means SSM is trying to switch to another mode so wait for contactors to close
            // if in charge or balance, make sure we don't need to go back to charge or balance
            //    if we do, go back to init
            //    otherwise finish
            if (input->mode_request == CSB_SSM_MODE_IDLE) {
                if (!input->contactors_closed && !input->elcon_status->elcon_charging) {
                    state->charge_state = CSB_CHARGE_OFF;
                }
            } else {
                if(state->curr_mode == CSB_SSM_MODE_CHARGE) {
                    if (input->pack_status->pack_cell_max_mV < state->pack_config->cell_max_mV) {
                        state->charge_state = CSB_CHARGE_CLOSE_CNTR;
                    } else {
                        state->charge_state = CSB_CHARGE_OFF;
                    }
                } else if (state->curr_mode == CSB_SSM_MODE_BALANCE) {
                    if (input->balance_req) {
                        state->charge_state = CSB_CHARGE_CLOSE_CNTR;
                    } else {
                        state->charge_state = CSB_CHARGE_OFF;
                    }
                }
            }
            break;
        case CSB_CHARGE_FAULT:
            _set_output(false, false, 0, 0, output);

            if (!input->low_side_cntr_fault && input->elcon_status->elcon_on) {
                state->charge_state = CSB_CHARGE_CLOSE_CNTR;
            }
            break;
    }
}

void _set_output(bool close_contactors, bool charger_on, uint32_t charge_voltage_mV, uint32_t charge_current_mA, CSB_OUTPUT_T *output) {
    output->close_contactors = close_contactors;
    output->charger_on = charger_on;
    output->voltage_req_mV = charge_voltage_mV;
    output->current_req_mA = charge_current_mA;
}
