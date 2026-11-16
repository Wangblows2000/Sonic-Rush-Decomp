/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 TalkDemo_GetFlags();
s32 NNS_SndStopSoundAll();
s32 NNS_SndHandleReleaseSeq();
s32 Sound_ResetHeap();

void func_02060824(u8 *p0) {
    s32 t0 = TalkDemo_GetFlags(0);
    if ((t0 & 0x10) == 0) {
        s32 t1 = (*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x28) + 0x4) & 1);
        if (t1 == 0) {
            NNS_SndStopSoundAll(1, t1);
            NNS_SndHandleReleaseSeq(((u8 *)p0 + 0x2c));
            NNS_SndHandleReleaseSeq(((u8 *)p0 + 0x30));
            s32 t2 = (*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x28) + 0x4) & 2);
            if (t2 == 0) {
                Sound_ResetHeap(2, t2);
            }
        }
    }
}

#pragma thumb reset
