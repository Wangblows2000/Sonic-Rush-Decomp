/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 StageObj_SetAnimation();
void Cannon_State_Unk_ov000_023043d0();

void Cannon_Action_Unk_ov000_023046c0(u8 *p0) {
    StageObj_SetAnimation(p0, 0);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)Cannon_State_Unk_ov000_023043d0;
    *(u32 *)((u8 *)p0 + 0x8) = ((*(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x4) & 3) << 0xe);
    *(u8 *)((u8 *)p0 + 0x1) = ((u32)*(u32 *)((u8 *)p0 + 0x8) >> 8);
}
