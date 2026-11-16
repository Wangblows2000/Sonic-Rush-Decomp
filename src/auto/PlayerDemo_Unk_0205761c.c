/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 NNS_SndPlayerStopSeq();
s32 NNS_SndStopSoundAll();
s32 NNS_SndHandleReleaseSeq();
s32 Sound_ResetHeap();

void PlayerDemo_Unk_0205761c(u8 *p0) {
    if ((*(u32 *)((u8 *)p0 + 0x4) & 0x10000000) == 0) {
        if (*(u32 *)((u8 *)p0 + 0x34) != 0) {
            NNS_SndPlayerStopSeq(((u8 *)p0 + 0x34), 0);
        }
        s32 t0 = (*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x2c) + 0x4) & 2);
        if (t0 == 0) {
            NNS_SndStopSoundAll(2, t0);
            NNS_SndHandleReleaseSeq(((u8 *)p0 + 0x30));
            NNS_SndHandleReleaseSeq(((u8 *)p0 + 0x34));
            Sound_ResetHeap();
        }
    }
}

#pragma thumb reset
