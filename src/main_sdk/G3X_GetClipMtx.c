#include <nitro/types.h>

void MI_Copy64B(const void *src, void *dst);
s32 G3X_GetClipMtx(void *m) { if (*(vu32 *)0x4000600 & 0x8000000) return -1; MI_Copy64B((void *)0x4000640, m); return 0; }
