/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_0200225c();
extern u8 gDisplayTable[];

s32 func_ov000_0233c4d8(s32 p0, s32 p1) {
    s32 t0 = *(u16 *)((u8 *)((s32)p0 + ((s32)p1 << 1)) + 0x4);
    *(u16 *)((u8 *)p0 + ((s32)p1 << 1)) = (*(s16 *)((u8 *)p0 + ((s32)p1 << 1)) + 0x1);
    s32 t1 = *(u16 *)((u8 *)p0 + ((s32)p1 << 1));
    s32 t2 = func_0200225c(((*(u16 *)((u8 *)p0 + ((s32)p1 << 1)) * 0xa) << 0xc), (t0 << 0xc));
    s32 t3 = *(u32 *)((u8 *)gDisplayTable + ((s32)p1 << 2));
    *(u16 *)((u8 *)t3 + 0x24) = ((s32)t2 >> 0xc);
    return ((u32)t1 >= (u32)t0);
}
