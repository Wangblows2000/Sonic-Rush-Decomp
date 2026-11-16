#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 state);

void func_02007ab8(u32 dmaNo)
{
    u32 enabled = OS_DisableInterrupts();
    vu16 *dmaCntp = &((vu16 *)0x040000b0)[dmaNo * 6 + 5];

    *dmaCntp &= ~0x3a00;
    *dmaCntp &= ~0x8000;
    {
        s32 dummy = dmaCntp[0];
    }
    {
        s32 dummy = dmaCntp[0];
    }
    if (dmaNo == 0) {
        vu32 *p = (vu32 *)(0x040000b0 + dmaNo * 12);
        *p = 0;
        *(p + 1) = 0;
        *(p + 2) = 0x81400001;
    }
    (void)OS_RestoreInterrupts(enabled);
}
