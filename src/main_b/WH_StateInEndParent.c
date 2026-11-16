#include <nitro/types.h>

void WH_StateOutEndParent(void);
s32 WM_EndParent(void (*cb)(void));
void WH_SetError(s32 v);
BOOL WH_StateInEndParent(void) { s32 r = WM_EndParent(WH_StateOutEndParent); if (r == 2) return TRUE; WH_SetError(r); return FALSE; }
