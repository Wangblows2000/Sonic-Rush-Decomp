#include <nitro/types.h>

void OS_Terminate(void);

void MIi_CheckAnotherAutoDMA(u32 dmaNo, u32 dmaType)
{
    s32 n;

    for (n = 0; n < 3; n++) {
        u32 dmaCnt;
        u32 timing;
        if (n == dmaNo) {
            continue;
        }
        dmaCnt = *(vu32 *)(0x040000b8 + n * 12);
        if (!(dmaCnt & 0x80000000)) {
            continue;
        }
        timing = dmaCnt & 0x38000000;
        if (timing == dmaType || (timing == 0x08000000 && dmaType == 0x10000000) || (timing == 0x10000000 && dmaType == 0x08000000)) {
            continue;
        }
        if (timing == 0x18000000 || timing == 0x20000000 || timing == 0x28000000 || timing == 0x30000000 || timing == 0x38000000 || timing == 0x08000000 || timing == 0x10000000) {
            OS_Terminate();
        }
    }
}
