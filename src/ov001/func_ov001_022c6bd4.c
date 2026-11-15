#include <nitro/types.h>

extern void *data_022b4574;
void func_020371c8(void *p);

void func_ov001_022c6bd4(void) {
    *(vu32 *)0x04000000 = (*(vu32 *)0x04000000 & ~0x1f00) | 0x700;
    func_020371c8(data_022b4574);
}
