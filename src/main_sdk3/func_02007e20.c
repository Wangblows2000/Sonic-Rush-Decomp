#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 state);
void func_01ff84c0(u32 dmaNo, u32 src, u32 dest, u32 ctrl);

void func_02007e20(u32 dmaNo, void *dest, u32 data, u32 size)
{
    vu32 *dmaCntp;

    if (size == 0) {
        return;
    }
    dmaCntp = &((vu32 *)0x040000b0)[dmaNo * 3 + 2];
    while (*dmaCntp & 0x80000000) {
    }
    {
        u32 enabled = OS_DisableInterrupts();
        ((vu32 *)0x040000e0)[dmaNo] = data;
        func_01ff84c0(dmaNo, (u32)&((vu32 *)0x040000e0)[dmaNo], (u32)dest, 0x85000000 | (size >> 2));
        (void)OS_RestoreInterrupts(enabled);
    }
    while (*dmaCntp & 0x80000000) {
    }
}
