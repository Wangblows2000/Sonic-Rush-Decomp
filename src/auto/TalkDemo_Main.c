/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 TalkDemo_CheckSkip();
s32 Task_Destroy();

void TalkDemo_Main(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    TalkDemo_CheckSkip(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    s32 t1 = *(u32 *)t0;
    if (t1 != 0) {
        ((s32 (*)())t1)(t0, t1);
    }
    s32 t2 = (*(u32 *)((u8 *)t0 + 0x4) & 0x80000000);
    if (t2 != 0) {
        Task_Destroy(*(u32 *)gTaskCurrent, t2);
    }
}

#pragma thumb reset
