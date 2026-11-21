/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 MIi_CheckAnotherAutoDMA();
s32 MIi_CheckDma0SourceAddress();
s32 MI_WaitDma();
s32 MIi_DmaSetParams();

void MI_HBlankDmaCopy16(s32 p0, s32 p1, s32 p2, s32 p3) {
    MIi_CheckAnotherAutoDMA(p0, 0x10000000);
    MIi_CheckDma0SourceAddress(p0, p1, p3, 0);
    if ((s32)p3 == 0) {
        return;
    }
    MI_WaitDma(p0);
    MIi_DmaSetParams(p0, p1, p2, (0x92600000 | ((u32)(s32)p3 >> 1)));
}
