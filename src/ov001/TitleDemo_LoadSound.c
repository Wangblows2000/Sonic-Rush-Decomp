#include <nitro/types.h>

extern u8 data_ov001_0231b7e0[];
extern s32 gSoundHeap;
void func_02036854(void *p);
void NNS_SndArcLoadGroup(s32 a, s32 b);

#pragma thumb on
void TitleDemo_LoadSound(void) {
    func_02036854(data_ov001_0231b7e0);
    NNS_SndArcLoadGroup(0, gSoundHeap);
}
#pragma thumb reset
