/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 TimeAttackMenu_CreateFadeTask();
s32 TimeAttackMenu_Unk_ov001_022ea3e0();
s32 TimeAttackMenu_Unk_ov001_022ea41c();
s32 TimeAttackMenu_Unk_ov001_022ea7c4();
s32 TimeAttackMenu_Unk_ov001_022ea6e4();
s32 SysSound_PlaySE();
void TimeAttackMenu_Unk_ov001_022ea894();

void TimeAttackMenu_Unk_ov001_022eb994(s32 p0) {
    s32 v0 = (s32)p0;
    if ((s32)p0 == 0) {
        v0 = *(u32 *)gTaskCurrent;
    }
    s32 t1 = *(u32 *)((u8 *)v0 + 0x10);
    *(u32 *)t1 = 0;
    s32 v2;
    if ((u32)*(u32 *)((u8 *)t1 + 0x4) <= (u32)1) {
        v2 = 0x10;
    } else {
        v2 = -16;
    }
    if (*(u32 *)(t1 + 0xe4) != 0) {
        TimeAttackMenu_CreateFadeTask(0, v2, 8, 8);
        TimeAttackMenu_Unk_ov001_022ea3e0(t1);
    } else {
        TimeAttackMenu_CreateFadeTask(0, v2, 8, 0);
        TimeAttackMenu_Unk_ov001_022ea41c(t1);
    }
    TimeAttackMenu_Unk_ov001_022ea7c4(t1);
    TimeAttackMenu_Unk_ov001_022ea6e4(t1);
    SysSound_PlaySE(1);
    *(u32 *)((u8 *)v0 + 0x8) = (u32)TimeAttackMenu_Unk_ov001_022ea894;
}

#pragma thumb reset
