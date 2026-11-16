#include <nitro/types.h>

void OptionsWfsHost_IsFinished(void);

#pragma thumb on
void OptionsMenu_CheckWfsHostFinished(void) {
    OptionsWfsHost_IsFinished();
}
#pragma thumb reset
