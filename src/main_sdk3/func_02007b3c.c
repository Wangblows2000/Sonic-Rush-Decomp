#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 state);

void func_02007b3c(u32 dmaNo)
{
    u32 enabled = OS_DisableInterrupts();
    vu32 *dmaCntp = &((vu32 *)0x040000b0)[dmaNo * 3 + 2];

    while (*dmaCntp & 0x80000000) {
    }
    if (dmaNo == 0) {
        vu32 *p = (vu32 *)(0x040000b0 + dmaNo * 12);
        *p = 0;
        *(p + 1) = 0;
        *(p + 2) = 0x81400001;
    }
    (void)OS_RestoreInterrupts(enabled);
}
