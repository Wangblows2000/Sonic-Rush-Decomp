#include <nitro/types.h>

void WH_StateOutEndScan(void);
s32 func_02011770(void (*cb)(void));
void WH_SetError(s32 v);
BOOL WH_StateInEndScan(void) { s32 r = func_02011770(WH_StateOutEndScan); if (r == 2) return TRUE; WH_SetError(r); return FALSE; }
