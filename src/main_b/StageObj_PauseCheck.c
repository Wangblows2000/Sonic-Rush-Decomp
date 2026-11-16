#include <nitro/types.h>

extern u32 gObjManagerFlags;
BOOL StageObj_PauseCheck(u32 v) { if ((gObjManagerFlags & 1) && !(v & 0x20)) return TRUE; return FALSE; }
