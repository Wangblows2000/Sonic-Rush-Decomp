#include <nitro/types.h>

void WFS_End(void);
BOOL WH_StateInReset(void);
void WH_ChangeSysState(s32 a);
void WH_Reset(void) { WFS_End(); if (WH_StateInReset()) return; WH_ChangeSysState(10); }
