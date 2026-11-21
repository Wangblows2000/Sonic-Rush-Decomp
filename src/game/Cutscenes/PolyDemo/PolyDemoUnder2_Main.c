/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 PlayerDemo_GetWork();
s32 Task_Destroy();
s32 PolyDemoUnder2_DrawSprites();

void PolyDemoUnder2_Main(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t1 = PlayerDemo_GetWork(*(u32 *)gTaskCurrent);
    *(u32 *)((u8 *)t0 + 0x4) = t1;
    if (*(u32 *)((u8 *)t0 + 0x4) == 0) {
        Task_Destroy(*(u32 *)gTaskCurrent);
        return;
    }
    PolyDemoUnder2_DrawSprites(t0);
}

#pragma thumb reset
