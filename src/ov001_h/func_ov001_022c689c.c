#include <nitro/types.h>

extern void *data_022b4574;
void func_020371c8(void *t);

void func_ov001_022c689c(void) {
    {
        u32 v = (*(vu32 *)0x4000000 & 0x1f00) >> 8;
        *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | ((v & ~1) << 8);
    }
    func_020371c8(data_022b4574);
}
