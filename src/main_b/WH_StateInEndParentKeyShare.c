#include <nitro/types.h>

void WH_StateOutEndParentKeyShare(void);
s32 func_02012d20(void (*cb)(void));
void WH_SetError(s32 v);
BOOL WH_StateInEndParentKeyShare(void) { s32 r = func_02012d20(WH_StateOutEndParentKeyShare); if (r == 2) return TRUE; WH_SetError(r); return FALSE; }
