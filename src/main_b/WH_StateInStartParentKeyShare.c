#include <nitro/types.h>

extern u8 data_022c1a40;
void WH_ChangeSysState(s32 a);
void func_0203fd30(void);
s32 func_02012da0(void (*cb)(void), void *p);
void WH_SetError(s32 v);
BOOL WH_StateInStartParentKeyShare(void) { s32 r; WH_ChangeSysState(6); r = func_02012da0(func_0203fd30, &data_022c1a40); if (r == 2) return TRUE; WH_SetError(r); return FALSE; }
