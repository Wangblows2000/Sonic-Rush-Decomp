/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Model_Release();
s32 NNS_SndHandleReleaseSeq();
s32 MapObject_Destructor();

void BossFPartDE_Destructor(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    Model_Release((*(u32 *)((u8 *)p0 + 0x10) + 0x4d4));
    NNS_SndHandleReleaseSeq((t0 + 0x4a8));
    MapObject_Destructor(p0);
}
