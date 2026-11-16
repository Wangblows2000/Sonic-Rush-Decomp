/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 ResultsScreen_SetupOrthoCamera();
s32 ResultsScreen_DrawModelB();
s32 ResultsScreen_DrawModelA();
s32 ResultsScreen_DrawModelsNop();

void ResultsScreen_DrawModels(u8 *p0) {
    ResultsScreen_SetupOrthoCamera();
    if (*(u32 *)((u8 *)p0 + 0x18) == 0) {
        ResultsScreen_DrawModelB(p0, 0x38, 0x90, 1);
    } else {
        ResultsScreen_DrawModelA(p0, 0x38, 0x90, 1);
    }
    if (*(u32 *)((u8 *)p0 + 0x1c) == 0) {
        ResultsScreen_DrawModelB(p0, 0xc8, 0x90, 0);
    } else {
        ResultsScreen_DrawModelA(p0, 0xc8, 0x90, 0);
    }
    ResultsScreen_DrawModelsNop(p0);
}

#pragma thumb reset
