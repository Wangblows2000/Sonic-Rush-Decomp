#include <nitro/types.h>

#pragma thumb on

extern void *data_022c465c;
void func_02051c30(s32 i);
void func_020519b0(s32 i);
void func_020520e0(s32 i) { if (data_022c465c != NULL && i < 8) { func_02051c30(i); func_020519b0(i); } }

#pragma thumb reset
