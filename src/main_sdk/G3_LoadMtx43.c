#include <nitro/types.h>

void func_020051d0(const void *src, void *dst);
void G3_LoadMtx43(const void *m) { *(vu32 *)0x4000400 = 0x17; func_020051d0(m, (void *)0x4000400); }
