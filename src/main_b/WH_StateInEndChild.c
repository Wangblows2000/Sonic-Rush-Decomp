#include <nitro/types.h>

void WH_ChangeSysState(s32 a);
void WH_StateOutEndChild(void);
s32 func_02011598(void (*cb)(void), s32 a);
void WH_SetError(s32 v);
void WH_Reset(void);
BOOL WH_StateInEndChild(void) { s32 r; WH_ChangeSysState(3); r = func_02011598(WH_StateOutEndChild, 0); if (r == 2) return TRUE; WH_SetError(r); WH_Reset(); return FALSE; }
