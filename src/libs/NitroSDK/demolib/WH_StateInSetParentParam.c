#include <nitro/types.h>

extern u8 sParentParam;
void WH_ChangeSysState(s32 a);
void WH_StateOutSetParentParam(void);
s32 WM_SetParentParameter(void (*cb)(void), void *p);
void WH_SetError(s32 v);
BOOL WH_StateInSetParentParam(void) { s32 r; WH_ChangeSysState(3); r = WM_SetParentParameter(WH_StateOutSetParentParam, &sParentParam); if (r == 2) return TRUE; WH_SetError(r); WH_ChangeSysState(9); return FALSE; }
