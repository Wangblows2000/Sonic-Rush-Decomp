#include <nitro/types.h>

extern u8 data_ov001_0231b710[];
extern s32 gSoundHeap;
void Sound_LoadSoundArchive(void *p);
void NNS_SndArcLoadGroup(s32 a, s32 b);

#pragma thumb on
void TitleScreen_LoadSound(void) {
    Sound_LoadSoundArchive(data_ov001_0231b710);
    NNS_SndArcLoadGroup(0, gSoundHeap);
}
#pragma thumb reset
