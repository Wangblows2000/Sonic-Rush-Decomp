#include <nitro/types.h>

#pragma thumb on

extern void *data_022c465c;
void func_02051ca4(s32 i);
void func_02051a74(s32 i);
void func_02052104(s32 i) { if (data_022c465c != NULL && i < 23) { func_02051ca4(i); func_02051a74(i); } }

#pragma thumb reset
