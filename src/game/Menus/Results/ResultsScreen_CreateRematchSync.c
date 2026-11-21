/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

void ResultsScreen_RematchSync_Main();
s32 Task_Create();

void ResultsScreen_CreateRematchSync(void) {
    Task_Create((u32)ResultsScreen_RematchSync_Main, 0, 0, 0x1000, 0x1000, 0);
}

#pragma thumb reset
