#include <nitro/types.h>

BOOL ObjAngle_IsUpsideDown(u32 v) { if (v > 0x60 && v < 0xa0) return TRUE; return FALSE; }
