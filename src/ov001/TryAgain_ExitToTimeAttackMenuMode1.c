#include <nitro/types.h>

void SceneChange_ToTimeAttackMenuMode1(void);

#pragma thumb on
void TryAgain_ExitToTimeAttackMenuMode1(void) {
    SceneChange_ToTimeAttackMenuMode1();
}
#pragma thumb reset
