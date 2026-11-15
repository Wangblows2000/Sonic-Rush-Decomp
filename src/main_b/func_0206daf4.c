#include <nitro/types.h>

BOOL func_0206daf4(u32 a, u32 b) { if ((a & ~0x80000007) & ~(b & ~7)) { if ((a & 7) >= (b & 7)) return TRUE; } return FALSE; }
