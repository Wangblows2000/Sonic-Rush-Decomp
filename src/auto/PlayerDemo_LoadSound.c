/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 PlayerDemo_GetSoundEntry();
extern u8 gPlayerDemoId[];
s32 Sound_ResetHeap();
extern u8 data_0207c5bc[];
s32 Sound_LoadSoundArchive();
extern u8 gSoundHeap[];
s32 NNS_SndArcLoadGroup();
s32 NNS_SndHandleInit();

void PlayerDemo_LoadSound(u8 *p0) {
    s32 t0 = PlayerDemo_GetSoundEntry();
    *(u32 *)((u8 *)p0 + 0x2c) = t0;
    s32 t1 = *(u32 *)((u8 *)p0 + 0x2c);
    if (*(u32 *)*(u32 *)((u8 *)p0 + 0x2c) == 0) {
        *(u32 *)((u8 *)p0 + 0x4) = (*(u32 *)((u8 *)p0 + 0x4) | 0x10000000);
        return;
    }
    if ((*(u32 *)((u8 *)t1 + 0x4) & 1) == 0) {
        s32 t2 = *(s16 *)((u8 *)gPlayerDemoId + 0);
        if (t2 != 0x2a) {
            Sound_ResetHeap(t2, gPlayerDemoId);
        }
        Sound_LoadSoundArchive(data_0207c5bc);
        NNS_SndArcLoadGroup(*(u32 *)*(u32 *)*(u32 *)((u8 *)p0 + 0x2c), *(u32 *)gSoundHeap);
    }
    NNS_SndHandleInit(((u8 *)p0 + 0x30));
    NNS_SndHandleInit(((u8 *)p0 + 0x34));
}

#pragma thumb reset
