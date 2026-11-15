#include <nitro/types.h>

void MI_Copy32B(const void *src, void *dst);
void func_02004070(const void *src) { MI_Copy32B(src, (void *)0x4000360); }
