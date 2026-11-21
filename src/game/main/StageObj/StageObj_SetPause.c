#include <nitro/types.h>

extern u32 gObjManagerFlags;
void StageObj_SetPause(void) { gObjManagerFlags |= 1; }
