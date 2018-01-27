#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdint.h>
#include <stdbool.h>
#include "CAN_Library.h"

extern volatile uint32_t msTicks;

typedef struct BMS_PACK_STATUS {
  uint32_t pack_cell_max_mV;
  uint32_t pack_current_mA;
  uint32_t pack_voltage_mV;
} BMS_PACK_STATUS_T;

typedef struct PACK_CONFIG {
    uint32_t cell_min_mV;
    uint32_t cell_max_mV;
    uint32_t cell_capacity_cAh;
    uint32_t num_modules;
    uint32_t cell_charge_c_rating_cC;
    uint32_t bal_on_thresh_mV;
    uint32_t bal_off_thresh_mV;
    uint32_t pack_cells_p;
    uint32_t cv_min_current_mA;
    uint32_t cv_min_current_ms;
    uint32_t cc_cell_voltage_mV;
    uint32_t module_cell_count;

} PACK_CONFIG_T;

typedef struct ELCON_STATUS {
    uint16_t elcon_output_voltage;
    uint16_t elcon_output_current;
    bool elcon_has_hardware_failure;
    bool elcon_over_temp_protection_on;
    bool elcon_is_input_voltage_wrong;
    bool elcon_battery_voltage_not_detected;
    bool elcon_is_comms_bad;
    bool elcon_on;
    bool elcon_charging;
} ELCON_STATUS_T;

typedef enum CSB_SSM_MODE {
    CSB_SSM_MODE_INIT,
    CSB_SSM_MODE_IDLE,
    CSB_SSM_MODE_CHARGE,
    CSB_SSM_MODE_BALANCE
} CSB_SSM_MODE_T;

static const char * const CSB_SSM_MODE_NAMES[] = {
    "CSB_SSM_MODE_INIT",
    "CSB_SSM_MODE_IDLE",
    "CSB_SSM_MODE_CHARGE",
    "CSB_SSM_MODE_BALANCE"
};

typedef enum {
    CSB_CHARGE_OFF,
    CSB_CHARGE_INIT,
    CSB_CHARGE_CC,
    CSB_CHARGE_CV,
    CSB_CHARGE_BAL,
    CSB_CHARGE_FAULT,
    CSB_CHARGE_DONE
} CSB_CHARGE_MODE_T;

static const char * const CSB_CHARGE_MODE_NAMES[] = {
    "CSB_CHARGE_OFF",
    "CSB_CHARGE_INIT",
    "CSB_CHARGE_CC",
    "CSB_CHARGE_CV",
    "CSB_CHARGE_BAL",
    "CSB_CHARGE_FAULT",
    "CSB_CHARGE_DONE"
};

typedef enum {
    CSB_INIT_OFF,
    CSB_INIT_SWITCH_500,
    CSB_INIT_SEND_500,
    CSB_INIT_SWITCH_250,
    CSB_INIT_WAIT_250,
    CSB_INIT_DONE
} CSB_INIT_MODE_T;

static const char * const CSB_INIT_MODE_NAMES[] = {
    "CSB_INIT_OFF",
    "CSB_INIT_SWITCH_500",
    "CSB_INIT_SEND_500",
    "CSB_INIT_SWITCH_250",
    "CSB_INIT_WAIT_250",
    "CSB_INIT_DONE"
};

typedef enum {
  CSB_IDLE_OFF,
  CSB_IDLE_ON
} CSB_IDLE_MODE_T;

static const char * const CSB_IDLE_MODE_NAMES[] = {
    "CSB_IDLE_OFF",
    "CSB_IDLE_ON"
};

typedef struct CSB_OUTPUT {
  uint32_t voltage_req_mV;
  uint32_t current_req_mA;
  bool send_bms_config;
  bool close_contactors;
  bool charger_on;
} CSB_OUTPUT_T;

typedef struct CSB_INPUT {
  BMS_PACK_STATUS_T *pack_status;
  ELCON_STATUS_T *elcon_status;
  CSB_SSM_MODE_T mode_request;
  uint32_t balance_mV; // console request balance to mV
  uint32_t msTicks;
  bool receive_bms_config;
  bool balance_req;
  bool contactors_closed;
  bool imd_fault;
  bool int_fault;
  bool bms_fault;
  bool low_side_cntr_fault;
  Can_BMSErrorsID_T bms_error;
} CSB_INPUT_T;

typedef struct CSB_STATE {
  PACK_CONFIG_T *pack_config;
  CSB_SSM_MODE_T curr_mode;

  // sub state machine state
  CSB_INIT_MODE_T init_state;
  CSB_CHARGE_MODE_T charge_state;
  CSB_IDLE_MODE_T idle_state;

  uint32_t curr_baud_rate;
  uint32_t *balance_timeon;
  bool *balance_waitingoff;
} CSB_STATE_T;

#endif //STRUCTS_H
