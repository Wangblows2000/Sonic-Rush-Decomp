#include <nitro/types.h>

extern s32 *gExtraBossPlayerManager;

s32 ExtraBoss_GetPlayer(void) {
    return gExtraBossPlayerManager[1];
}
