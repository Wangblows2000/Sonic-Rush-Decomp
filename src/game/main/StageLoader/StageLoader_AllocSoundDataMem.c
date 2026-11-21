#include <nitro/types.h>

extern void *gSoundHeap;
void NNS_SndHeapAlloc(void *a, u32 b, s32 c, s32 d, s32 e);
void StageLoader_AllocSoundDataMem(void) { NNS_SndHeapAlloc(gSoundHeap, 0x60400, 0, 0, 0); }
