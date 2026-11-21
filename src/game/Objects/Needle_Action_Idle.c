/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 StageObj_SetAnimation();
void Needle_State_Idle();

void Needle_Action_Idle(u8 *p0) {
    StageObj_SetAnimation(p0, *(u16 *)((u8 *)p0 + 0x1f8));
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)Needle_State_Idle;
    *(u32 *)((u8 *)p0 + 0xc) = 0;
}
