#include <nitro/types.h>

void MI_Copy36B(const void *src, void *dst);
s32 func_0200409c(const void *m) { if (*(vu32 *)0x4000600 & 0x8000000) return -1; MI_Copy36B((void *)0x4000680, (void *)m); return 0; }
