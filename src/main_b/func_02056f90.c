#include <nitro/types.h>

#pragma thumb on

extern u8 data_0207c450;
void func_0203879c(void *p, s32 a, s32 b);
void func_02056f90(void) { func_0203879c(&data_0207c450, 0, -1); }

#pragma thumb reset
