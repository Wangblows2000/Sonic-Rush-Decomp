/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 StageObj_SetAnimation();
void KuragenBomb_State_Hold();

void KuragenBomb_Action_Hold(u8 *p0) {
    StageObj_SetAnimation(p0, 1);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u32 *)((u8 *)p0 + 0xc) = 0x3c;
    *(u32 *)((u8 *)p0 + 0x68) = (u32)KuragenBomb_State_Hold;
}
