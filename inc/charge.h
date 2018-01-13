#ifndef _CHARGE_H
#define _CHARGE_H
#include "structs.h"
#include <string.h>

void Charge_Init(CSB_STATE_T *state);
void Charge_Config(PACK_CONFIG_T *pack_config);
void Charge_Step(CSB_INPUT_T *input, CSB_STATE_T *state, CSB_OUTPUT_T *output);

#endif
