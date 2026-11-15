#include <nitro/types.h>

u16 func_02042904(u16 v) { u16 n = 0; while (v != 0) { if (v & 1) n++; v >>= 1; } return n; }
