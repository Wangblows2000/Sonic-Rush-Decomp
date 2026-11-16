/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 EffShadow_Create();

void Player_AttachGimmickTable(u8 *p0, s32 p1) {
    *(u32 *)((u8 *)p0 + 0x3e8) = p1;
    *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 1);
    *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) | 0x2000);
    *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) | 0x1000);
    *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) | 0x800);
    *(u32 *)((u8 *)p0 + 0x330) = (*(u32 *)((u8 *)p0 + 0x330) | 8);
    *(u32 *)((u8 *)p0 + 0x330) = (*(u32 *)((u8 *)p0 + 0x330) | 0x30);
    *(u32 *)((u8 *)p0 + 0x330) = (*(u32 *)((u8 *)p0 + 0x330) | 0x40000);
    *(u16 *)((u8 *)p0 + 0x3ec) = 0;
    *(u16 *)((u8 *)p0 + 0x3ee) = 0x28;
    s32 t0 = (*(u32 *)((u8 *)p0 + 0x330) & 0x200);
    if (t0 == 0) {
        EffShadow_Create(p0, p1, t0, 0x28);
    }
    *(u32 *)((u8 *)p0 + 0x330) = (*(u32 *)((u8 *)p0 + 0x330) | 0x200);
}
