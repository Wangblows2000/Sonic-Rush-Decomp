#include <nitro/types.h>

void MI_CpuFill8(void *dst, u8 v, u32 size);
extern void *data_02080f44;
void func_0201a980(void *p) { data_02080f44 = p; MI_CpuFill8(p, 0, 0x21c); }
