#include <nitro/types.h>

void WH_ChangeSysState(s32 a);
void WH_StateOutEndChildMP(void);
s32 func_02011bb8(void (*cb)(void));
void WH_SetError(s32 v);
BOOL WH_StateInEndChildMP(void) { s32 r; WH_ChangeSysState(3); r = func_02011bb8(WH_StateOutEndChildMP); if (r == 2) return TRUE; WH_SetError(r); return FALSE; }
