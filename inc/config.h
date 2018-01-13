#ifndef _CONFIG_H_
#define _CONFIG_H_

#define MAX_NUM_MODULES 15
#define MAX_CELLS_PER_MODULE 12
#define MAX_THERMISTORS_PER_MODULE 24


#define LTC6804_BAUD 600000
#define CAN_BAUD 500000

#define CELL_MIN_mV 2500 // from datasheet, contact elliot
#define CELL_MAX_mV 4250 // from datasheet, contact elliot
#define CELL_CAPACITY_cAh 250
#define NUM_MODULES 6
#define CELL_CHARGE_C_RATING_cC 5
#define BALANCE_ON_THRESHOLD_mV 4
#define BALANCE_OFF_THRESHOLD_mV 1
#define PACK_CELLS_PARALLEL 12
#define CV_MIN_CURRENT_mA 100
#define CV_MIN_CURRENT_ms 60000
#define CC_CELL_VOLTAGE_mV 4300
#define CELL_DISCHARGE_C_RATING_cC 200 // at 27 degrees C
#define MAX_CELL_TEMP_dC 600
#define MODULE_CELL_COUNT 12

#define BALANCE_ON_THRESH_MS 41000
#define BALANCE_OFF_WAITING_THRESH_MS 188000

#endif
