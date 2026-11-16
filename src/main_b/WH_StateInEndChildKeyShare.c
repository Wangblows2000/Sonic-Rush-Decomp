#include <nitro/types.h>

extern s32 sSysState;
void WH_ChangeSysState(s32 a);
void WH_StateOutEndParentKeyShare(void);
s32 func_02012d20(void (*cb)(void));
void WH_SetError(s32 v);
BOOL WH_StateInEndChildKeyShare(void) { s32 r; if (sSysState != 6) return FALSE; WH_ChangeSysState(3); r = func_02012d20(WH_StateOutEndParentKeyShare); if (r == 2) return TRUE; WH_SetError(r); return FALSE; }
