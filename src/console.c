#include <string.h>
#include <stdlib.h>
#include "console.h"
#include "microrl.h"
#include "board.h"
#include "console_types.h"
#include "config.h"
#include "pack_config.h"
#include "ssm.h"

static CSB_INPUT_T *csb_input;
static CSB_STATE_T *csb_state;
static CONSOLE_OUTPUT_T *console_output;

static uint32_t lastPackCurrentPrintTime = 0;
static uint32_t lastPackVoltagePrintTime = 0;

// [TODO] Fix to not parse strings falsely
uint32_t my_atou(const char *str) {
    uint32_t res = 0; // Initialize result

    // Iterate through all characters of input string and
    // update result
    uint32_t i;
    for ( i = 0; str[i] != '\0'; ++i)
        res = res*10 + str[i] - '0';

    // return result.
    return res;
}

static void get(const char * const * argv) {
    rw_loc_label_t rwloc;

    //loop over r/w entries
    bool foundloc = false;
    for (rwloc = 0; rwloc < RWL_LENGTH; ++rwloc){
        if (strcmp(argv[1],locstring[rwloc]) == 0){
            foundloc = true;
            break;
        }
    }

    if (foundloc) {
        char tempstr[20];
        switch (rwloc) {
            case RWL_cell_min_mV:
                utoa(csb_state->pack_config->cell_min_mV, tempstr,10);
                Board_Println(tempstr);
                break;
            case RWL_cell_max_mV:
                utoa(csb_state->pack_config->cell_max_mV, tempstr,10);
                Board_Println(tempstr);
                break;
            case RWL_cell_capacity_cAh:
                utoa(csb_state->pack_config->cell_capacity_cAh, tempstr,10);
                Board_Println(tempstr);
                break;
            case RWL_num_modules:
                utoa(csb_state->pack_config->num_modules, tempstr,10);
                Board_Println(tempstr);
                break;
            case RWL_module_cell_count:
                utoa(csb_state->pack_config->module_cell_count, tempstr,10);
                Board_Println(tempstr);
                break;
            case RWL_cell_charge_c_rating_cC:
              utoa(csb_state->pack_config->cell_charge_c_rating_cC, tempstr,10);
              Board_Println(tempstr);
              break;
            case RWL_bal_on_thresh_mV:
                utoa(csb_state->pack_config->bal_on_thresh_mV, tempstr,10);
                Board_Println(tempstr);
                break;
            case RWL_bal_off_thresh_mV:
                utoa(csb_state->pack_config->bal_off_thresh_mV, tempstr,10);
                Board_Println(tempstr);
                break;
            case RWL_pack_cells_p:
                utoa(csb_state->pack_config->pack_cells_p, tempstr,10);
                Board_Println(tempstr);
                break;
            case RWL_cv_min_current_mA:
                utoa(csb_state->pack_config->cv_min_current_mA, tempstr,10);
                Board_Println(tempstr);
                break;
            case RWL_cv_min_current_ms:
                utoa(csb_state->pack_config->cv_min_current_ms, tempstr,10);
                Board_Println(tempstr);
                break;
            case RWL_cc_cell_voltage_mV:
                utoa(csb_state->pack_config->cc_cell_voltage_mV, tempstr,10);
                Board_Println(tempstr);
                break;
            case RWL_LENGTH:
                break;
        }

    }
    else {
        //loop over r/o entries
        ro_loc_label_t roloc;
        for (roloc = (ro_loc_label_t)ROL_FIRST; roloc< ROL_LENGTH; ++roloc){
            if (strcmp(argv[1],locstring[roloc]) == 0){
                foundloc = true;
                break;
            }
        }
        if (foundloc) {
            char tempstr[20];
            switch (roloc) {
                case ROL_state:
                    Board_Println(CSB_SSM_MODE_NAMES[csb_state->curr_mode]);
                    Board_Println(CSB_INIT_MODE_NAMES[csb_state->init_state]);
                    Board_Println(CSB_CHARGE_MODE_NAMES[csb_state->charge_state]);
                    break;
                case ROL_pack_cell_max_mV:
                    utoa(csb_input->pack_status->pack_cell_max_mV, tempstr,10);
                    Board_Println(tempstr);
                    break;
                case ROL_pack_current_mA:
                    utoa(csb_input->pack_status->pack_current_mA, tempstr,10);
                    Board_Println(tempstr);
                    break;
                case ROL_pack_voltage_mV:
                    utoa(csb_input->pack_status->pack_voltage_mV, tempstr,10);
                    Board_Println(tempstr);
                    break;
                case ROL_fault:
                    //change this to say what fault
                    utoa(csb_input->low_side_cntr_fault, tempstr,10);
                    Board_Println(tempstr);
                    break;
                case ROL_LENGTH:
                    break; //how the hell?
            }
        }
        else{
            Board_Println("invalid get location");
        }
    }
}

static void set(const char * const * argv) {
    if (csb_state->curr_mode != CSB_SSM_MODE_IDLE)
    {
        Board_Println("Set failed (not in standby mode)!");
        return;
    }
    rw_loc_label_t rwloc;
    //loop over r/w entries
    bool foundloc = false;
    for (rwloc = 0; rwloc < RWL_LENGTH; ++rwloc){
        if (strcmp(argv[1],locstring[rwloc]) == 0){
            foundloc = true;
            break;
        }
    }
    if(foundloc){
        switch (rwloc) {
            case RWL_cell_min_mV:
                csb_state->pack_config->cell_min_mV = my_atou(argv[2]);
                Board_Println("Done!");
                break;
            case RWL_cell_max_mV:
                csb_state->pack_config->cell_max_mV = my_atou(argv[2]);
                Board_Println("Done!");
                break;
            case RWL_cell_capacity_cAh:
                csb_state->pack_config->cell_capacity_cAh = my_atou(argv[2]);
                Board_Println("Done!");
                break;
            case RWL_num_modules:
                csb_state->pack_config->num_modules = my_atou(argv[2]);
                Board_Println("Done!");
                break;
            case RWL_module_cell_count:
                csb_state->pack_config->module_cell_count = my_atou(argv[2]);
                Board_Println("Done!");
                break;
            case RWL_cell_charge_c_rating_cC:
              csb_state->pack_config->cell_charge_c_rating_cC = my_atou(argv[2]);
              Board_Println("Done!");
              break;
            case RWL_bal_on_thresh_mV:
                csb_state->pack_config->bal_on_thresh_mV = my_atou(argv[2]);
                Board_Println("Done!");
                break;
            case RWL_bal_off_thresh_mV:
                csb_state->pack_config->bal_off_thresh_mV = my_atou(argv[2]);
                Board_Println("Done!");
                break;
            case RWL_pack_cells_p:
                csb_state->pack_config->pack_cells_p = my_atou(argv[2]);
                Board_Println("Done!");
                break;
            case RWL_cv_min_current_mA:
                csb_state->pack_config->cv_min_current_mA = my_atou(argv[2]);
                Board_Println("Done!");
                break;
            case RWL_cv_min_current_ms:
                csb_state->pack_config->cv_min_current_ms = my_atou(argv[2]);
                Board_Println("Done!");
                break;
            case RWL_cc_cell_voltage_mV:
                csb_state->pack_config->cc_cell_voltage_mV = my_atou(argv[2]);
                Board_Println("Done!");
                break;
            case RWL_LENGTH:
                break;
        }
    } else {
        //loop over r/o entries
        ro_loc_label_t roloc;
        for (roloc = (ro_loc_label_t)ROL_FIRST; roloc< ROL_LENGTH; ++roloc){
            if (strcmp(argv[1],locstring[roloc]) == 0){
                foundloc = true;
                Board_Println("this location is read only");
                return;
            }
        }
        Board_Println("invalid location");
    }
}

static void help(const char * const * argv) {
    command_label_t command_i = 0;
    for (command_i = 0; command_i < NUMCOMMANDS; ++command_i)
    {
        if (strcmp(argv[1],commands[command_i]) == 0){
            Board_Println_BLOCKING(helpstring[command_i]); //blocking print

            break;
        }
    }
    Board_Print("");

    if (command_i == C_GET || command_i == C_SET)
    {
        rw_loc_label_t i;
        Board_Println_BLOCKING("------r/w entries------");
        for (i = 0; i < RWL_LENGTH; ++i){
            Board_Println_BLOCKING(locstring[i]); //blocking print.
        }

        Board_Println_BLOCKING("------r/o entries------");
        for (i = ROL_FIRST; i < (rw_loc_label_t)(ROL_LENGTH); ++i){
            Board_Println_BLOCKING(locstring[i]); //blocking print.
        }
    }
}

static void config_def(const char * const * argv) {
  bool foundloc = false;
  config_loc_label_t configloc;
  for (configloc = 0; configloc< CONL_LENGTH; ++configloc){
      if (strcmp(argv[1],config_locstring[configloc]) == 0){
          foundloc = true;
          break;
      }
  }
  if (foundloc) {
      switch (configloc) {
          case CONL_MY18:
              MY18_Pack_Config(csb_state);
              Board_Println("Done!");
              break;
          case CONL_MY16:
              MY16_Pack_Config(csb_state);
              Board_Println("Done!");
              break;
          case CONL_LENGTH:
              break;
      }
  } else {
      Board_Println("invalid location");
  }
}

static void measure(const char * const * argv) {
    if (strcmp(argv[1],"on") == 0) {
        console_output->measure_on = true;
        Board_Println("Measure On!");

    } else if (strcmp(argv[1],"off") == 0) {
        console_output->measure_on = false;
        Board_Println("Measure Off!");

    } else if (strcmp(argv[1],"print_flags") == 0) {

        if(console_output->measure_packcurrent) {
            Board_Println("Pack Current: On");
        } else {
            Board_Println("Pack Current: Off");
        }

        if(console_output->measure_packvoltage) {
            Board_Println("Pack Current: On");
        } else {
            Board_Println("Pack Current: Off");
        }

    } else if (strcmp(argv[1],"packcurrent") == 0) {
        console_output->measure_packcurrent = !console_output->measure_packcurrent;

    } else if (strcmp(argv[1],"packvoltage") == 0) {
        console_output->measure_packvoltage = !console_output->measure_packvoltage;

    } else {
        Board_Println("Unrecognized command!");
    }
}


static void bal(const char * const * argv) {
    UNUSED(argv);
    if (Is_Valid_Jump(csb_state->curr_mode, CSB_SSM_MODE_BALANCE)) {

        if (console_output->valid_mode_request) {
          if (strcmp(argv[1],"off") == 0) {
              console_output->valid_mode_request = false;
              console_output->balance_mV = UINT32_MAX;
              Board_Println("bal off");
          } else {
              console_output->valid_mode_request = true;
              console_output->mode_request = CSB_SSM_MODE_BALANCE;
              console_output->balance_mV = my_atou(argv[1]);
              Board_Println("bal on");
          }
        }
    } else {
        Board_Println("Must be in standby");
    }
}

static void chrg(const char * const * argv) {
    UNUSED(argv);
    if (Is_Valid_Jump(csb_state->curr_mode, CSB_SSM_MODE_CHARGE)) {
        if (console_output->valid_mode_request) {
            console_output->valid_mode_request = false;
            Board_Println("chrg off");
        } else {
            console_output->valid_mode_request = true;
            console_output->mode_request = CSB_SSM_MODE_CHARGE;
            Board_Println("chrg on");
        }
    } else {
        Board_Println("Must be in standby");
    }
}

static const EXECUTE_HANDLER handlers[] = {get, set, help, config_def, bal, chrg, measure};

void console_init(CSB_INPUT_T * input, CSB_STATE_T * state, CONSOLE_OUTPUT_T *con_output){
    csb_input = input;
    csb_state = state;
    console_output = con_output;
    console_output->valid_mode_request = false;
    console_output->mode_request = CSB_SSM_MODE_IDLE;
    console_output->config_default = false;

    console_output->measure_on = false;
    console_output->measure_packcurrent = false;
    console_output->measure_packvoltage = false;
}

void executerl(int32_t argc, const char * const * argv){
    uint32_t command_i = 0;
    bool found_command = false;
    for (command_i = 0; command_i < NUMCOMMANDS; ++command_i)
    {
        if (strcmp(argv[0],commands[command_i]) == 0){
            found_command = true;
            break;
        }
    }
    if (found_command) {
        if (nargs[command_i] == (uint32_t)(argc-1))
        {
            handlers[command_i](argv);
        }
        else {
            Board_Println("incorrect number of args");
        }
    }
    else{
        Board_Println("Unrecognized command");
    }
}

void Output_Measurements(
        CONSOLE_OUTPUT_T *console_output,
        CSB_INPUT_T* csb_input,
        uint32_t msTicks
) {

    char tempstr[20];

    if(console_output->measure_on) {

        bool printPackCurrent = (msTicks - lastPackCurrentPrintTime) >
                PACK_CURRENT_PRINT_PERIOD_ms;
        if(console_output->measure_packcurrent && printPackCurrent) {
            utoa(msTicks, tempstr, 10); // print msTicks
            Board_Print_BLOCKING(tempstr);
            Board_Print_BLOCKING(",pcurr,");
            utoa(csb_input->pack_status->pack_current_mA, tempstr, 10);
            Board_Println_BLOCKING(tempstr); // print pack current
            Board_Print_BLOCKING("\n");
            lastPackCurrentPrintTime = msTicks;
        }

        bool printPackVoltage = (msTicks - lastPackVoltagePrintTime) >
                PACK_VOLTAGE_PRINT_PERIOD_ms;
        if(console_output->measure_packvoltage && printPackVoltage) {
            utoa(msTicks, tempstr, 10); // print msTicks
            Board_Print_BLOCKING(tempstr);
            Board_Print_BLOCKING(",pvolt,");
            utoa(csb_input->pack_status->pack_voltage_mV, tempstr, 10);
            Board_Println_BLOCKING(tempstr); // print pack voltage
            Board_Print_BLOCKING("\n");
            lastPackVoltagePrintTime = msTicks;
        }
    }
}
