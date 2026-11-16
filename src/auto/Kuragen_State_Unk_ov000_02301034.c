/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_02300fd0();

void Kuragen_State_Unk_ov000_02301034(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x88);
    if (t0 == 0) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x10) = *(u32 *)((u8 *)t0 + 0x10);
    *(u32 *)((u8 *)p0 + 0x14) = (*(u32 *)((u8 *)t0 + 0x14) + 0xf00);
    s32 t1 = (*(u32 *)((u8 *)t0 + 0x238) & 0x20);
    if (t1 == 0) {
        return;
    }
    func_ov000_02300fd0(p0, t1);
    *(u32 *)((u8 *)t0 + 0x238) = (*(u32 *)((u8 *)t0 + 0x238) & ~0x20);
    *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 0x200);
}
