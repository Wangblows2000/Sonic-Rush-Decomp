#include <nitro/types.h>

void MI_Copy32B(const void *src, void *dst);
void G3X_SetFogTable(const void *src) { MI_Copy32B(src, (void *)0x4000360); }
