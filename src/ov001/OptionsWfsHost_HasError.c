#include <nitro/types.h>

extern s32 gOptionsWfsHostError;

#pragma thumb on
BOOL OptionsWfsHost_HasError(void) {
    if (gOptionsWfsHostError != 0) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
