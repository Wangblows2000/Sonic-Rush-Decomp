/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 WBT_IsPacketReceived(u8 *p0, s32 p1) {
    u32 t0 = *(u32 *)((u8 *)p0 + 0x14);
    u32 t1 = *(u32 *)((u8 *)t0 + (((s32)(s32)p1 >> 5) << 2));
    s32 v2 = 1;
    if ((t1 & (1 << ((s32)p1 & 0x1f))) == 0) {
        v2 = 0;
    }
    return v2;
}
