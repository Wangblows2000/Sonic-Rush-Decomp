#include <nitro/types.h>

extern s32 *gExtraBossPlayerManager;

BOOL ExtraBoss_IsPlayerSpawned(void) {
    if (gExtraBossPlayerManager == NULL) {
        return FALSE;
    }
    return gExtraBossPlayerManager[1] != 0;
}
