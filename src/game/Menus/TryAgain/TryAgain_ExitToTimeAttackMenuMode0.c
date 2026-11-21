#include <nitro/types.h>

void SceneChange_ToTimeAttackMenuMode0(void);

#pragma thumb on
void TryAgain_ExitToTimeAttackMenuMode0(void) {
    SceneChange_ToTimeAttackMenuMode0();
}
#pragma thumb reset
