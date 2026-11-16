#include <nitro/types.h>

extern u8 gSoundArchive[];
extern u32 gSoundHeap;
void func_0201d868(void *a, u32 b, u32 c, u32 d);
void NNS_SndArcPlayerSetup(u32 a);

void Sound_LoadSoundArchive(u32 a) {
    func_0201d868(gSoundArchive, a, gSoundHeap, 0);
    NNS_SndArcPlayerSetup(gSoundHeap);
}
