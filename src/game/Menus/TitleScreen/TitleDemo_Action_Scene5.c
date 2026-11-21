/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
void TitleDemo_State_Scene5();

void TitleDemo_Action_Scene5(s32 p0) {
    s32 v0 = (s32)p0;
    if ((s32)p0 == 0) {
        v0 = *(u32 *)gTaskCurrent;
    }
    s32 t1 = *(u32 *)((u8 *)v0 + 0x10);
    *(u32 *)t1 = 0;
    *(u8 *)(t1 + 0x120) = (*(u8 *)(t1 + 0x120) & ~1);
    *(u8 *)(t1 + 0x258) = (*(u8 *)(t1 + 0x258) & ~1);
    *(volatile u32 *)0x4000000 = ((*(volatile u32 *)0x4000000 & -7937) | 0x300);
    *(volatile u32 *)0x4001000 = ((*(volatile u32 *)0x4001000 & -7937) | 0x1200);
    *(u32 *)((u8 *)v0 + 0x8) = (u32)TitleDemo_State_Scene5;
}

#pragma thumb reset
