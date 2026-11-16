#include <nitro/types.h>

void WH_StateOutEndParent(void);
s32 func_020118bc(void (*cb)(void));
void WH_SetError(s32 v);
BOOL WH_StateInEndParent(void) { s32 r = func_020118bc(WH_StateOutEndParent); if (r == 2) return TRUE; WH_SetError(r); return FALSE; }
