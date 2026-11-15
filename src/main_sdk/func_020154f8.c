#include <nitro/types.h>

s32 func_020154f8(s32 v) { s32 i; for (i = 0; i < 16; i++) { if ((v >> i) & 1) return i; } return -1; }
