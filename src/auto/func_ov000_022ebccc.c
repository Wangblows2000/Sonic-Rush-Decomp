/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_0206e3c0();

s32 func_ov000_022ebccc(u8 *p0, s32 p1, s32 p2) {
    s32 t0 = *(u16 *)p0;
    if (t0 != 7) {
        return t0;
    }
    s32 t1 = *(u16 *)((u8 *)p0 + 0x4);
    if ((u32)t1 > (u32)1) {
        return t1;
    }
    s32 t2 = *(u16 *)((u8 *)((t1 * 0x2c + (s32)p2) + 0x300) + 0x82);
    *(u32 *)((u8 *)(t1 * 0x2c + (s32)p2) + 0x390) = p2;
    func_0206e3c0((*(u16 *)((u8 *)p0 + 0x4) * 0x2c + (s32)((u8 *)p2 + 0x378)), *(s16 *)((u8 *)p0 + 0x8), *(s16 *)((u8 *)p0 + 0xa), *(s16 *)((u8 *)p0 + 0xc), *(s16 *)((u8 *)p0 + 0xe));
    if ((t2 & 4) != 0) {
        s32 t3 = (*(u16 *)((u8 *)((u8 *)p2 + 0x382) + (*(u16 *)((u8 *)p0 + 0x4) * 0x2c)) | 4);
        *(u16 *)((u8 *)((u8 *)p2 + 0x382) + (*(u16 *)((u8 *)p0 + 0x4) * 0x2c)) = t3;
        return t3;
    }
    s32 t4 = (*(u16 *)((u8 *)((u8 *)p2 + 0x382) + (*(u16 *)((u8 *)p0 + 0x4) * 0x2c)) & ~4);
    *(u16 *)((u8 *)((u8 *)p2 + 0x382) + (*(u16 *)((u8 *)p0 + 0x4) * 0x2c)) = t4;
    return t4;
}
