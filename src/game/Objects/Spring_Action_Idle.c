/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 StageObj_SetAnimation();
void Spring_State_Idle();

void Spring_Action_Idle(u8 *p0) {
    StageObj_SetAnimation(p0, ((u32)(*(u32 *)((u8 *)p0 + 0x8) << 0x10) >> 0x10));
    *(u32 *)((u8 *)p0 + 0x68) = (u32)Spring_State_Idle;
    s32 t0 = (*(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x4) & 5);
    if (t0 != 0) {
        *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 0x20);
    }
}
