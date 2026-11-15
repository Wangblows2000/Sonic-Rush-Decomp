#include <nitro/types.h>

void MI_Copy64B(const void *src, void *dst);
s32 func_020040e4(void *m) { if (*(vu32 *)0x4000600 & 0x8000000) return -1; MI_Copy64B((void *)0x4000640, m); return 0; }
