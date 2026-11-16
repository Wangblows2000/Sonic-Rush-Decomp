#include <nitro/types.h>

s32 mystrlen(const u16 *s) { s32 n = 0; while (*s++ != 0) n++; return n; }
