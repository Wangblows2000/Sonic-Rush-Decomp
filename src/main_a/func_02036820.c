#include <nitro/types.h>

extern u8 data_022504e8[];
extern u32 gSoundHeap;
void func_0201d580(void *a, u32 b);
void NNS_SndArcPlayerSetup(u32 a);

void func_02036820(u32 a) {
    func_0201d580(data_022504e8, a);
    NNS_SndArcPlayerSetup(gSoundHeap);
}
