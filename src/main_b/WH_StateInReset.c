#include <nitro/types.h>

void WH_ChangeSysState(s32 a);
void WH_StateOutReset(void);
s32 func_02011b10(void (*cb)(void));
void WH_SetError(s32 v);
BOOL WH_StateInReset(void) { s32 r; WH_ChangeSysState(3); r = func_02011b10(WH_StateOutReset); if (r == 2) return TRUE; WH_SetError(r); return FALSE; }
