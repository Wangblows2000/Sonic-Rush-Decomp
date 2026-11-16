/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 StageObj_SetAnimation();
void Saucer_State_Ridden();
s32 Sound_PlaySeqArcEx();

void Saucer_Action_Ridden(u8 *p0) {
    StageObj_SetAnimation(p0, 1);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)Saucer_State_Ridden;
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x2000);
    *(u32 *)((u8 *)p0 + 0xc) = 0;
    *(u16 *)((u8 *)p0 + 0x34) = 0;
    *(u16 *)((u8 *)p0 + 0x34) = 0;
    *(u32 *)((u8 *)p0 + 0x8) = 0x100;
    *(u16 *)((u8 *)p0 + 0x62) = 0x60;
    Sound_PlaySeqArcEx(*(u32 *)((u8 *)p0 + 0x1dc), 0, 0x6c);
}
