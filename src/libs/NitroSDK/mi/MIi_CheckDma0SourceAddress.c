#include <nitro/types.h>

void OS_Terminate(void);

void MIi_CheckDma0SourceAddress(u32 dmaNo, u32 src, u32 size, u32 dir)
{
    if (dmaNo == 0) {
        u32 addStart = src & 0xff000000;
        u32 addEnd;
        switch (dir) {
        case 0:
            addEnd = src + size;
            break;
        case 0x800000:
            addEnd = src - size;
            break;
        default:
            addEnd = src;
            break;
        }
        addEnd &= 0xff000000;
        if (addStart == 0x04000000 || addStart >= 0x08000000 || addEnd == 0x04000000 || addEnd >= 0x08000000) {
            OS_Terminate();
        }
    }
}
