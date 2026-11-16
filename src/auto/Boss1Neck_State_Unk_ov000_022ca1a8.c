/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void Boss1Neck_State_Unk_ov000_022ca1a8(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0xc);
    if (t0 < 0xa) {
        *(u32 *)((u8 *)p0 + 0xc) = (t0 + 0x1);
        s32 t1 = *(u32 *)((u8 *)p0 + 0xc);
        if (t1 == 0xa) {
            *(u16 *)((u8 *)p0 + 0x16a) = (*(u16 *)((u8 *)p0 + 0x16a) & ~4);
        }
    }
    s32 t2 = (*(u8 *)((u8 *)p0 + 0x318) & 2);
    if (t2 != 0) {
        *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 4);
    }
}
