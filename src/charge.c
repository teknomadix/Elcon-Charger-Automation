#include "charge.h"

static uint16_t total_num_cells;
static uint32_t cc_charge_voltage_mV;
static uint32_t cc_charge_current_mA;
static uint32_t cv_charge_voltage_mV;
static uint32_t cv_charge_current_mA;
static uint32_t last_time_above_cv_min_curr;

void _set_output(bool close_contactors, bool charger_on, uint32_t charge_voltage_mV, uint32_t charge_current_mA, CSB_OUTPUT_T *output);

void Charge_Init(CSB_STATE_T *state, PACK_CONFIG_T *pack_config) {
    state->charge_state = CSB_CHARGE_OFF;
    last_time_above_cv_min_curr = 0;
    Charge_Config(pack_config);
}

void Charge_Config(PACK_CONFIG_T *pack_config) {
    total_num_cells = pack_config->num_modules * pack_config->module_cell_count;

    cc_charge_voltage_mV = pack_config->cc_cell_voltage_mV * total_num_cells;
    cc_charge_current_mA = pack_config->cell_capacity_cAh * pack_config->cell_charge_c_rating_cC * pack_config->pack_cells_p / 10;

    cv_charge_voltage_mV = pack_config->cell_max_mV * total_num_cells;
    cv_charge_current_mA = cc_charge_current_mA;
}

void Charge_Step(CSB_INPUT_T *input, CSB_STATE_T *state, CSB_OUTPUT_T *output) {

    switch (input->mode_request) {
        case CSB_SSM_MODE_CHARGE:
            if (state->charge_state == CSB_CHARGE_OFF
                    || state->charge_state == CSB_CHARGE_BAL) {
                state->charge_state = CSB_CHARGE_INIT;
            }
            break;

        case CSB_SSM_MODE_BALANCE:
            if (state->charge_state == CSB_CHARGE_OFF
                    || state->charge_state == CSB_CHARGE_CC
                    || state->charge_state == CSB_CHARGE_CV) {
                state->charge_state = CSB_CHARGE_INIT;
            }
            break;

        case CSB_SSM_MODE_IDLE:
            if(state->charge_state == CSB_CHARGE_OFF) {
                state->charge_state = CSB_CHARGE_OFF;
            } else {
                state->charge_state = CSB_CHARGE_DONE;
            }
            break;

        case CSB_SSM_MODE_INIT:
        case CSB_SSM_MODE_NULL:
            // no request do nothing
            break;
    }

    switch (state->charge_state) {
        case CSB_CHARGE_OFF:
            _set_output(false, false, 0, 0, output);
            break;
        case CSB_CHARGE_INIT:
            if (input->mode_request == CSB_SSM_MODE_CHARGE && input->low_side_cntr_fault == false) { //if in charge mode and there is no fault
              _set_output(true, false, 0, 0, output); //close contactors

              if (input->contactors_closed == true) { //if contactors are closed
                  if(input->mode_request == CSB_SSM_MODE_CHARGE) {
                      state->charge_state =
                          (input->pack_status->pack_cell_max_mV < state->pack_config->cell_max_mV) ? CSB_CHARGE_CC : CSB_CHARGE_CV;
                  } else if (input->mode_request == CSB_SSM_MODE_BALANCE) {
                      state->charge_state = CSB_CHARGE_BAL;
                  }
              }
            }

            if (input->low_side_cntr_fault) {
              state->charge_state = CSB_CHARGE_FAULT;
            }
            break;
        case CSB_CHARGE_CC:
            if (input->pack_status->pack_cell_max_mV >= state->pack_config->cell_max_mV) {
                state->charge_state = CSB_CHARGE_CV; // Need to go to CV Mode
                _set_output(true, true, cv_charge_voltage_mV, cv_charge_current_mA, output);
            } else {
                // Charge in CC Mode
                _set_output(true, true, cc_charge_voltage_mV, cc_charge_current_mA, output); //know that this is not immediate
            }

            if (input->low_side_cntr_fault) {
              state->charge_state = CSB_CHARGE_FAULT;
            } else if (!input->contactors_closed || !input->charger_on) {
                _set_output(true, false, 0, 0, output);
                state->charge_state = CSB_CHARGE_INIT;
            }
            break;
        case CSB_CHARGE_CV:

            if (input->pack_status->pack_cell_max_mV < state->pack_config->cell_max_mV) {
                // Need to go back to CC Mode
                state->charge_state = CSB_CHARGE_CC;
                _set_output(true, true, cc_charge_voltage_mV, cc_charge_current_mA, output);
            } else {
                _set_output(true, true, cv_charge_voltage_mV, cv_charge_current_mA, output);

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
                state->charge_state = CSB_CHARGE_INIT;
            } else if (input->low_side_cntr_fault) {
              state->charge_state = CSB_CHARGE_FAULT;
            }
            break;
        case CSB_CHARGE_BAL:
            //TODO: figure this out bc wtf
            _set_output(false, false, 0, 0, output);
            bool balancing = input->balance_req;

            // Done balancing
            if (!balancing) {
                state->charge_state = CSB_CHARGE_DONE;
            }

            if(!input->contactors_closed) {
                _set_output(false, false, 0, 0, output);
                state->charge_state = CSB_CHARGE_INIT;
            } else if (input->low_side_cntr_fault) {
              state->charge_state = CSB_CHARGE_FAULT;
            }

            break;
        case CSB_CHARGE_DONE:
            _set_output(false, false, 0, 0, output);

            // if mode request is idle, that means SSM is trying to switch to another mode so wait for contactors to close
            // if in charge or balance, make sure we don't need to go back to charge or balance
            //    if we do, go back to init
            //    otherwise finish
            if (input->mode_request == CSB_SSM_MODE_IDLE) {
                if (!input->contactors_closed && !input->charger_on) {
                    state->charge_state = CSB_CHARGE_OFF;
                }
            } else {
                if(state->curr_mode == CSB_SSM_MODE_CHARGE) {
                    if (input->pack_status->pack_cell_max_mV < state->pack_config->cell_max_mV) {
                        state->charge_state = CSB_CHARGE_INIT;
                    } else {
                        state->charge_state = CSB_CHARGE_OFF;
                    }
                } else if (state->curr_mode == CSB_SSM_MODE_BALANCE) {
                    //TODO: idk think abt this, but lets just stop
                    if (!input->contactors_closed && !input->charger_on) {
                        state->charge_state = CSB_CHARGE_OFF;
                    }
                }
            }
            break;
        case CSB_CHARGE_FAULT:
            _set_output(false, false, 0, 0, output);
            if (input->low_side_cntr_fault) {
                state->charge_state = CSB_CHARGE_INIT;
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
