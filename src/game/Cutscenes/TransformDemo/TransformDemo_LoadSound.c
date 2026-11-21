/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov001_0231c394[];
s32 Sound_LoadSoundArchive();
extern u8 gSoundHeap[];
s32 NNS_SndArcLoadGroup();
s32 NNS_SndHandleInit();

void TransformDemo_LoadSound(s32 p0, s32 p1) {
    Sound_LoadSoundArchive(data_ov001_0231c394);
    NNS_SndArcLoadGroup(0xf, *(u32 *)gSoundHeap);
    NNS_SndHandleInit(((u8 *)p1 + 0x4));
}
