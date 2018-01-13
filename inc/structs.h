#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdint.h>
#include <stdbool.h>

extern volatile uint32_t msTicks;

typedef struct BMS_PACK_STATUS {
  uint32_t *cell_voltages_mV; // array size = #modules * cells/module
  int16_t *cell_temperatures_dC; // array size = #modules * thermistors/module
  uint32_t pack_cell_max_mV;
  uint32_t pack_cell_min_mV;
  uint32_t pack_current_mA;
  uint32_t pack_voltage_mV;
  int16_t max_cell_temp_dC;

  int16_t min_cell_temp_dC;
  int16_t avg_cell_temp_dC;
  uint16_t max_cell_temp_position; //range: 0-MAX_NUM_MODULES*MAX_THERMISTORS_PER_MODULE
  uint16_t min_cell_temp_position; //range: 0-MAX_NUM_MODULES*MAX_THERMISTORS_PER_MODULE
} BMS_PACK_STATUS_T;

typedef struct PACK_CONFIG {
    uint32_t cell_min_mV;               // 1
    uint32_t cell_max_mV;
    uint32_t cell_capacity_cAh;
    uint32_t num_modules;
    uint32_t cell_charge_c_rating_cC;   // 5
    uint32_t bal_on_thresh_mV;
    uint32_t bal_off_thresh_mV;
    uint32_t pack_cells_p;
    uint32_t cv_min_current_mA;
    uint32_t cv_min_current_ms;         // 10
    uint32_t cc_cell_voltage_mV;        // 11
    // Size = 4*11 = 44 bytes

    uint32_t cell_discharge_c_rating_cC; // at 27 degrees C
    uint32_t max_cell_temp_dC;
    int16_t min_cell_temp_dC;
    int16_t fan_on_threshold_dC;

    uint8_t module_cell_count;
    // Total Size = 44 + 4 + 4 + 1= 53 bytes (not including fan_on_threshold)

} PACK_CONFIG_T;

typedef enum CSB_SSM_MODE {
    CSB_SSM_MODE_INIT,
    CSB_SSM_MODE_IDLE,
    CSB_SSM_MODE_CHARGE,
    CSB_SSM_MODE_BALANCE,
} CSB_SSM_MODE_T;

static const char * const CSB_SSM_MODE_NAMES[] = {
    "CSB_SSM_MODE_INIT",
    "CSB_SSM_MODE_STANDBY",
    "CSB_SSM_MODE_CHARGE",
    "CSB_SSM_MODE_BALANCE",
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
    CSB_INIT_BMS_CONFIG,
    CSB_INIT_DONE
} CSB_INIT_MODE_T;

static const char * const CSB_INIT_MODE_NAMES[] = {
    "CSB_INIT_OFF",
    "CSB_INIT_BMS_CONFIG",
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
  CSB_SSM_MODE_T mode_request;
  uint32_t balance_mV; // console request balance to mV
  uint32_t msTicks;
  bool receive_bms_config;
  bool balance_req;
  bool contactors_closed;
  bool charger_on;
} CSB_INPUT_T;

typedef struct CSB_STATE {
  PACK_CONFIG_T *pack_config;
  CSB_SSM_MODE_T curr_mode;

  // sub state machine state
  CSB_INIT_MODE_T init_state;
  CSB_CHARGE_MODE_T charge_state;
  CSB_IDLE_MODE_T idle_state;

  uint32_t *balance_timeon;
  bool *balance_waitingoff;
} CSB_STATE_T;

#endif //STRUCTS_H
