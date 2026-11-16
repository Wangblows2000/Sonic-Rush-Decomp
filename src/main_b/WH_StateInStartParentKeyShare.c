#include <nitro/types.h>

extern u8 sWMKeySetBuf;
void WH_ChangeSysState(s32 a);
void WH_StateOutStartParentKeyShare(void);
s32 func_02012da0(void (*cb)(void), void *p);
void WH_SetError(s32 v);
BOOL WH_StateInStartParentKeyShare(void) { s32 r; WH_ChangeSysState(6); r = func_02012da0(WH_StateOutStartParentKeyShare, &sWMKeySetBuf); if (r == 2) return TRUE; WH_SetError(r); return FALSE; }
