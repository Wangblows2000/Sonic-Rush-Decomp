#include <nitro/types.h>

extern u32 gObjManagerFlags;
void StageObj_ClearPause(void) { gObjManagerFlags &= ~1; }
