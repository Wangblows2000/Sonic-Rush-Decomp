#include <nitro/types.h>

void PlayerSelectMenu_ResetBgOffsets(void);

#pragma thumb on
void PlayerSelectMenu_ReleaseDisplay(void) {
    PlayerSelectMenu_ResetBgOffsets();
}
#pragma thumb reset
