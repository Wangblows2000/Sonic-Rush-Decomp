#include <nitro/types.h>

extern u8 gSoundArchive[];
extern u32 gSoundHeap;
void func_0201d580(void *a, u32 b);
void NNS_SndArcPlayerSetup(u32 a);

void Sound_LoadSoundArchiveFromMemory(u32 a) {
    func_0201d580(gSoundArchive, a);
    NNS_SndArcPlayerSetup(gSoundHeap);
}
