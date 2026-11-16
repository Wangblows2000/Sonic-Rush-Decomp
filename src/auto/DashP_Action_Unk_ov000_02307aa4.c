/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 StageObj_SetAnimation();
void DashP_State_Unk_ov000_02307a90();

void DashP_Action_Unk_ov000_02307aa4(u8 *p0) {
    StageObj_SetAnimation(p0, ((u32)(*(u32 *)((u8 *)p0 + 0x8) << 0x10) >> 0x10));
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)DashP_State_Unk_ov000_02307a90;
}
