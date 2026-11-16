/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 MapObject_Spawn();

void func_ov000_022ec16c(s32 p0, s32 p1) {
    s32 t0 = MapObject_Spawn(0xd7, 0, 0, ((u32)((s32)p1 << 0x10) >> 0x10), 0, 0, 0, 0, 0);
    *(u32 *)((u8 *)t0 + 0x348) = p0;
}
