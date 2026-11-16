/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 StageObj_SetAnimation();
void Needle_State_Unk_ov000_02310a68();

void Needle_Action_Unk_ov000_02310aa0(u8 *p0) {
    StageObj_SetAnimation(p0, *(u16 *)((u8 *)p0 + 0x1f8));
    *(u32 *)((u8 *)p0 + 0x68) = (u32)Needle_State_Unk_ov000_02310a68;
    *(u32 *)((u8 *)p0 + 0xc) = 0;
    *(u32 *)((u8 *)p0 + 0x8) = 0;
    *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 2);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) & ~0x20);
    *(u32 *)((u8 *)p0 + 0x8) = 0x18;
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 0x10);
}
