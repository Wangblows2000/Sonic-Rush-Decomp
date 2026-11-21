#include <nitro/types.h>

extern s32 gGameState[];

#pragma thumb on
BOOL ResultsScreen_Unk_ov000_0235cbd8(void) {
    if (gGameState[0] == gGameState[1])
        return TRUE;
    return FALSE;
}
#pragma thumb reset
