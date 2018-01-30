#ifndef _CONSOLE_TYPES_H
#define _CONSOLE_TYPES_H

typedef enum {
    C_GET,
    C_SET,
    C_HELP,
    C_CONFIG_DEF,
    C_BAL,
    C_CHRG,
    C_MEASURE,
    NUMCOMMANDS
} command_label_t;

typedef enum {
    RWL_cell_min_mV,
    RWL_cell_max_mV,
    RWL_cell_capacity_cAh,
    RWL_num_modules,
    RWL_module_cell_count, //need to think through how this will work
    RWL_cell_charge_c_rating_cC,
    RWL_bal_on_thresh_mV,
    RWL_bal_off_thresh_mV,
    RWL_pack_cells_p,
    RWL_cv_min_current_mA,
    RWL_cv_min_current_ms,
    RWL_cc_cell_voltage_mV,
    RWL_cc_charge_voltage_mV,
    RWL_cc_charge_current_mA,
    RWL_cv_charge_voltage_mV,
    RWL_cv_charge_current_mA,
    RWL_bms_comm,
    RWL_LENGTH
} rw_loc_label_t;

#define ROL_FIRST RWL_LENGTH

typedef enum {
    ROL_state = (int)ROL_FIRST,
    ROL_total_num_cells,
    ROL_pack_name,
    ROL_pack_cell_max_mV,
    ROL_pack_current_mA,
    ROL_pack_voltage_mV,
    ROL_fault,
    ROL_LENGTH
} ro_loc_label_t;

typedef enum {
    CONL_MY18,
    CONL_MY16,
    CONL_LENGTH
} config_loc_label_t;

typedef enum {
    YNL_YES,
    YNL_NO,
    YNL_LENGTH
} yn_loc_label_t;

#endif
