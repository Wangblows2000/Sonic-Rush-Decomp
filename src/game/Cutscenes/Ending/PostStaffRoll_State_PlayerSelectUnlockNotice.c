/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 SysSound_PlaySE();
s32 MsgBox_RequestClose();
s32 MsgBox_IsClosed();
s32 SysSound_Release();
s32 Task_Destroy();
s32 SaveData_UnlockPlayerSelectPage();
s32 SaveData_CommitOrShowError();
s32 PostStaffRoll_ShowSoundTestUnlockNotice();

void PostStaffRoll_State_PlayerSelectUnlockNotice(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    if (*(u32 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) == 0xa) {
        SysSound_PlaySE(0x15);
    }
    if (*(u32 *)t0 == 0x28) {
        SysSound_PlaySE(0x16);
    }
    s32 t1 = *(u32 *)t0;
    if (t1 == 0xf0) {
        MsgBox_RequestClose(t1);
    }
    s32 t2 = (*(u32 *)t0 + 0x1);
    *(u32 *)t0 = t2;
    s32 t3 = MsgBox_IsClosed(t2);
    if (t3 != 0) {
        *(volatile u32 *)0x4000000 = ((*(volatile u32 *)0x4000000 & -7937) | 0x1000);
        s32 t4 = ((*(volatile u32 *)0x4001000 & -7937) | 0x1000);
        *(volatile u32 *)0x4001000 = t4;
        s32 t5 = *(u32 *)((u8 *)t0 + 0x4);
        SysSound_Release(-7937, 0x1000, 0x4001000, t4);
        Task_Destroy(*(u32 *)gTaskCurrent);
        SaveData_UnlockPlayerSelectPage();
        s32 t6 = SaveData_CommitOrShowError(0);
        if (t6 != 0) {
            PostStaffRoll_ShowSoundTestUnlockNotice(t5);
        }
    }
}

#pragma thumb reset
