#include <nitro/types.h>

extern s32 sSysState;
void WH_ChangeSysState(s32 a);
BOOL WH_EndScan(void) { if (sSysState != 2) return FALSE; WH_ChangeSysState(3); return TRUE; }
