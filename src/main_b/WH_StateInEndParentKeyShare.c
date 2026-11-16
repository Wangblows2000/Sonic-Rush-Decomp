#include <nitro/types.h>

void WH_StateOutEndParentKeyShare(void);
s32 WM_EndKeySharing(void (*cb)(void));
void WH_SetError(s32 v);
BOOL WH_StateInEndParentKeyShare(void) { s32 r = WM_EndKeySharing(WH_StateOutEndParentKeyShare); if (r == 2) return TRUE; WH_SetError(r); return FALSE; }
