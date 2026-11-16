#include <nitro/types.h>

extern void *gSoundHeap;
void func_0201d9e4(void *a, u32 b, s32 c, s32 d, s32 e);
void StageLoader_AllocSoundDataMem(void) { func_0201d9e4(gSoundHeap, 0x60400, 0, 0, 0); }
