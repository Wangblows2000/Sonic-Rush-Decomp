#include <nitro/types.h>

BOOL ObjRect_FlagCheck(u32 a, u32 b) { if ((a & ~0x80000007) & ~(b & ~7)) { if ((a & 7) >= (b & 7)) return TRUE; } return FALSE; }
