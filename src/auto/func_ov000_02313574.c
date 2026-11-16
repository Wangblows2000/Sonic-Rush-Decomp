/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 StageObj_SetAnimation();
void Duct_State_Unk_ov000_02313500();

void func_ov000_02313574(u8 *p0) {
    StageObj_SetAnimation(p0, 0);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 0x10);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)Duct_State_Unk_ov000_02313500;
}
