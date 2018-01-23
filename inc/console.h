#include "microrl.h"
#include "structs.h"
#include "console_types.h"

#ifndef _CONSOLE_H
#define _CONSOLE_H

static const char * const commands[] = { "get",
                            "set",
                            "help",
                            "config_def",
                            "bal",
                            "chrg",
                            "measure"
                                    };

static const char nargs[7] = {  1 ,
                        2 ,
                        1 ,
                        1 ,
                        1 ,
                        0 ,
                        1};

static const char * const helpstring[NUMCOMMANDS] =  {"get",
                            "set",
                            "help",
                            "config_def",
                            "bal",
                            "chrg",
                            "measure"
                                    };

static const char * const locstring[] =  {
                            "cell_min_mV",
                            "cell_max_mV",
                            "cell_capacity_cAh",
                            "num_modules",
                            "module_cell_count",
                            "cell_charge_c_rating_cC",
                            "bal_on_thresh_mV",
                            "bal_off_thresh_mV",
                            "pack_cells_p",
                            "cv_min_current_mA",
                            "cv_min_current_ms",
                            "cc_cell_voltage_mV",
                            "state",
                            "pack_cell_max_mV",
                            "pack_current_mA",
                            "pack_voltage_mV",
                            "fault"
};

static const char * const config_locstring[] =  {
                            "MY18",
                            "MY16"
};

typedef void (* const EXECUTE_HANDLER)(const char * const *);

typedef struct {
    bool valid_mode_request;
    bool config_default;
    CSB_SSM_MODE_T mode_request;
    uint32_t balance_mV;
    bool measure_on;
    bool measure_packcurrent;
    bool measure_packvoltage;
} CONSOLE_OUTPUT_T;

void console_init(CSB_INPUT_T *csb_input, CSB_STATE_T *csb_state, CONSOLE_OUTPUT_T *console_output);
void executerl(int32_t argc, const char * const * argv);
void Output_Measurements(CONSOLE_OUTPUT_T *console_output, CSB_INPUT_T* csb_input, CSB_STATE_T* csb_state, uint32_t msTicks);

#endif
