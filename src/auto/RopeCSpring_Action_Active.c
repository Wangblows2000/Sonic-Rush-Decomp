/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 StageObj_SetAnimation();
void RopeCSpring_State_Active();

void RopeCSpring_Action_Active(u8 *p0) {
    StageObj_SetAnimation(p0, 0);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)RopeCSpring_State_Active;
    *(u16 *)((u8 *)p0 + 0x62) = 0x80;
    *(u32 *)((u8 *)p0 + 0xc) = 0x10;
    *(u16 *)((u8 *)p0 + 0x44) = 0x140;
    s32 t0 = *(s16 *)((u8 *)p0 + 0x44);
    *(u16 *)((u8 *)p0 + 0x46) = t0;
}
