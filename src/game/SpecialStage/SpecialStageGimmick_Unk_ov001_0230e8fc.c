/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void SpecialStageGimmick_Unk_ov001_0230e78c();

void SpecialStageGimmick_Unk_ov001_0230e8fc(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0x20) = (*(u32 *)((u8 *)p0 + 0x20) + *(u32 *)((u8 *)p0 + 0x2c));
    *(u32 *)((u8 *)p0 + 0x24) = (*(u32 *)((u8 *)p0 + 0x24) + *(u32 *)((u8 *)p0 + 0x30));
    s32 t0 = *(u32 *)((u8 *)p0 + 0x28);
    *(u32 *)((u8 *)p0 + 0x28) = (*(u32 *)((u8 *)p0 + 0x28) + *(u32 *)((u8 *)p0 + 0x34));
    s32 t1 = *(u32 *)((u8 *)p0 + 0xd0);
    if (t1 >= 0x14) {
        *(u32 *)((u8 *)p0 + 0xcc) = (u32)SpecialStageGimmick_Unk_ov001_0230e78c;
        *(u32 *)((u8 *)p0 + 0xd0) = 0;
    }
}
