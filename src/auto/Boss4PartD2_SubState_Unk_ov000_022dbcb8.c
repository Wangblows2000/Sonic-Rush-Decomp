/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void Boss4PartD2_SubState_Unk_ov000_022dbcb8(u8 *p0) {
    if (*(u32 *)((u8 *)p0 + 0x4d8) != 0) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x4dc) = (*(u32 *)((u8 *)p0 + 0x4dc) - 0x40);
    s32 t0 = *(u32 *)((u8 *)p0 + 0x4dc);
    if (t0 < 0x700) {
        *(u32 *)((u8 *)p0 + 0x4dc) = 0x700;
    }
    *(u32 *)((u8 *)p0 + 0x4e0) = (*(u32 *)((u8 *)p0 + 0x4e0) + *(u32 *)((u8 *)p0 + 0x4dc));
    *(u32 *)((u8 *)p0 + 0x4e4) = (*(u32 *)((u8 *)p0 + 0x4e4) - *(u32 *)((u8 *)p0 + 0x4dc));
}
