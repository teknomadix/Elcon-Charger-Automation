#include "pack_config.h"
#include "config.h"

void MY18_Pack_Config(CSB_STATE_T* csb_state) {
  csb_state->pack_config->module_cell_count = MY18_MODULE_CELL_COUNT;
  csb_state->pack_config->cell_min_mV = MY18_CELL_MIN_mV;
  csb_state->pack_config->cell_max_mV = MY18_CELL_MAX_mV;
  csb_state->pack_config->cell_capacity_cAh = MY18_CELL_CAPACITY_cAh;
  csb_state->pack_config->num_modules = MY18_NUM_MODULES;
  csb_state->pack_config->cell_charge_c_rating_cC = MY18_CELL_CHARGE_C_RATING_cC;
  csb_state->pack_config->bal_on_thresh_mV = MY18_BALANCE_ON_THRESHOLD_mV;
  csb_state->pack_config->bal_off_thresh_mV = MY18_BALANCE_OFF_THRESHOLD_mV;
  csb_state->pack_config->pack_cells_p = MY18_PACK_CELLS_PARALLEL;
  csb_state->pack_config->cv_min_current_mA = MY18_CV_MIN_CURRENT_mA;
  csb_state->pack_config->cv_min_current_ms = MY18_CV_MIN_CURRENT_ms;
  csb_state->pack_config->cc_cell_voltage_mV = MY18_CC_CELL_VOLTAGE_mV;
}

void MY16_Pack_Config(CSB_STATE_T* csb_state) {
  csb_state->pack_config->module_cell_count = MY16_MODULE_CELL_COUNT;
  csb_state->pack_config->cell_min_mV = MY16_CELL_MIN_mV;
  csb_state->pack_config->cell_max_mV = MY16_CELL_MAX_mV;
  csb_state->pack_config->cell_capacity_cAh = MY16_CELL_CAPACITY_cAh;
  csb_state->pack_config->num_modules = MY16_NUM_MODULES;
  csb_state->pack_config->cell_charge_c_rating_cC = MY16_CELL_CHARGE_C_RATING_cC;
  csb_state->pack_config->bal_on_thresh_mV = MY16_BALANCE_ON_THRESHOLD_mV;
  csb_state->pack_config->bal_off_thresh_mV = MY16_BALANCE_OFF_THRESHOLD_mV;
  csb_state->pack_config->pack_cells_p = MY16_PACK_CELLS_PARALLEL;
  csb_state->pack_config->cv_min_current_mA = MY16_CV_MIN_CURRENT_mA;
  csb_state->pack_config->cv_min_current_ms = MY16_CV_MIN_CURRENT_ms;
  csb_state->pack_config->cc_cell_voltage_mV = MY16_CC_CELL_VOLTAGE_mV;
}
