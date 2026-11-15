#include <nitro/types.h>

#pragma thumb on

extern void *data_0224ff40;
void func_02054dbc(void);
void func_02051628(BOOL on) { if (on) data_0224ff40 = (void *)func_02054dbc; else data_0224ff40 = NULL; }

#pragma thumb reset
