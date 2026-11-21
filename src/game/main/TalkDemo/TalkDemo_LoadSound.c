/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 TalkDemo_GetEntry();
s32 TalkDemo_SetFlags();
s32 Sound_ResetHeap();
extern u8 data_0207cb7c[];
s32 Sound_LoadSoundArchive();
extern u8 gSoundHeap[];
s32 NNS_SndArcLoadGroup();
s32 NNS_SndHandleInit();

void TalkDemo_LoadSound(u8 *p0) {
    s32 t0 = TalkDemo_GetEntry();
    *(u32 *)((u8 *)p0 + 0x28) = t0;
    s32 t1 = *(u32 *)((u8 *)p0 + 0x28);
    if (*(u32 *)*(u32 *)((u8 *)p0 + 0x28) == 0) {
        TalkDemo_SetFlags(0, 0x10);
        return;
    }
    s32 t2 = (*(u32 *)((u8 *)t1 + 0x4) & 1);
    s32 t3 = *(u32 *)((u8 *)t1 + 0x4);
    if (t2 == 0) {
        if ((t3 & 2) == 0) {
            Sound_ResetHeap(2, t2, (t3 & 2));
            Sound_LoadSoundArchive(data_0207cb7c);
        }
        NNS_SndArcLoadGroup(*(u32 *)*(u32 *)*(u32 *)((u8 *)p0 + 0x28), *(u32 *)gSoundHeap);
    }
    NNS_SndHandleInit(((u8 *)p0 + 0x2c));
    NNS_SndHandleInit(((u8 *)p0 + 0x30));
}

#pragma thumb reset
