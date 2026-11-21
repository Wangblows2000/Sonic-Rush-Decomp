#include <nitro/types.h>

extern u8 gSoundArchive[];
extern u32 gSoundHeap;
void NNS_SndArcInitOnMemory(void *a, u32 b);
void NNS_SndArcPlayerSetup(u32 a);

void Sound_LoadSoundArchiveFromMemory(u32 a) {
    NNS_SndArcInitOnMemory(gSoundArchive, a);
    NNS_SndArcPlayerSetup(gSoundHeap);
}
