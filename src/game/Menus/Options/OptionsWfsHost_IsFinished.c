#include <nitro/types.h>

extern s32 gOptionsWfsHostTask;

#pragma thumb on
BOOL OptionsWfsHost_IsFinished(void) {
    if (gOptionsWfsHostTask == 0) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
