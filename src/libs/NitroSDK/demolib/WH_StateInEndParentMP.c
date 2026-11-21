#include <nitro/types.h>

void WH_ChangeSysState(s32 a);
void WH_StateOutEndParentMP(void);
s32 WM_EndMP(void (*cb)(void));
void WH_SetError(s32 v);
BOOL WH_StateInEndParentMP(void) { s32 r; WH_ChangeSysState(3); r = WM_EndMP(WH_StateOutEndParentMP); if (r == 2) return TRUE; WH_SetError(r); return FALSE; }
