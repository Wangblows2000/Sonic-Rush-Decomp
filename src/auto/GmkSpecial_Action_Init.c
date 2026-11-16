/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 StageObj_SetAnimation();
void GmkSpecial_State_Main();

void GmkSpecial_Action_Init(u8 *p0) {
    StageObj_SetAnimation(p0, 0);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 0x10);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)GmkSpecial_State_Main;
}
