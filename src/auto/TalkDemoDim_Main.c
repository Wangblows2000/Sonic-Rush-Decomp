/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 TalkDemo_IsActive();
s32 Task_Destroy();
s32 TalkDemo_GetFlags();

void TalkDemoDim_Main(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t1 = TalkDemo_IsActive(*(u32 *)gTaskCurrent);
    if (t1 == 0) {
        Task_Destroy(*(u32 *)gTaskCurrent);
        return;
    }
    s32 t2 = *(u32 *)t0;
    if (t2 != 0) {
        ((s32 (*)())t2)(t0, t2);
    }
    s32 t3 = TalkDemo_GetFlags(2);
    if ((t3 & 0x80000000) != 0) {
        Task_Destroy(*(u32 *)gTaskCurrent, 0x80000000);
    }
}

#pragma thumb reset
