/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 PlayerDemo_GetWork();
s32 PlayerDemo_IsActive();
s32 Task_Destroy();

void PlayerDemoSkip_Main(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t1 = PlayerDemo_GetWork(*(u32 *)gTaskCurrent);
    *(u32 *)t0 = t1;
    s32 t2 = PlayerDemo_IsActive();
    if (t2 == 0) {
        Task_Destroy(*(u32 *)gTaskCurrent);
        return;
    }
    *(u16 *)((u8 *)t0 + 0x4) = (*(s16 *)((u8 *)t0 + 4) - 0x1);
    if (*(s16 *)((u8 *)t0 + 4) <= 0) {
        s32 t3 = (*(u32 *)((u8 *)*(u32 *)t0 + 0x4) | 0x80000000);
        s32 t4 = *(u32 *)t0;
        *(u32 *)((u8 *)t4 + 0x4) = t3;
        Task_Destroy(*(u32 *)gTaskCurrent, t3, t4);
    }
}

#pragma thumb reset
