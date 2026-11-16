/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_ov000_0235cc60();
s32 func_ov000_0235cc4c();
s32 func_ov000_0235cbf0();
s32 ResultsScreen_CreateTask();
s32 ResultsScreen_InitDisplay();
s32 func_020513a8();
s32 Stage_FreeZoneFiles();
s32 Stage_FreeCommonFiles();
s32 ResultsScreen_LoadSprites();
s32 ResultsScreen_InitSprites();
s32 ResultsScreen_InitBackgrounds();
s32 ResultsScreen_LoadArchives();
s32 func_ov000_0235d888();
s32 func_ov000_0235d7b4();
s32 ResultsScreen_LoadPlayerNames();
s32 ResultsScreen_CreateLinkIcon();
s32 Touch_Enable();

void ResultsScreen_Create(void) {
    s32 t0 = func_ov000_0235cc60();
    s32 t1 = func_ov000_0235cc4c();
    s32 t2 = func_ov000_0235cbf0();
    s32 t3 = ResultsScreen_CreateTask(t0, t1, t2);
    ResultsScreen_InitDisplay();
    s32 t4 = func_020513a8();
    if (t4 == 0) {
        Stage_FreeZoneFiles();
        Stage_FreeCommonFiles();
    }
    ResultsScreen_LoadSprites(t3);
    ResultsScreen_InitSprites(t3);
    ResultsScreen_InitBackgrounds(t3);
    ResultsScreen_LoadArchives(t3);
    func_ov000_0235d888(t3, 0);
    func_ov000_0235d7b4(t3, 0);
    ResultsScreen_LoadPlayerNames(t3);
    ResultsScreen_CreateLinkIcon((t3 + 0x4ec));
    Touch_Enable();
}

#pragma thumb reset
