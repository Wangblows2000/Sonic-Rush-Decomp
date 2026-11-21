#include <nitro/types.h>

extern u8 gSoundArchive[];
extern u32 gSoundHeap;
void NNS_SndArcInit(void *a, u32 b, u32 c, u32 d);
void NNS_SndArcPlayerSetup(u32 a);

void Sound_LoadSoundArchive(u32 a) {
    NNS_SndArcInit(gSoundArchive, a, gSoundHeap, 0);
    NNS_SndArcPlayerSetup(gSoundHeap);
}
