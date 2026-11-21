/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 Task_Destroy();
void ClearDemo_Create();
s32 Player_LoadArchive();
extern u8 gGameState[];
s32 SaveData_SetSonicProgress();
s32 SaveData_SetBlazeProgress();
s32 SaveData_StoreLives();

void DemoScript_Main(void) {
    u32 t0 = *(u32 *)gTaskCurrent;
    u32 t1 = *(u32 *)((u8 *)t0 + 0x10);
    u32 t2 = *(u32 *)t1;
    if (t2 != 0) {
        ((s32 (*)())t2)(t1, t2);
    }
    u32 t3 = *(u32 *)((u8 *)t1 + 0xc);
    if ((t3 & 0x80000000) != 0) {
        u32 t4 = *(u32 *)((u8 *)t1 + 0x4);
        u32 t5 = *(u32 *)((u8 *)t1 + 0x1c);
        u32 t6 = *(u32 *)gTaskCurrent;
        Task_Destroy(t6, (t3 & 0x80000000));
        if (t4 == (s32)(u32)ClearDemo_Create) {
            Player_LoadArchive((u32)ClearDemo_Create);
        }
        if ((t3 & 1) != 0) {
            u32 t7 = *(u32 *)gGameState;
            if (t7 != 1) {
                SaveData_SetSonicProgress(t5);
            } else {
                SaveData_SetBlazeProgress(t5);
            }
            SaveData_StoreLives();
            ((s32 (*)())t4)();
            return;
        }
        ((s32 (*)())t4)();
    }
}

#pragma thumb reset
