/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 TalkDemoRocks_Draw();
s32 TalkDemoRocks_Release();
s32 Task_Destroy();

void TalkDemoRocks_Main(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    *(u32 *)((u8 *)t0 + 0xc) = (*(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0xc) - 0x1);
    s32 t1 = *(u32 *)((u8 *)t0 + 0xc);
    if (t1 != 0) {
        TalkDemoRocks_Draw(t0, t1);
        return;
    }
    TalkDemoRocks_Release();
    Task_Destroy(*(u32 *)gTaskCurrent);
}

#pragma thumb reset
