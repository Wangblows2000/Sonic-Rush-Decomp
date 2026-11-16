#include <nitro/types.h>

extern s32 *gExtraBossPlayerManager;

BOOL func_ov001_02300c68(void) {
    if (gExtraBossPlayerManager == NULL) {
        return FALSE;
    }
    return gExtraBossPlayerManager[1] != 0;
}
