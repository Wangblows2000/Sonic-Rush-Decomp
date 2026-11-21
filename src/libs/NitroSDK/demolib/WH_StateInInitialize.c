#include <nitro/types.h>

extern u8 data_022c2c60;
void WH_ChangeSysState(s32 a);
void WH_StateOutInitialize(void);
s32 WM_Initialize(void *p, void (*cb)(void), s32 n);
void WH_SetError(s32 v);
BOOL WH_StateInInitialize(void) { s32 r; WH_ChangeSysState(3); r = WM_Initialize(&data_022c2c60, WH_StateOutInitialize, 2); if (r == 2) return TRUE; WH_SetError(r); WH_ChangeSysState(10); return FALSE; }
