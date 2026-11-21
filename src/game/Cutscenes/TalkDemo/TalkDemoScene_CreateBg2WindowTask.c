/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

void TalkDemoSceneBg2Window_Main();
s32 Task_Create();

void TalkDemoScene_CreateBg2WindowTask(void) {
    Task_Create((u32)TalkDemoSceneBg2Window_Main, 0, 7, 0xf00a, 3, 0);
}

#pragma thumb reset
