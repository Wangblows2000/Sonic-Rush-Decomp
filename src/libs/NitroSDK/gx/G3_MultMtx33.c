#include <nitro/types.h>

void MI_Copy36B(const void *src, void *dst);
void G3_MultMtx33(const void *m) { *(vu32 *)0x4000400 = 0x1a; MI_Copy36B(m, (void *)0x4000400); }
