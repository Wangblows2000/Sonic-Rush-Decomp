#include <nitro/types.h>

#pragma thumb on

extern void *data_022c465c;
BOOL func_020521a0(void) { if (data_022c465c != NULL) return TRUE; return FALSE; }

#pragma thumb reset
