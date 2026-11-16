/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_0206e3c0();

void func_ov000_022fb23c(u8 *p0, s32 p1, s32 p2) {
    if (*(u16 *)p0 != 7) {
        return;
    }
    s32 t0 = *(u16 *)((u8 *)p0 + 0x4);
    if ((u32)t0 > (u32)2) {
        return;
    }
    if (*(s16 *)((u8 *)p0 + 0x8) == *(s16 *)((u8 *)p0 + 0xc)) {
        if (*(s16 *)((u8 *)p0 + 0xa) == *(s16 *)((u8 *)p0 + 0xe)) {
            *(u16 *)((u8 *)((u8 *)p2 + 0x13e) + (t0 * 0x2c)) = (*(u16 *)((u8 *)((u8 *)p2 + 0x13e) + (t0 * 0x2c)) & ~4);
            return;
        }
    }
    *(u32 *)((u8 *)(t0 * 0x2c + (s32)p2) + 0x14c) = p2;
    func_0206e3c0((*(u16 *)((u8 *)p0 + 0x4) * 0x2c + (s32)((u8 *)p2 + 0x134)), *(s16 *)((u8 *)p0 + 0x8), *(s16 *)((u8 *)p0 + 0xa), *(s16 *)((u8 *)p0 + 0xc), *(s16 *)((u8 *)p0 + 0xe));
}
