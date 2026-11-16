#include <nitro/types.h>

extern u8 data_022504e8[];
extern u32 gSoundHeap;
void func_0201d868(void *a, u32 b, u32 c, u32 d);
void NNS_SndArcPlayerSetup(u32 a);

void func_02036854(u32 a) {
    func_0201d868(data_022504e8, a, gSoundHeap, 0);
    NNS_SndArcPlayerSetup(gSoundHeap);
}
