#include <nitro/types.h>

#pragma thumb on

extern void *data_022c4da4;
BOOL func_02060e8c(void) { if (data_022c4da4 != NULL) return TRUE; return FALSE; }

#pragma thumb reset
