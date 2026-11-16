/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Model_Release();
s32 MapObject_Destructor();

void Boss3PartCD_Destructor(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    Model_Release((*(u32 *)((u8 *)p0 + 0x10) + 0x290));
    Model_Release((t0 + 0x404));
    MapObject_Destructor(p0);
}
