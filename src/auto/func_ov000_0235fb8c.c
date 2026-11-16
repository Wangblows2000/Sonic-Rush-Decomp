/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 StageLoader_Init();
s32 StageLoader_LoadZoneFiles();

void func_ov000_0235fb8c(void) {
    StageLoader_Init(1);
    StageLoader_LoadZoneFiles();
}

#pragma thumb reset
