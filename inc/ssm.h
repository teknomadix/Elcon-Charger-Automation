#ifndef SSM_H
#define SSM_H

#include "structs.h"
#include "config.h"

void SSM_Init(CSB_STATE_T *state, PACK_CONFIG_T *pack_config);
void Init_Step(CSB_INPUT_T *input, CSB_STATE_T *state, CSB_OUTPUT_T *output);
bool Is_Valid_Jump(CSB_SSM_MODE_T mode1, CSB_SSM_MODE_T mode2);
bool Is_State_Done(CSB_STATE_T *state);
void SSM_Step(CSB_INPUT_T *input, CSB_STATE_T *state, CSB_OUTPUT_T *output);


#endif
