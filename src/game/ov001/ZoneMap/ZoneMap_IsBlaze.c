#include <nitro/types.h>

extern s32 gGameState;

#pragma thumb on
BOOL ZoneMap_IsBlaze(void) {
    switch (gGameState) {
    case 0:
        return FALSE;
    case 1:
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
