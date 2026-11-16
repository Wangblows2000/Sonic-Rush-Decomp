/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 WBT_Unk_0201567c();

s32 WBT_CalcPacketbitmapSize(s32 p0) {
    s32 t0 = WBT_Unk_0201567c();
    return (((((((s32)p0 / t0) + (((s32)p0 % t0) != 0)) & 0x1f) != 0) + ((s32)(((s32)p0 / t0) + (((s32)p0 % t0) != 0)) >> 5)) << 2);
}
