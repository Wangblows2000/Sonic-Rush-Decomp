#include <nitro/types.h>

#pragma thumb on

extern void *data_022c4638;
BOOL func_02050c3c(void) { if (data_022c4638 == NULL) return TRUE; return FALSE; }

#pragma thumb reset
